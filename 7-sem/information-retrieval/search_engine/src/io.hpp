#pragma once
#include <string>
#include "index.hpp"

namespace ir {

bool save_index(const InvertedIndex& idx, const std::string& path);
bool load_index(InvertedIndex& idx, const std::string& path);

} // namespace ir
