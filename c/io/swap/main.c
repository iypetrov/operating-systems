#include <stdio.h>
#include <string.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int is_reg_file(int fd);

int is_reg_file(int fd) {
  if (!S_ISREG(fd)) {
    return 0;
  }

  return 1;
}

int main(int argc, char* argv []) {
    if (argc != 3) {
      errx(1, "provide 2 args");
    }

    // loading data
    int fdr1 = open(argv[1], O_RDONLY);
    if (fdr1 < 0) {
      err(1, "failed to open file for reading");
    }

    if (is_reg_file(fdr1)) {
      err(1, "first arg is not a file");
    }
    
    char buf1[4096];
    int offsetr1 = read(fdr1, &buf1, 4096 * sizeof(char));
    if (offsetr1 < 0) {
      err(1, "failed to read from file 1");
    }
    int len1 = strlen(buf1);
    dprintf(1, "%d\n", len1);
    buf1[len1] = '\0';

    int fdr2 = open(argv[2], O_RDONLY);
    if (fdr2 < 0) {
      err(1, "failed to open file for reading");
    }

    if (is_reg_file(fdr2)) {
      err(1, "second arg is not a file");
    }

    char buf2[4096];
    int offsetr2= read(fdr2, &buf2, 4096 * sizeof(char));
    if (offsetr2 < 0) {
      err(1, "failed to read from file 2");
    }
    int len2 = strlen(buf2);
    dprintf(1, "%d\n", len2);
    buf2[len2] = '\0';

    close(fdr1);
    close(fdr2);

    // swaping data
    int fdw1 = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fdw1 < 0) {
      err(1, "failed to open file for writing");
    }

    if (write(fdw1, buf2, len1 * sizeof(char)) < 0){
      err(1, "failed to write");
    }

    int fdw2 = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fdw2 < 0) {
      err(1, "failed to open file for writing");
    }

    if (write(fdw2, buf1, len2 * sizeof(char)) < 0){
      err(1, "failed to write");
    }

    close(fdw1);
    close(fdw2);

    return 0;
}

