#include "io.hpp"
#include <fstream>
#include <cstring>

namespace ir {

static void write_u32(std::ofstream& f, uint32_t v) { 
    f.write(reinterpret_cast<const char*>(&v), sizeof(v)); 
}

static void write_u64(std::ofstream& f, uint64_t v) { 
    f.write(reinterpret_cast<const char*>(&v), sizeof(v)); 
}

static void write_i64(std::ofstream& f, int64_t v) { 
    f.write(reinterpret_cast<const char*>(&v), sizeof(v)); 
}

static bool read_u32(std::ifstream& f, uint32_t& v) { 
    return static_cast<bool>(f.read(reinterpret_cast<char*>(&v), sizeof(v))); 
}

static bool read_u64(std::ifstream& f, uint64_t& v) { 
    return static_cast<bool>(f.read(reinterpret_cast<char*>(&v), sizeof(v))); 
}

static bool read_i64(std::ifstream& f, int64_t& v) { 
    return static_cast<bool>(f.read(reinterpret_cast<char*>(&v), sizeof(v))); 
}

static void write_str(std::ofstream& f, const std::string& s) {
    write_u32(f, static_cast<uint32_t>(s.size()));
    f.write(s.data(), static_cast<std::streamsize>(s.size()));
}

static bool read_str(std::ifstream& f, std::string& s) {
    uint32_t n = 0;
    
    if (!read_u32(f, n)) {
        return false;
    }
    s.assign(n, '\0');

    return static_cast<bool>(f.read(s.data(), static_cast<std::streamsize>(n)));
}

bool save_index(const InvertedIndex& idx, const std::string& path) {
    std::ofstream f(path, std::ios::binary);
    if (!f) {
        return false;
    }

    const char magic[8] = {'I','R','I','D','X','1',0,0};
    f.write(magic, sizeof(magic));
    write_u32(f, 1);

    // Stats
    write_u64(f, idx.stats_.tok.token_count);
    write_u64(f, idx.stats_.tok.total_token_chars);
    write_u64(f, idx.stats_.tok.input_bytes);
    write_u64(f, idx.stats_.elapsed_us);

    // Docs
    write_u32(f, idx.doc_count());
    for (uint32_t i = 0; i < idx.doc_count(); ++i) {
        const auto& d = idx.docs_.at(i);

        write_str(f, d.mongo_id);
        write_str(f, d.url);
        write_str(f, d.source_name);
        write_i64(f, d.crawl_date);
    }

    // Terms + postings + term freq
    write_u32(f, idx.term_count());
    for (uint32_t tid = 0; tid < idx.term_count(); ++tid) {
        write_str(f, idx.id_to_term_.at(tid));
        write_u64(f, idx.term_freq_.at(tid));

        const auto& post = idx.postings_.at(tid);
        write_u32(f, static_cast<uint32_t>(post.size()));

        for (uint32_t doc_id : post) {
            write_u32(f, doc_id);
        }
    }

    return static_cast<bool>(f);
}

bool load_index(InvertedIndex& idx, const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f) {
        return false;
    }

    char magic[8]{0};
    f.read(magic, sizeof(magic));
    if (std::memcmp(magic, "IRIDX1", 5) != 0) {
        return false;
    }

    uint32_t version = 0;
    if (!read_u32(f, version)) return false;
    if (version != 1) return false;

    // Stats
    if (!read_u64(f, idx.stats_.tok.token_count)) return false;
    if (!read_u64(f, idx.stats_.tok.total_token_chars)) return false;
    if (!read_u64(f, idx.stats_.tok.input_bytes)) return false;
    if (!read_u64(f, idx.stats_.elapsed_us)) return false;

    // Docs
    uint32_t doc_count = 0;
    if (!read_u32(f, doc_count)) {
        return false;
    }

    idx.docs_.clear();
    idx.docs_.reserve(doc_count);
    for (uint32_t i = 0; i < doc_count; ++i) {
        DocumentMeta d;

        if (!read_str(f, d.mongo_id)) return false;
        if (!read_str(f, d.url)) return false;
        if (!read_str(f, d.source_name)) return false;
        if (!read_i64(f, d.crawl_date)) return false;

        idx.docs_.push_back(std::move(d));
    }

    // Terms
    uint32_t term_count = 0;
    if (!read_u32(f, term_count)) {
        return false;
    }

    idx.term_to_id_ = HashMap<std::string, uint32_t>();
    idx.id_to_term_.clear();
    idx.postings_.clear();
    idx.term_freq_.clear();

    idx.id_to_term_.reserve(term_count);
    idx.postings_.reserve(term_count);
    idx.term_freq_.reserve(term_count);

    for (uint32_t tid = 0; tid < term_count; ++tid) {
        std::string term;
        uint64_t freq = 0;

        if (!read_str(f, term)) return false;
        if (!read_u64(f, freq)) return false;

        idx.term_to_id_.put(term, tid);
        idx.id_to_term_.push_back(term);
        idx.term_freq_.push_back(freq);

        uint32_t pc = 0;
        if (!read_u32(f, pc)) {
            return false;
        }

        std::vector<uint32_t> post;
        post.resize(pc);
        for (uint32_t j = 0; j < pc; ++j) {
            uint32_t did = 0;

            if (!read_u32(f, did)) {
                return false;
            }

            post[j] = did;
        }

        idx.postings_.push_back(std::move(post));
    }

    idx.finalize();
    
    return true;
}

} // namespace ir
