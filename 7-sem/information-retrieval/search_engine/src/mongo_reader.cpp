#include "mongo_reader.hpp"
#include <stdexcept>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/oid.hpp>
#include <bsoncxx/types.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/options/find.hpp>
#include <mongocxx/uri.hpp>

namespace ir {

// Достаточно одного instance на процесс
static mongocxx::instance g_mongo_instance{};

static bool is_string_field(const bsoncxx::document::element& el) {
    return el && el.type() == bsoncxx::type::k_string;
}

static std::string get_string(const bsoncxx::document::element& el) {
    return std::string(el.get_string().value);
}

void index_from_mongo(InvertedIndex& idx, const MongoConfig& cfg) {
    mongocxx::client client{mongocxx::uri{cfg.uri}};
    auto coll = client[cfg.db][cfg.collection];

    mongocxx::options::find opts;
    opts.projection(bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("url", 1),
        bsoncxx::builder::basic::kvp("raw_html", 1),
        bsoncxx::builder::basic::kvp("source_name", 1),
        bsoncxx::builder::basic::kvp("crawl_date", 1)
    ));

    if (cfg.limit > 0) {
        opts.limit(cfg.limit);
    }

    auto cursor = coll.find({}, opts);
    for (auto&& doc : cursor) {
        DocumentMeta meta;

        if (auto el = doc["_id"]; el && el.type() == bsoncxx::type::k_oid) {
            meta.mongo_id = el.get_oid().value.to_string();
        }

        if (auto el = doc["url"]; is_string_field(el)) {
            meta.url = get_string(el);
        }

        if (auto el = doc["source_name"]; is_string_field(el)) {
            meta.source_name = get_string(el);
        }

        if (auto el = doc["crawl_date"]; el) {
            if (el.type() == bsoncxx::type::k_int64) {
                meta.crawl_date = el.get_int64().value;
            } else if (el.type() == bsoncxx::type::k_int32) {
                meta.crawl_date = el.get_int32().value;
            }
        }

        std::string raw;
        if (auto el = doc["raw_html"]; is_string_field(el)) {
            raw = get_string(el);
        }

        idx.add_document(meta, raw);
    }
}

std::string fetch_raw_html_by_id(const MongoConfig& cfg, const std::string& mongo_id_hex) {
    mongocxx::client client{mongocxx::uri{cfg.uri}};
    auto coll = client[cfg.db][cfg.collection];

    bsoncxx::oid oid;
    try {
        oid = bsoncxx::oid{mongo_id_hex};
    } catch (...) {
        return {};
    }

    using bsoncxx::builder::basic::kvp;
    using bsoncxx::builder::basic::make_document;

    auto filter = make_document(kvp("_id", oid));

    mongocxx::options::find opts;
    opts.projection(make_document(kvp("raw_html", 1), kvp("url", 1)));

    auto res = coll.find_one(filter.view(), opts);
    if (!res) return {};

    auto view = res->view();
    if (auto el = view["raw_html"]; el && el.type() == bsoncxx::type::k_string) {
        return std::string(el.get_string().value);
    }
    
    return {};
}

} // namespace ir
