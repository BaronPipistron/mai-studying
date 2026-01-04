#pragma once
#include <string>
#include "index.hpp"

namespace ir {

// Writes CSV: rank,term,freq,zipf_pred
bool write_zipf_csv(const InvertedIndex& idx, const std::string& path);

} // namespace ir
