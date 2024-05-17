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

  uint16_t buf;
  ssize_t offset;
  while ((offset = read(in, &buf, sizeof(uint16_t))) > 0) {
    uint8_t result = 0;
    for (int i = 0; i < 16; i+=2) {
      int bit1 = (buf >> i) & 1;
      int bit2 = (buf >> (i + 1)) & 1;
      if (bit1 + bit2 != 1) errx(1, "file not in correct state");

      int index = 0;
      if (i < 8) index = i / 2 + 4;
      else index = i / 2 - 4;

      if (bit1 == 0) {
        result |= 1 << index;
      }
    }
    if (write(out, &result, sizeof(uint8_t)) == -1) err(1, "failed to write");
  }
  if (offset == -1) err(1, "failed to read");

  close(in);
  close(out);

  return 0;
}
