#include "index.hpp"
#include "html_strip.hpp"
#include "stemmer.hpp"
#include <algorithm>

namespace ir {

static std::vector<uint32_t> g_empty_postings;

const std::vector<uint32_t>& InvertedIndex::empty_postings() {
    return g_empty_postings;
}

uint32_t InvertedIndex::get_or_add_term_id(const std::string& term) {
    if (auto* v = term_to_id_.find_ptr(term)) return *v;

    uint32_t id = static_cast<uint32_t>(id_to_term_.size());
    term_to_id_.put(term, id);
    id_to_term_.push_back(term);

    postings_.emplace_back();
    term_freq_.push_back(0);

    return id;
}

void InvertedIndex::add_document(const DocumentMeta& meta, const std::string& raw_html) {
    const uint32_t doc_id = static_cast<uint32_t>(docs_.size());
    docs_.push_back(meta);

    std::string text = html_to_text(raw_html);
    auto tokens = tokenize(text, &stats_.tok);

    std::vector<uint32_t> term_ids;
    term_ids.reserve(tokens.size());

    for (auto& t : tokens) {
        std::string s = stem(t);
        if (s.empty()) continue;

        uint32_t tid = get_or_add_term_id(s);
        term_freq_[tid] += 1;
        term_ids.push_back(tid);
    }

    std::sort(term_ids.begin(), term_ids.end());
    term_ids.erase(std::unique(term_ids.begin(), term_ids.end()), term_ids.end());

    for (uint32_t tid : term_ids) {
        postings_[tid].push_back(doc_id);
    }
}

void InvertedIndex::finalize() {
    all_docs_.resize(docs_.size());

    for (uint32_t i = 0; i < docs_.size(); ++i) {
        all_docs_[i] = i;
    }
}

bool InvertedIndex::has_term(const std::string& normalized_term) const {
    return term_to_id_.contains(normalized_term);
}

const std::vector<uint32_t>& InvertedIndex::postings_for(const std::string& normalized_term) const {
    const uint32_t* tid = term_to_id_.find_ptr(normalized_term);
    
    if (!tid) { 
        return empty_postings();
    }
    
    return postings_.at(*tid);
}

} // namespace ir
