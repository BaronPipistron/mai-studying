#include "html_strip.hpp"
#include <cctype>
#include <string>

namespace ir {

static int hexval(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    return -1;
}

static void append_utf8_cp(std::string& out, uint32_t cp) {
    if (cp <= 0x7F) { 
        out.push_back(static_cast<char>(cp));
    } else if (cp <= 0x7FF) {
        out.push_back(static_cast<char>(0xC0 | (cp >> 6)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else if (cp <= 0xFFFF) {
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

static bool decode_entity(const std::string& ent, std::string& out) {
    if (ent == "&amp;") { out.push_back('&'); return true; }
    if (ent == "&lt;") { out.push_back('<'); return true; }
    if (ent == "&gt;") { out.push_back('>'); return true; }
    if (ent == "&quot;") { out.push_back('"'); return true; }
    if (ent == "&apos;") { out.push_back('\''); return true; }
    if (ent == "&nbsp;") { out.push_back(' '); return true; }

    // numeric: &#123; or &#x1F60A;
    if (ent.size() >= 4 && ent[0] == '&' && ent[1] == '#') {
        uint32_t cp = 0;
        size_t i = 2;
        int base = 10;

        if (i < ent.size() && (ent[i] == 'x' || ent[i] == 'X')) { 
            base = 16; ++i; 
        }

        for (; i < ent.size(); ++i) {
            char c = ent[i];

            if (c == ';') {
                break;
            }

            int v = (base == 16) ? hexval(c) : (std::isdigit(static_cast<unsigned char>(c)) ? (c - '0') : -1);

            if (v < 0) { 
                return false;
            }

            cp = cp * base + static_cast<uint32_t>(v);
        }

        if (cp > 0) { 
            append_utf8_cp(out, cp); 
            return true; 
        }
    }

    return false;
}

std::string html_to_text(const std::string& html) {
    std::string out;
    out.reserve(html.size());

    bool in_tag = false;
    bool last_space = false;

    std::string ent;
    bool in_ent = false;

    for (size_t i = 0; i < html.size(); ++i) {
        char c = html[i];

        if (in_tag) {
            if (c == '>') in_tag = false;
            continue;
        }

        if (in_ent) {
            ent.push_back(c);

            if (c == ';' || ent.size() > 16) {
                if (!decode_entity(ent, out)) {
                    // fallback: keep as-is without '&'/'...'
                    // add a space to avoid merging tokens
                    out.push_back(' ');
                }
                in_ent = false;
                ent.clear();
            }

            continue;
        }

        if (c == '<') { 
            in_tag = true; 
            continue; 
        }

        if (c == '&') { 
            in_ent = true; 
            ent = "&"; 
            continue; 
        }

        unsigned char uc = static_cast<unsigned char>(c);
        
        if (std::isspace(uc)) {
            if (!last_space) out.push_back(' ');
            last_space = true;
        } else {
            out.push_back(c);
            last_space = false;
        }
    }

    return out;
}

} // namespace ir
