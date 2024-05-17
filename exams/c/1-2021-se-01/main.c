#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>

off_t get_file_size(int fd);

off_t get_file_size(int fd) {
  struct stat info;
  if (fstat(fd, &info) == -1) err(1, "failed to fstat");
  return info.st_size;
}

int main(int argc, char* argv[]) {
  if (argc != 3) errx(1, "provide 2 args");

  int in = open(argv[1], O_RDONLY);
  if (in == -1) err(1, "failed to open"); 
  int out = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
  if (out == -1) err(1, "failed to open"); 

  off_t in_size = get_file_size(in);
  uint8_t buf[in_size];
  if(read(in, &buf, in_size) == -1) err(1, "failed to read"); 

  off_t counter = 0;
  for (int i = 0; i < in_size; i++) {
    uint8_t result1 = 0;
    uint8_t result2 = 0;
    for (int j = 0; j < 4; j++) {
      if (counter == in_size * 8) break;

      int bit = (buf[i] >> j) & 1;
      if (bit) { 
        result1 |= 1 << (2 * j + 1);
      } else {
        result1 |= 1 << (2 * j);
      }

      counter++;
    }
    for (int j = 4; j < 8; j++) {
      if (counter == in_size * 8) break;

      int bit = (buf[i] >> j) & 1;
      if (bit)  {
        result2 |= 1 << (2 * (j - 4) + 1);
      } else {
        result2 |= 1 << (2 * (j - 4));
      }

      counter++;
    }

    if (write(out, &result2, sizeof(uint8_t)) == -1) err(1, "failed to write"); 
    if (write(out, &result1, sizeof(uint8_t)) == -1) err(1, "failed to write"); 
  }

  close(in);
  close(out);

  return 0;
}
