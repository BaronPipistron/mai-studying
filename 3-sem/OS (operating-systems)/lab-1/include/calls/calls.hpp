#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <string>

namespace calls {

[[nodiscard]] pid_t create_process();
[[nodiscard]] int open_file(const char*);

void create_dup2FD(const int, const int);
void closeFD(const int);
void create_pipe(int*);
void run_process(const char*, const char*);

}; // namespace calls