#include "../../include/calls/calls.hpp"

namespace calls {

pid_t create_process() {
    pid_t pid = fork();

    if (pid == -1) throw std::runtime_error("Failed with creating child process");
    return pid;
}

int open_file(const char* file_name) {
    int file = open(file_name, O_CREAT | O_WRONLY | O_TRUNC);

    if (file == -1) throw std::runtime_error("Failed with opening file");
    return file;
}

void create_dup2FD(const int old_fd, const int new_fd) {
    int dup2_num = dup2(old_fd, new_fd);

    if (dup2_num == -1) throw std::runtime_error("Failed with creating dup2 to this file directories");
    return;
}

void closeFD(const int fd) {
    int closeFD_num = close(fd);

    if (closeFD_num == -1) throw std::runtime_error("Failed with closing fd");
    return;
}

void create_pipe(int* fd) {
    int pipe_num = pipe(fd);
    
    if (pipe_num == -1) throw std::runtime_error("Failed with creating pipe");
    return;
}

void run_process(const char* child_process, const char* file_name) {
    int execl_num = execl(child_process, child_process, file_name, NULL);

    if (execl_num == -1) throw std::runtime_error("Failed with run child process");
    return;
}

}; // namespace calls