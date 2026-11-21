
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {

    int fd[2];

    // Create a pipe
    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    // Create child process
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        close(fd[0]);
        close(fd[1]);
        return 1;
    }

    // Child process
    if (pid == 0) {

        close(fd[1]);  // Close write end

        char buffer[128] = {0};

        ssize_t n = read(fd[0], buffer, sizeof(buffer) - 1);

        if (n == -1) {
            perror("read");
            close(fd[0]);
            return 1;
        }

        if (n > 0) {
            buffer[n] = '\0';
            printf("Child received: %s\n", buffer);
        }

        close(fd[0]);

        return 0;
    }

    // Parent process
    close(fd[0]);  // Close read end

    const char msg[] = "Hello from the parent";

    if (write(fd[1], msg, strlen(msg)) == -1) {
        perror("write");
        close(fd[1]);
        waitpid(pid, NULL, 0);
        return 1;
    }

    close(fd[1]);

    waitpid(pid, NULL, 0);

    return 0;
}
