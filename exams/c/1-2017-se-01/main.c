#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>

typedef struct __attribute__((packed)) {
  uint16_t offset;
  uint8_t origin;
  uint8_t new_byte;
} diff;

off_t get_file_size(int fd); 
off_t get_file_size(int fd) {
  struct stat info;
  if (fstat(fd, &info) == -1) err(1, "failed to stat");
  return info.st_size;
}

int main(int argc, char* argv[]) {
    if (argc != 4) errx(1, "provide 3 args");

    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1) err(1, "failed to open file");

    int fd2 = open(argv[2], O_RDONLY);
    if (fd2 == -1) err(1, "failed to open file");
  
    int fd3 = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd3 == -1) err(1, "failed to open file");

    off_t f1_size = get_file_size(fd1);
    off_t f2_size = get_file_size(fd2);

    if (f1_size != f2_size) errx(1, "both files are not with the same size");
    if(f1_size > UINT16_MAX) errx(1, "files too big");

    uint8_t info1;
    uint8_t info2;
    ssize_t offset1;
    uint16_t index = 0;
    while ((offset1 = read(fd1, &info1, sizeof(uint8_t))) > 0) {
      if (read(fd2, &info2, sizeof(uint8_t) == -1) err(1, "failed to read file");
      if (info1 != info2) {
        diff d;
        d.offset = index;
        d.origin = info1;
        d.new_byte = info2;
        if (write(fd3, &d, sizeof(diff)) == -1) err(1, "failed to write fiile");
      }  

      index += offset1;
    }
    if (offset1 == -1) err(1, "failed to read file");

    close(fd1);
    close(fd2);
    close(fd3);

    return 0;
}

