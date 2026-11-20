
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define SHM_NAME "/mission6_shared_memory"
#define SHM_SIZE 128

int main(void) {

    // Step 1: Create shared memory
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0600);

    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    // Step 2: Set shared memory size
    if (ftruncate(fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        close(fd);
        shm_unlink(SHM_NAME);
        return 1;
    }

    // Step 3: Map shared memory
    char *shared_data = mmap(
        NULL,
        SHM_SIZE,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        0
    );

    if (shared_data == MAP_FAILED) {
        perror("mmap");
        close(fd);
        shm_unlink(SHM_NAME);
        return 1;
    }

    close(fd);

    // Step 4: Parent writes into shared memory
    const char msg[] = "Hello through shared memory";
    strcpy(shared_data, msg);

    printf("Parent sent: %s\n", shared_data);
    fflush(stdout);

    // Step 5: Create child process
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        munmap(shared_data, SHM_SIZE);
        shm_unlink(SHM_NAME);
        return 1;
    }

    if (pid == 0) {

        // Child reads the shared memory
        printf("Child received: %s\n", shared_data);

        munmap(shared_data, SHM_SIZE);
        return 0;
    }

    waitpid(pid, NULL, 0);

    // Step 6: Cleanup
    munmap(shared_data, SHM_SIZE);
    shm_unlink(SHM_NAME);

    return 0;
}
  
