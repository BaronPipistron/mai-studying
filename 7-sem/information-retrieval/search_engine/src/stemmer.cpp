#include "stemmer.hpp"
#include "utf8.hpp"
#include <algorithm>

namespace ir {

static bool contains_cyrillic(const std::vector<char32_t>& cps) {
    for (auto cp : cps) {
        if (utf8::is_cyrillic_letter(cp)) {
            return true;
        }
    }

    return false;
}

static bool ends_with(const std::vector<char32_t>& w, const std::u32string& suf) {
    if (w.size() < suf.size()) {
        return false;
    }

    for (size_t i = 0; i < suf.size(); ++i) {
        if (w[w.size() - suf.size() + i] != suf[i]) {
            return false;
        }
    }

    return true;
}

static void strip_suffix(std::vector<char32_t>& w, const std::u32string& suf) {
    if (ends_with(w, suf)) {
        w.resize(w.size() - suf.size());
    }
}

static std::string stem_ru(const std::string& tok) {
    auto w = utf8::decode(tok);

    // Keep only letters/digits/+/#/_/- in the token already; but for RU stemming remove non-letters/digits
    // Stem only if token contains Cyrillic letters
    if (w.size() < 4) { 
        return tok;
    }

    // Order matters: longer first
    static const std::u32string suffixes[] = {
        U"иями", U"ями", U"ами", U"ией", U"ий", U"ый", U"ого", U"его",
        U"ыми", U"ими", U"ыми", U"ому", U"ему", U"ах", U"ях",
        U"ов", U"ев", U"ей", U"ам", U"ям", U"ом", U"ем",
        U"ою", U"ею", U"ую", U"юю",
        U"ая", U"яя", U"ое", U"ее", U"ые", U"ие", U"ой", U"ей",
        U"а", U"я", U"у", U"ю", U"е", U"и", U"ы", U"о"
    };

    for (const auto& suf : suffixes) {
        if (w.size() <= 3) {
            break;
        }

        if (ends_with(w, suf)) {
            w.resize(w.size() - suf.size());
            break;
        }
    }

    if (w.size() < 2) {
        return tok;
    }

    return utf8::encode(w);
}

static std::string stem_en(const std::string& tok) {
    std::string s = tok;
    if (s.size() < 4) {
        return s;
    }

    for (auto& ch : s) {
        if (ch >= 'A' && ch <= 'Z') {
            ch = char(ch + 32);
        }
    }

    auto strip = [&](const std::string& suf) {
        if (s.size() > suf.size() + 2 && s.rfind(suf) == s.size() - suf.size()) {
            s.resize(s.size() - suf.size());
            
            return true;
        }

        return false;
    };

    //Suffix stripping
    if (strip("ingly") || strip("edly") || strip("ing") || strip("ed")) {}
    if (strip("ments") || strip("ment") || strip("ation") || strip("ations")) {}
    if (strip("ness") || strip("less") || strip("ful") || strip("ly")) {}
    if (strip("ies")) { s += "y"; }
    else if (strip("es")) {}
    else if (strip("s")) {}

    return s;
}

std::string stem(const std::string& token_utf8) {
    if (token_utf8.empty()) { 
        return token_utf8;
    }
    
    auto cps = utf8::decode(token_utf8);
    if (contains_cyrillic(cps)) {
        return stem_ru(token_utf8);
    }
    
    return stem_en(token_utf8);
}

} // namespace ir
