#include "boolean_query.hpp"
#include "stemmer.hpp"
#include "utf8.hpp"
#include <algorithm>
#include <cctype>
#include <stack>

namespace ir {

enum class TokType { TERM, AND, OR, NOT, LPAREN, RPAREN };

struct Tok {
    TokType type;
    std::string text; // for TERM
};

static std::string ascii_lower(std::string s) {
    for (char& c : s) if (c >= 'A' && c <= 'Z') c = char(c + 32);

    return s;
}

static std::string unicode_lower_utf8(const std::string& in) {
    auto cps = utf8::decode(in);

    for (auto& cp : cps) {
        cp = utf8::to_lower(cp);
    }

    return utf8::encode(cps);
}

static bool is_op(const Tok& t) {
    return t.type == TokType::AND || t.type == TokType::OR || t.type == TokType::NOT;
}

static int prec(TokType t) {
    if (t == TokType::NOT) return 3;
    if (t == TokType::AND) return 2;
    if (t == TokType::OR) return 1;

    return 0;
}

static std::vector<Tok> lex_query(const std::string& q) {
    std::vector<Tok> out;
    std::string cur;

    auto flush_term = [&]() {
        if (cur.empty()) return;

        std::string s = unicode_lower_utf8(cur);

        if (s == "and" || s == "&&") {
            out.push_back({TokType::AND, {}});
        } else if (s == "or" || s == "||") {
            out.push_back({TokType::OR, {}});
        } else if (s == "not" || s == "!") {
            out.push_back({TokType::NOT, {}});
        } else {
            out.push_back({TokType::TERM, stem(s)});
        }

        cur.clear();
    };

    for (size_t i = 0; i < q.size(); ++i) {
        char c = q[i];

        if (c == '(') { 
            flush_term(); 
            out.push_back({TokType::LPAREN,{}}); 
        } else if (c == ')') { 
            flush_term(); 
            out.push_back({TokType::RPAREN,{}}); 
        } else if (std::isspace(static_cast<unsigned char>(c))) { 
            flush_term(); 
        } else if (c == '!') { 
            flush_term(); 
            out.push_back({TokType::NOT,{}}); 
        } else {
            cur.push_back(c);
        }
    }

    flush_term();

    // remove empty terms after stemming
    std::vector<Tok> cleaned;
    cleaned.reserve(out.size());

    for (auto& t : out) {
        if (t.type == TokType::TERM && t.text.empty()) continue;
        cleaned.push_back(std::move(t));
    }

    return cleaned;
}

// Insert implicit AND between:
// (TERM or RPAREN) [AND inserted] (TERM or LPAREN or NOT)
static std::vector<Tok> insert_implicit_and(std::vector<Tok> in) {
    std::vector<Tok> out;
    out.reserve(in.size() * 2);

    auto is_lhs = [](TokType t) { return t == TokType::TERM || t == TokType::RPAREN; };
    auto is_rhs = [](TokType t) { return t == TokType::TERM || t == TokType::LPAREN || t == TokType::NOT; };

    for (size_t i = 0; i < in.size(); ++i) {
        out.push_back(in[i]);

        if (i + 1 < in.size()) {
            if (is_lhs(in[i].type) && is_rhs(in[i + 1].type)) {
                out.push_back({TokType::AND, {}});
            }
        }
    }

    return out;
}

std::vector<uint32_t> and_merge(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b) {
    std::vector<uint32_t> out;
    out.reserve(std::min(a.size(), b.size()));
    size_t i = 0, j = 0;

    while (i < a.size() && j < b.size()) {
        if (a[i] == b[j]) { 
            out.push_back(a[i]); 
            ++i; 
            ++j; 
        }
        else if (a[i] < b[j]) { 
            ++i;
        }
        else {
            ++j;
        }
    }

    return out;
}

std::vector<uint32_t> or_merge(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b) {
    std::vector<uint32_t> out;
    out.reserve(a.size() + b.size());
    size_t i = 0, j = 0;

    while (i < a.size() || j < b.size()) {
        if (j >= b.size() || (i < a.size() && a[i] < b[j])) { 
            out.push_back(a[i++]); 
        } else if (i >= a.size() || (j < b.size() && b[j] < a[i])) {
            out.push_back(b[j++]); 
        } else { 
            out.push_back(a[i]); ++i; ++j; 
        }
    }

    return out;
}

std::vector<uint32_t> diff_merge(const std::vector<uint32_t>& all, const std::vector<uint32_t>& b) {
    std::vector<uint32_t> out;
    out.reserve(all.size());
    size_t i = 0, j = 0;

    while (i < all.size()) {
        if (j >= b.size()) { 
            out.push_back(all[i++]); 
            continue; 
        }

        if (all[i] == b[j]) { 
            ++i; 
            ++j; 
        } else if (all[i] < b[j]) { 
            out.push_back(all[i++]); 
        } else { 
            ++j; 
        }
    }

    return out;
}

static SearchResult eval_rpn(const InvertedIndex& idx, const std::vector<Tok>& rpn) {
    SearchResult res;
    std::vector<std::vector<uint32_t>> st;

    for (const auto& t : rpn) {
        if (t.type == TokType::TERM) {
            st.push_back(idx.postings_for(t.text));
        } else if (t.type == TokType::NOT) {
            if (st.empty()) { 
                res.error = "NOT expects an operand"; 
                return res; 
            }

            auto a = std::move(st.back()); st.pop_back();
            st.push_back(diff_merge(idx.all_docs(), a));
        } else {
            if (st.size() < 2) { 
                res.error = "Operator expects two operands"; 
                return res; 
            }

            auto b = std::move(st.back()); st.pop_back();
            auto a = std::move(st.back()); st.pop_back();

            if (t.type == TokType::AND) { 
                st.push_back(and_merge(a, b));
            } else if (t.type == TokType::OR) {
                st.push_back(or_merge(a, b));
            } else { 
                res.error = "Unknown operator"; 
                return res; 
            }
        }
    }

    if (st.size() != 1) { 
        res.error = "Invalid query"; 
        return res; 
    }

    res.doc_ids = std::move(st.back());

    return res;
}

static std::vector<Tok> shunting_yard(const std::vector<Tok>& toks, std::string& err) {
    std::vector<Tok> output;
    std::vector<Tok> ops;

    for (const auto& t : toks) {
        if (t.type == TokType::TERM) {
            output.push_back(t);
        } else if (t.type == TokType::LPAREN) {
            ops.push_back(t);
        } else if (t.type == TokType::RPAREN) {
            bool found = false;
            while (!ops.empty()) {
                Tok top = ops.back(); ops.pop_back();

                if (top.type == TokType::LPAREN) { 
                    found = true; 
                    break;
                }

                output.push_back(top);
            }
            
            if (!found) { 
                err = "Mismatched parentheses"; 
                return {}; 
            }
        } else { // operator
            while (!ops.empty() && is_op(ops.back()) &&
                   ((prec(ops.back().type) > prec(t.type)) ||
                    (prec(ops.back().type) == prec(t.type) && t.type != TokType::NOT))) {
                output.push_back(ops.back());
                ops.pop_back();
            }
            ops.push_back(t);
        }
    }
    while (!ops.empty()) {
        if (ops.back().type == TokType::LPAREN) { 
            err = "Mismatched parentheses"; 
            return {}; 
        }

        output.push_back(ops.back());
        ops.pop_back();
    }

    return output;
}

SearchResult boolean_search(const InvertedIndex& idx, const std::string& query_utf8) {
    SearchResult res;
    auto toks = lex_query(query_utf8);
    toks = insert_implicit_and(std::move(toks));
    std::string err;
    auto rpn = shunting_yard(toks, err);

    if (!err.empty()) { 
        res.error = err; 
        return res; 
    }

    return eval_rpn(idx, rpn);
}

} // namespace ir
