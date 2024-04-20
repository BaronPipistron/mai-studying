#include "../include/processes/child_process.hpp"

int main(int argc, char *argv[]) {
    processes::ChildProcess::child_process_handler(argv[1]);

    return 0;
}