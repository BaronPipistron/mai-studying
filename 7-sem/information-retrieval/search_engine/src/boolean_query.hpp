#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "index.hpp"

namespace ir {

struct SearchResult {
    std::vector<uint32_t> doc_ids; // document ids matching boolean query
    std::string error;             // non-empty if parsing failed
};

SearchResult boolean_search(const InvertedIndex& idx, const std::string& query_utf8);

// Set operations on sorted doc id lists
std::vector<uint32_t> and_merge(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b);
std::vector<uint32_t> or_merge(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b);
std::vector<uint32_t> diff_merge(const std::vector<uint32_t>& all, const std::vector<uint32_t>& b);

} // namespace ir
