#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace ir::utf8 {

// Decode UTF-8 into code points (char32_t). Invalid sequences are replaced with U+FFFD.
inline std::vector<char32_t> decode(const std::string& s) {
    std::vector<char32_t> out;
    out.reserve(s.size());

    const uint8_t* p = reinterpret_cast<const uint8_t*>(s.data());
    size_t i = 0, n = s.size();

    while (i < n) {
        uint8_t c = p[i];

        if (c < 0x80) {
            out.push_back(static_cast<char32_t>(c));
            ++i;
        } else if ((c >> 5) == 0x6 && i + 1 < n) {
            uint8_t c1 = p[i + 1];

            if ((c1 & 0xC0) != 0x80) { 
                out.push_back(0xFFFD); ++i; 
                continue; 
            }

            char32_t cp = ((c & 0x1F) << 6) | (c1 & 0x3F);
            out.push_back(cp);

            i += 2;
        } else if ((c >> 4) == 0xE && i + 2 < n) {
            uint8_t c1 = p[i + 1], c2 = p[i + 2];

            if ((c1 & 0xC0) != 0x80 || (c2 & 0xC0) != 0x80) { 
                out.push_back(0xFFFD);
                ++i; 
                continue; 
            }

            char32_t cp = ((c & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);
            out.push_back(cp);

            i += 3;
        } else if ((c >> 3) == 0x1E && i + 3 < n) {
            uint8_t c1 = p[i + 1], c2 = p[i + 2], c3 = p[i + 3];

            if ((c1 & 0xC0) != 0x80 || (c2 & 0xC0) != 0x80 || (c3 & 0xC0) != 0x80) { 
                out.push_back(0xFFFD); 
                ++i; 
                continue; 
            }

            char32_t cp = ((c & 0x07) << 18) | ((c1 & 0x3F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F);
            out.push_back(cp);

            i += 4;
        } else {
            out.push_back(0xFFFD);
            ++i;
        }
    }

    return out;
}

inline void append_utf8(std::string& out, char32_t cp) {
    if (cp < 0x80) {
        out.push_back(static_cast<char>(cp));
    } else if (cp < 0x800) {
        out.push_back(static_cast<char>(0xC0 | (cp >> 6)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else if (cp < 0x10000) {
        out.push_back(static_cast<char>(0xE0 | (cp >> 12)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else {
        out.push_back(static_cast<char>(0xF0 | (cp >> 18)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    }
}

inline std::string encode(const std::vector<char32_t>& cps) {
    std::string out;
    out.reserve(cps.size());

    for (auto cp : cps) {
        append_utf8(out, cp);
    }

    return out;
}

inline bool is_latin_letter(char32_t cp) {
    return (cp >= U'A' && cp <= U'Z') || (cp >= U'a' && cp <= U'z');
}

inline bool is_digit(char32_t cp) { 
    return (cp >= U'0' && cp <= U'9'); 
}

inline bool is_cyrillic_letter(char32_t cp) {
    // Basic Cyrillic + Ё/ё
    return (cp >= 0x0410 && cp <= 0x044F) || cp == 0x0401 || cp == 0x0451;
}

inline char32_t to_lower(char32_t cp) {
    // ASCII
    if (cp >= U'A' && cp <= U'Z') {
        return cp + 32;
    }
    // Cyrillic А-Я -> а-я, Ё -> ё
    if (cp >= 0x0410 && cp <= 0x042F) {
        return cp + 32;
    }

    if (cp == 0x0401) {
        return 0x0451;
    }

    return cp;
}

inline bool is_token_char(char32_t cp) {
    if (is_latin_letter(cp) || is_cyrillic_letter(cp) || is_digit(cp)) {
        return true;
    }
    
    // keep a small set of symbols useful for programming terms
    return (cp == U'+' || cp == U'#' || cp == U'_' || cp == U'-');
}

} // namespace ir::utf8
