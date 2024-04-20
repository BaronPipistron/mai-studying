#pragma once

#include <iostream>

namespace processes {

class ChildProcess final {
public:
    static void child_process_handler(const char*);
};

}; // namespace processes