#include <stdio.h>
#include <string.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int is_reg_file(int fd);

int is_reg_file(int fd) {
  if (!S_ISREG(fd)) {
    return 0;
  }

  return 1;
}

int main(int argc, char* argv[]) {
    if (argc <= 2) {
      errx(1, "provide at least 2 args");
    }

    for (int i = 2; i < argc; i++) {
      int fd = open(argv[i], O_RDONLY);
      if ( fd == -1) {
        err(1, "failed to open a file\n");
      }

      dprintf(1, "file -> %s\n", argv[i]);
      if (is_reg_file(fd) != -1) {
        char buf;
        int offset;
        int cnt = 0; 
        if (!strcmp(argv[1], "lines")) {
          while ((offset = read(fd, &buf, sizeof(char))) > 0) {
            if (buf == '\n') {
              cnt++;
            }
          }
        } else if (!strcmp(argv[1], "bytes")) {
          while ((offset = read(fd, &buf, sizeof(char))) > 0) {
            cnt++;
          }
        } else {
          err(1, "1 arg shoudl be lines/bytes");
        }
        if (offset < 0) {
          err(1, "failed to read file");
        } 
        dprintf(1, "file %s has number of lines %d\n", argv[i], cnt);
      } else {
        warnx("arg on posiotion %d %s is not a file", i, argv[i]);
      }

      close(fd);
    }

    return 0;
}

