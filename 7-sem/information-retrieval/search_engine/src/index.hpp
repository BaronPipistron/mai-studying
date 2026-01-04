#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "containers/hash_map.hpp"
#include "tokenizer.hpp"

namespace ir {

struct DocumentMeta {
    std::string mongo_id;    // ObjectId hex string (or empty if not available)
    std::string url;
    std::string source_name;
    int64_t crawl_date = 0;  // unix timestamp
};

struct BuildStats {
    TokenizationStats tok{};
    uint64_t elapsed_us = 0;
};

class InvertedIndex {
public:
    void add_document(const DocumentMeta& meta, const std::string& raw_html);

    void finalize();

    // Query helpers
    const std::vector<uint32_t>& postings_for(const std::string& normalized_term) const;
    bool has_term(const std::string& normalized_term) const;

    const std::vector<uint32_t>& all_docs() const { 
        return all_docs_; 
    }

    const DocumentMeta& doc(uint32_t doc_id) const { 
        return docs_.at(doc_id); 
    }

    uint32_t doc_count() const { 
        return static_cast<uint32_t>(docs_.size()); 
    }

    uint32_t term_count() const { 
        return static_cast<uint32_t>(id_to_term_.size()); 
    }

    const std::string& term_text(uint32_t term_id) const { 
        return id_to_term_.at(term_id); 
    }

    uint64_t term_freq(uint32_t term_id) const {
        return term_freq_.at(term_id); 
    }

    const BuildStats& stats() const { 
        return stats_; 
    }

    BuildStats& stats_mut() { 
        return stats_; 
    }

    friend bool save_index(const InvertedIndex& idx, const std::string& path);
    friend bool load_index(InvertedIndex& idx, const std::string& path);

private:
    uint32_t get_or_add_term_id(const std::string& term);

    HashMap<std::string, uint32_t> term_to_id_;
    std::vector<std::string> id_to_term_;
    std::vector<std::vector<uint32_t>> postings_;
    std::vector<uint64_t> term_freq_;

    std::vector<DocumentMeta> docs_;
    std::vector<uint32_t> all_docs_;

    BuildStats stats_{};

    static const std::vector<uint32_t>& empty_postings();
};

} // namespace ir
