#include "args.hpp"
#include "boolean_query.hpp"
#include "io.hpp"
#include "mongo_reader.hpp"

#include <httplib.h>

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

static std::vector<std::string> collect_args(int argc, char** argv) {
    std::vector<std::string> a;
    a.reserve(static_cast<size_t>(argc));

    for (int i = 1; i < argc; ++i) {
        a.emplace_back(argv[i]);
    }

    return a;
}

static std::string html_escape(const std::string& s) {
    std::string out;
    out.reserve(s.size());

    for (char c : s) {
        switch (c) {
            case '&': out += "&amp;"; break;
            case '<': out += "&lt;"; break;
            case '>': out += "&gt;"; break;
            case '"': out += "&quot;"; break;
            case '\'': out += "&#39;"; break;
            default: out.push_back(c); break;
        }
    }

    return out;
}

static std::string render_home() {
    std::ostringstream h;

    h << "<!doctype html><html><head><meta charset='utf-8'><title>IR Search</title></head><body>";
    h << "<h2>IR Search</h2>";
    h << "<form action='/search' method='get'>";
    h << "<input style='width:520px' name='q' placeholder='query' />";
    h << "<button type='submit'>Search</button>";
    h << "</form>";
    h << "<p>Syntax: <b>AND</b> / <b>OR</b> / <b>NOT</b>, parentheses. Implicit AND: <code>rust mongodb</code></p>";
    h << "</body></html>";

    return h.str();
}

int main(int argc, char** argv) {
    auto args = collect_args(argc, argv);

    const std::string index_path = ir::get_arg(args, "--index", "index.bin");
    const int port = static_cast<int>(ir::get_arg_i64(args, "--port", 8080));

    ir::InvertedIndex idx;
    try {
        ir::load_index(idx, index_path);
    } catch (const std::exception& e) {
        std::cerr << "Failed to load index '" << index_path << "': " << e.what() << "\n";

        return 2;
    }

    // Mongo config for /doc endpoint (view raw_html)
    ir::MongoConfig mcfg;
    mcfg.uri = ir::get_arg(args, "--mongo-uri", "mongodb://localhost:27017");
    mcfg.db = ir::get_arg(args, "--db", "mai_ir_crawler");
    mcfg.collection = ir::get_arg(args, "--collection", "documents");

    httplib::Server svr;

    svr.Get("/", [&](const httplib::Request&, httplib::Response& res) {
        res.set_content(render_home(), "text/html; charset=utf-8");
    });

    svr.Get("/healthz", [&](const httplib::Request&, httplib::Response& res) {
        res.set_content("ok\n", "text/plain; charset=utf-8");
    });

    svr.Get("/search", [&](const httplib::Request& req, httplib::Response& res) {
        std::string q;
        if (req.has_param("q")) {
            q = req.get_param_value("q");
        }

        std::ostringstream h;
        h << "<!doctype html><html><head><meta charset='utf-8'><title>Results</title></head><body>";
        h << "<a href='/'>← back</a>";
        h << "<h3>Query: <code>" << html_escape(q) << "</code></h3>";

        auto r = ir::boolean_search(idx, q);
        if (!r.error.empty()) {
            h << "<p style='color:red'>Error: " << html_escape(r.error) << "</p>";
            h << "</body></html>";

            res.set_content(h.str(), "text/html; charset=utf-8");

            return;
        }

        h << "<p>Hits: " << r.doc_ids.size() << "</p>";
        h << "<ol>";
        for (uint32_t doc_id : r.doc_ids) {
            const auto& d = idx.doc(doc_id);

            h << "<li>";
            h << "<a href='" << html_escape(d.url) << "' target='_blank'>" << html_escape(d.url) << "</a>";
            h << " <small>(" << html_escape(d.source_name) << ")</small>";
            h << " <a href='/doc?id=" << doc_id << "'>[view raw_html]</a>";
            h << "</li>";
        }
        h << "</ol>";
        h << "</body></html>";

        res.set_content(h.str(), "text/html; charset=utf-8");
    });

    svr.Get("/doc", [&](const httplib::Request& req, httplib::Response& res) {
        std::string id_s;
        if (req.has_param("id")) {
            id_s = req.get_param_value("id");
        }

        uint32_t doc_id = 0;
        try {
            doc_id = static_cast<uint32_t>(std::stoul(id_s));
        } catch (...) {
            res.status = 400;
            res.set_content("bad id\n", "text/plain; charset=utf-8");

            return;
        }

        if (doc_id >= idx.doc_count()) {
            res.status = 404;
            res.set_content("not found\n", "text/plain; charset=utf-8");

            return;
        }

        const auto& d = idx.doc(doc_id);
        std::ostringstream h;
        h << "<!doctype html><html><head><meta charset='utf-8'><title>Document</title></head><body>";
        h << "<a href='/search?q='>← back to search</a>";
        h << "<h3><a href='" << html_escape(d.url) << "' target='_blank'>" << html_escape(d.url) << "</a></h3>";
        h << "<p><small>source: " << html_escape(d.source_name) << "</small></p>";

        if (!d.mongo_id.empty()) {
            auto raw = ir::fetch_raw_html_by_id(mcfg, d.mongo_id);
            if (raw.empty()) {
                h << "<p style='color:gray'>(raw_html not found in Mongo)</p>";
            } else {
                h << "<pre style='white-space:pre-wrap; word-break:break-word; border:1px solid #ccc; padding:12px;'>";
                h << html_escape(raw);
                h << "</pre>";
            }
        } else {
            h << "<p style='color:gray'>(mongo_id missing in index)</p>";
        }

        h << "</body></html>";
        res.set_content(h.str(), "text/html; charset=utf-8");
    });

    std::cout << "Listening on 0.0.0.0:" << port << "\n";
    if (!svr.listen("0.0.0.0", port)) {
        std::cerr << "Failed to listen on port " << port << "\n";

        return 3;
    }
    
    return 0;
}