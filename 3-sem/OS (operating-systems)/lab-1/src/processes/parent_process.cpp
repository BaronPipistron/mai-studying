#include "../../include/processes/parent_process.hpp"
#include "../../include/calls/calls.hpp"

#define CHILD_NAME "./bin/run_child"

using namespace processes;
using namespace calls;

void ParentProcess::parent_process_handler() {
    std::cout << "Parent process with pid " << getpid() << " started" << std::endl; 

    std::string file_name = get_file_name();

    int fd_1[2];
    int fd_2[2];

    create_pipe(fd_1);
    create_pipe(fd_2);

    int write_1 = fd_1[1], write_2 = fd_2[1];
    int read_1 = fd_1[0], read_2 = fd_2[0];

    pid_t pid = create_process();

    if (pid == 0) {
        closeFD(write_1);
        closeFD(read_2);

        create_dup2FD(read_1, STDIN_FILENO);
        
        closeFD(write_2);
        closeFD(read_1);
        
        run_process(CHILD_NAME, file_name.c_str());
    } else {
        closeFD(write_2);
        closeFD(read_1);

        std::cout << "Parent process with pid " << getpid() << std::endl;

        uint64_t num;
        while (std::cin >> num) {
            dprintf(write_1, "%ld ", num);
        }

        closeFD(write_1);
        closeFD(read_2);
    }

    return;
}

std::string ParentProcess::get_file_name() {
    std::string file_name;

    std::cout << "Input file name: ";
    std::cin >> file_name;

    if (file_name.length() > 255) throw std::invalid_argument("File name must be less than 256 symbols");

    for (char c: file_name) {
        if (c == '/' || c == '\\' || c == '?' || c == '<' || c == '>' || c == '*' || c == '|') {
            throw std::invalid_argument("File name can't contains /, \\, ?, <, >, *, |");
        }
    }
    file_name = "output_files/" + file_name + ".txt";
    
    return file_name;
}