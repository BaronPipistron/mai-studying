#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace ir {

struct TokenizationStats {
    uint64_t token_count = 0;
    uint64_t total_token_chars = 0; // in Unicode code points
    uint64_t input_bytes = 0;
};

// Tokenizes UTF-8 text into lowercase tokens.
// Rule: token = maximal sequence of token-chars (letters/digits/+/#/_/-). Everything else splits
std::vector<std::string> tokenize(const std::string& utf8_text, TokenizationStats* stats);

} // namespace ir
