#pragma once
#include <string>
#include <vector>

namespace ir {

inline std::string get_arg(const std::vector<std::string>& args, const std::string& key, const std::string& def = "") {
    for (size_t i = 0; i + 1 < args.size(); ++i) {
        if (args[i] == key) return args[i + 1];
    }

    return def;
}

inline bool has_flag(const std::vector<std::string>& args, const std::string& key) {
    for (const auto& a : args) if (a == key) return true;

    return false;
}

inline int64_t get_arg_i64(const std::vector<std::string>& args, const std::string& key, int64_t def = 0) {
    std::string v = get_arg(args, key, "");
    
    if (v.empty()) return def;

    try { 
        return std::stoll(v); 
    } catch (...) {
        return def; 
    }
}

} // namespace ir
