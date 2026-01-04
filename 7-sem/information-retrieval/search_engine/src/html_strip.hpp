#pragma once
#include <string>

namespace ir {

// Removes tags, collapses whitespace, decodes a few basic entities.
std::string html_to_text(const std::string& html);

} // namespace ir
