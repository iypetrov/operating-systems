#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>

int main(int argc, char* argv[]) {
  if (argc != 3) {
    errx(1, "provide 2 args");
  }

  int src = open(argv[1], O_RDONLY); 
  if (src < 0) {
    err(1, "failed to open");
  }

  int dst = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644); 
  if (dst < 0) {
    err(1, "failed to open");
  }

  char start = 0x55;
  char buf;
  off_t offset;
  while ((offset = read(src, &buf, sizeof(char))) > 0) {
    if (buf == start) {
      uint8_t len;
      offset = read(src, &len, sizeof(uint8_t));
      if (offset < 0) {
        err(1, "failed to read");
      }

      char arr[len - 1]; 
      arr[0] = start;
      arr[1] = len;

      char msg[len - 3];
      offset = read(src, &msg, (len - 3) * sizeof(char)); 
      if (offset < 0) {
        err(1, "failed to read");
      }
      for (int i = 2; i < len - 1; i++) {
        arr[i] = msg[i - 2]; 
      }

      uint8_t checksum;
      offset = read(src, &checksum, sizeof(uint8_t));
      if (offset < 0) {
        err(1, "failed to read");
      }

      uint8_t arr_checksum = 0x55;
      for (int i = 1; i < len - 1; i++) {
        arr_checksum ^= arr[i];
      }

      if (checksum == arr_checksum) {
        ssize_t woffset;
        woffset = write(dst, &start, sizeof(char));
        if (woffset < 0) {
          err(1, "failed to write");
        }
        woffset = write(dst, &len, sizeof(uint8_t));
        if (woffset < 0) {
          err(1, "failed to write");
        }
        woffset = write(dst, &msg, (len - 3) * sizeof(char));
        if (woffset < 0) {
          err(1, "failed to write");
        }
        woffset = write(dst, &checksum, sizeof(uint8_t));
        if (woffset < 0) {
          err(1, "failed to write");
        }
      }
    }
  }
  if (offset < 0) {
    err(1, "failed to read");
  }

  close(src);
  close(dst);

  return 0;
}

