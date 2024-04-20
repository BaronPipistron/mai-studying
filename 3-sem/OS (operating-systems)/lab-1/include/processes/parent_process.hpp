#pragma once

#include <iostream>
#include <string>

namespace processes {

class ParentProcess final {
public:
    static void parent_process_handler();

private:
    [[nodiscard]] static std::string get_file_name();
};

}; // namespace processes