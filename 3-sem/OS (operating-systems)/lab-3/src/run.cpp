#include <fcntl.h>
#include <iostream>
#include <semaphore.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int memoryd;
    memoryd = open("memory_files/memory.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
    ftruncate(memoryd, 1024);
    char* buffer = (char*)mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, memoryd, 0);
    close(memoryd);

    sem_t* _sem = sem_open("mmap_sem11", O_CREAT, S_IRWXU, 0);  
    sem_t* _sem2 = sem_open("mmap_sem22", O_CREAT, S_IRWXU, 1);  

    sem_close(_sem);
    sem_close(_sem2);
    sem_unlink("mmap_sem11");
    sem_unlink("mmap_sem22");
    
    sem_t* sem = sem_open("mmap_sem11", O_CREAT, S_IRWXU, 0);  
    sem_t* sem2 = sem_open("mmap_sem22", O_CREAT, S_IRWXU, 1);  

    if (sem == SEM_FAILED) {
        perror("Could not open semaphore");
        return -1;
    }

    pid_t id = fork();

    if (id == -1) {  
        return 2;
    } else if (id == 0) {  
        execl("build/src/_calculator_exe", "build/src/_calculator_exe", "mmap_sem", NULL);
        return 3;
    } else { 
        sem_wait(sem2);
        char c;
        c = getchar();
        size_t i = 0;

        while (c != EOF) {
            buffer[i++] = c;
            c = getchar();
        }
        buffer[i] = c;

        sem_post(sem);

        munmap(buffer, 1024);

        int status;
        waitpid(0, &status, 0); 

        if (status != 0)
            perror("Child process exited with an error");

        return status;
    }
}
