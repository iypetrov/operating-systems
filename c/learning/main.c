#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    const char* file = "text.txt";
    int fd = open(
        file,
        O_WRONLY|O_CREAT|O_APPEND,
        0644
    );

    if (fd < 0) {
        err(1, "failed to open a fd");
        return 99;
    }

    const char* msg = "Hello World!\n";
    int offset = write(fd, msg, strlen(msg));

    if (offset < 0) {
        err(1, "failed to write in the file");
        return 99;
    }

    if (close(fd) < 0) {
        err(1, "failed to close a fd %d", fd);
        return 99;
    }

    return 0;
}
