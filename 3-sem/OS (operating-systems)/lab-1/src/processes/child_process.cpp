#include "../../include/processes/child_process.hpp"
#include "../../include/calls/calls.hpp"

using namespace processes;
using namespace calls;

void ChildProcess::child_process_handler(const char* file_name) {
    std::cout << "Child process with pid " << getpid() << " started\nInput numbers: " << std::endl;

    int file = open_file(file_name);

    create_dup2FD(file, STDOUT_FILENO);
    closeFD(file);

    int64_t sum { 0 };
    int64_t num;

    while (std::cin >> num) {
        sum += num;
    }
    std::cout << "Result sum: " << sum;

    return;
}

