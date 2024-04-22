#include <stdio.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

const char* cp_cnt(int fdsrc, int fddest);

const char* cp_cnt(int fdsrc, int fddest) {
    char buf;
    int offsetsrc;
    while((offsetsrc = read(fdsrc, &buf, sizeof(buf))) > 0) {
      int offsetdest = write(fddest, &buf, sizeof(buf));
      if (offsetdest < 0) {
        return "failed to write dest";
      }
    }

    if (offsetsrc < 0) {
      return "failed to read src";
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
      errx(99, "provide 2 args");
      return 99;
    }

    struct stat info;

    char* src= argv[1];
    char* dest = argv[2];

    // check src if it is a reg file
    if (lstat(src, &info) == -1) {
      err(99, "couldn't get stats of 1 arg");
      return 99;
    }

    if (!S_ISREG(info.st_mode)) {
      err(99, "1 arg is not a reg file");
      return 99;
    }

    // open fds
    int fdsrc = open(src, O_RDONLY);
    if (fdsrc == -1) {
      err(99, "failed to open src for reading");
      return 99;
    } 

    int fddest = open(
      dest,
      O_WRONLY | O_CREAT | O_TRUNC,
      0644
    );
    if (fddest == -1) {
      err(99, "failed to open/create dest for writing");
      return 99;
    } 

    // core
    const char* er = cp_cnt(fdsrc, fddest);
    if (er != NULL) {
      err(99, "%s\n", er);
      return 99;
    }

    // close fds
    if (close(fdsrc) < 0) {
      err(99, "failed to close src");
      return 99;
    }

    if (close(fddest) < 0) {
      err(99, "failed to close dest");
      return 99;
    }

    return 0;
}

