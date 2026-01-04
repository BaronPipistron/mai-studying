#include "tokenizer.hpp"
#include "utf8.hpp"

namespace ir {

std::vector<std::string> tokenize(const std::string& utf8_text, TokenizationStats* stats) {
    if (stats) {
        stats->input_bytes += utf8_text.size();
    }

    std::vector<std::string> tokens;
    tokens.reserve(256);

    std::vector<char32_t> cps = utf8::decode(utf8_text);
    std::vector<char32_t> cur;
    cur.reserve(32);

    auto flush = [&]() {
        if (cur.empty()) {
            return;
        }

        if (stats) {
            stats->token_count += 1;
            stats->total_token_chars += cur.size();
        }

        tokens.push_back(utf8::encode(cur));
        cur.clear();
    };

    for (char32_t cp : cps) {
        cp = utf8::to_lower(cp);
        
        if (utf8::is_token_char(cp)) {
            cur.push_back(cp);
        } else {
            flush();
        }
    }
    flush();

    return tokens;
}

} // namespace ir
