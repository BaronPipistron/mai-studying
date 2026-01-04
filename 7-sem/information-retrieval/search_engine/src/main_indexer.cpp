#include "args.hpp"
#include "mongo_reader.hpp"
#include "io.hpp"
#include "zipf.hpp"
#include <chrono>
#include <iostream>

static std::vector<std::string> collect_args(int argc, char** argv) {
    std::vector<std::string> a;
    a.reserve(argc);

    for (int i = 1; i < argc; ++i) {
        a.emplace_back(argv[i]);
    }

    return a;
}

int main(int argc, char** argv) {
    auto args = collect_args(argc, argv);

    ir::MongoConfig cfg;
    cfg.uri = ir::get_arg(args, "--mongo-uri", cfg.uri);
    cfg.db = ir::get_arg(args, "--db", cfg.db);
    cfg.collection = ir::get_arg(args, "--collection", cfg.collection);
    cfg.limit = ir::get_arg_i64(args, "--limit", -1);

    std::string index_path = ir::get_arg(args, "--index", "index.bin");
    std::string zipf_path = ir::get_arg(args, "--zipf", "zipf.csv");

    ir::InvertedIndex idx;

    auto t0 = std::chrono::high_resolution_clock::now();
    try {
        ir::index_from_mongo(idx, cfg);
    } catch (const std::exception& e) {
        std::cerr << "Indexing failed: " << e.what() << "\n";

        return 1;
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    idx.stats_mut().elapsed_us = (uint64_t)std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

    idx.finalize();

    double seconds = idx.stats().elapsed_us / 1e6;
    double kb = idx.stats().tok.input_bytes / 1024.0;
    double kbps = seconds > 0 ? kb / seconds : 0.0;
    double avg_len = idx.stats().tok.token_count ? (double)idx.stats().tok.total_token_chars / (double)idx.stats().tok.token_count : 0.0;

    std::cout << "Docs: " << idx.doc_count() << "\n";
    std::cout << "Terms: " << idx.term_count() << "\n";
    std::cout << "Tokens: " << idx.stats().tok.token_count << "\n";
    std::cout << "Avg token length (codepoints): " << avg_len << "\n";
    std::cout << "Elapsed: " << seconds << " s\n";
    std::cout << "Tokenization speed: " << kbps << " KB/s\n";
    std::cout << "Tip: run indexer multiple times with different --limit to get time vs input size.\n";

    if (!ir::save_index(idx, index_path)) {
        std::cerr << "Failed to save index to " << index_path << "\n";

        return 2;
    }
    
    std::cout << "Saved index: " << index_path << "\n";

    if (!zipf_path.empty()) {
        if (!ir::write_zipf_csv(idx, zipf_path)) {
            std::cerr << "Failed to write zipf csv: " << zipf_path << "\n";
        } else {
            std::cout << "Saved Zipf CSV: " << zipf_path << "\n";
        }
    }

    return 0;
}
