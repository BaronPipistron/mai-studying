#include "zipf.hpp"
#include <fstream>
#include <vector>
#include <algorithm>

namespace ir {

struct Row {
    uint32_t term_id;
    uint64_t freq;
};

bool write_zipf_csv(const InvertedIndex& idx, const std::string& path) {
    if (idx.term_count() == 0) {
        return false;
    }

    std::vector<Row> rows;
    rows.reserve(idx.term_count());
    for (uint32_t tid = 0; tid < idx.term_count(); ++tid) {
        rows.push_back({tid, idx.term_freq(tid)});
    }

    std::sort(rows.begin(), rows.end(), [](const Row& a, const Row& b) {
        return a.freq > b.freq;
    });

    uint64_t C = rows.front().freq ? rows.front().freq : 1;

    std::ofstream f(path);

    if (!f) { 
        return false;
    }

    f << "rank,term,freq,zipf_pred\n";

    for (size_t i = 0; i < rows.size(); ++i) {
        uint64_t rank = static_cast<uint64_t>(i + 1);
        double pred = static_cast<double>(C) / static_cast<double>(rank);
        f << rank << ",";

        const std::string& term = idx.term_text(rows[i].term_id);
        f << "\"";

        for (char c : term) {
            if (c == '"') f << "\"\"";
            else f << c;
        }

        f << "\"";
        f << "," << rows[i].freq << "," << pred << "\n";
    }
    
    return true;
}

} // namespace ir
