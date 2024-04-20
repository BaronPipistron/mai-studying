#include <fcntl.h>
#include <iostream>
#include <semaphore.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

int main(int argc, char** argv) {
    std::cout << "Child process started" << std::endl;

    FILE* f = fopen("output_files/output.txt", "w+");
    fprintf(f, "The sums are: ");

    int memoryd;
    memoryd = open("memory_files/memory.txt", O_RDWR, 0666);
    char* buffer = (char*)mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, memoryd, 0);
    close(memoryd);

    sem_t* sem = sem_open("mmap_sem11", O_CREAT, S_IRWXU, 0);  
    sem_t* sem2 = sem_open("mmap_sem22", O_CREAT, S_IRWXU, 1); 

    if (sem == SEM_FAILED) {
        perror("Could not open semaphore");
        return -1;
    }

    int num = 0, sum = 0;
    size_t i = 0;

    sem_wait(sem);  
    while (buffer[i] != -1) {
        if (buffer[i] != ' ' && buffer[i] != '\n') {
            num *= 10;
            num += buffer[i] - '0';
        } else if (buffer[i] == ' ') {
            sum += num;
            num = 0;
        } else if (buffer[i] == '\n') {
            if (num != 0)
                sum += num;
            fprintf(f, "%d ", sum);
            sum = 0;
            num = 0;
        }
        ++i;
    }
    sem_post(sem2);

    sem_close(sem);
    sem_close(sem2);

    munmap(buffer, 1024);

    return 0;
}
