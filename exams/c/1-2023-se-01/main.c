#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
  if (argc != 3) errx(1, "provide 2 args"); 

  int in = open(argv[1], O_RDONLY);
  if (in == -1) err(1, "failed to open");

  int out = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
  if (out == -1) err(1, "failed to open");
  
  uint8_t buf;
  ssize_t offset;
  while ((offset = read(in, &buf, sizeof(uint8_t))) > 0) {
    if (buf == 0x55) {
      off_t curr = lseek(in, 0, SEEK_CUR);

      uint8_t n;
      if (read(in, &n, sizeof(uint8_t)) == -1) err(1, "failed to read");

      uint8_t chk = 0x55 ^ n;

      uint8_t arr[n - 4];
      for (int i = 0; i < n - 3; i++) {
        uint8_t tmp;
        if (read(in, &tmp, sizeof(uint8_t)) == -1) err(1, "failed to read");
        arr[i] = tmp;
        chk ^= tmp;
      }

      uint8_t goal;
      if (read(in, &goal, sizeof(uint8_t)) == -1) err(1, "failed to read");

      if (goal == chk) {
        if (write(out, &buf, sizeof(uint8_t)) == -1) err(1, "failed to write");
        if (write(out, &n, sizeof(uint8_t)) == -1) err(1, "failed to write");
        if (write(out, &arr, (n - 4) * sizeof(uint8_t)) == -1) err(1, "failed to write");
      } 

      if (curr == -1) err(1, "failed to lseek");
      if (lseek(in, curr, SEEK_SET) == -1) err(1, "failed to lseek");
    } 
  }
  if (offset == -1) err(1, "failed to write");

  close(in);
  close(out);

  return 0;
}

