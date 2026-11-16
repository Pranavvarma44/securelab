
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    int fd = open("hello.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);

    if (fd == -1) {
        perror("open");
        return 1;
    }

    const char msg[] = "Hello from user space!\n";

    ssize_t n = write(fd, msg, sizeof(msg) - 1);

    if (n < 0) {
        perror("write");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}
