#pragma once
#include <string>
#include <vector>
#include "index.hpp"

namespace ir {

struct MongoConfig {
    std::string uri = "mongodb://localhost:27017";
    std::string db = "mai_ir_crawler";
    std::string collection = "documents";
    int64_t limit = -1; // -1 = no limit
};

// Fetch documents from MongoDB and feed into index.
// Throws std::runtime_error if Mongo driver is not enabled.
void index_from_mongo(InvertedIndex& idx, const MongoConfig& cfg);

// Fetch raw_html by mongo_id (ObjectId hex string). Returns empty string if not found.
std::string fetch_raw_html_by_id(const MongoConfig& cfg, const std::string& mongo_id_hex);

} // namespace ir