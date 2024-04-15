#include <stdio.h>
#include <fcntl.h>
#include <err.h>
#include <sys/stat.h>

int main(void) {
     struct stat info;
     const char *file = "text.txt";

    if (stat(file, &info) == -1) {
        err(1, "can not stat file");
        return 99;
    }

    int result = open(file, O_RDONLY);
    if (result < 0) {
        err(1, "could not open file");
        return 99;
    }

    if (info.st_size == 0) {
        warn("file is empty");
        return 99;
    }
}
