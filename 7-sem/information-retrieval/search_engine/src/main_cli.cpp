#include "args.hpp"
#include "boolean_query.hpp"
#include "io.hpp"
#include <iostream>
#include <sstream>

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
    std::string index_path = ir::get_arg(args, "--index", "index.bin");
    int64_t top = ir::get_arg_i64(args, "--top", 50);

    ir::InvertedIndex idx;
    if (!ir::load_index(idx, index_path)) {
        std::cerr << "Failed to load index: " << index_path << "\n";

        return 1;
    }

    std::ostringstream ss;
    ss << std::cin.rdbuf();
    std::string query = ss.str();

    if (query.empty()) {
        return 0;
    }

    auto res = ir::boolean_search(idx, query);
    if (!res.error.empty()) {
        std::cerr << "Query error: " << res.error << "\n";

        return 2;
    }

    int64_t printed = 0;
    for (uint32_t doc_id : res.doc_ids) {
        const auto& d = idx.doc(doc_id);
        std::cout << doc_id << "\t" << d.url << "\n";

        if (top > 0 && ++printed >= top) {
            break;
        }
    }

    return 0;
}
