#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

uint16_t pow(uint8_t base, uint8_t power) {
  uint16_t res = 1;

  if (power == 0) {
    return 1; 
  }

  for (uint8_t i = 0; i < power; i++) {
    res *= base;
  }
  return res;
}

uint8_t mod(uint8_t a, uint8_t b) {
  if (a > b) {
    return a - b;
  }
  
  if (a < b) {
    return b - a;
  }

  return 0;
}

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

  uint8_t buf;
  uint16_t res;
  ssize_t offset;
  while ((offset = read(src, &buf, sizeof(uint8_t))) > 0) {
    res = 0;
    for (int i = 7; i >= 0; i--) {
      uint8_t bit = (buf >> i) & 1; 
      res <<= 2;
      res |= (bit ? 2 : 1);
    }
    
    if (write(dst, &res, sizeof(uint16_t)) < 0) {
      err(1, "failed to write");
    }
  }
  if (offset < 0) {
    err(1, "failed to read");
  }

  close(src);

  return 0;
}

