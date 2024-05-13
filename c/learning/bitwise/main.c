#include <stdio.h>
#include <stdint.h>

int main(void) {
  uint32_t number = 69420;

  for (int8_t i = 31; i >= 0; i--) {
    uint8_t bit = ( number >> i) & 1;
    dprintf(1, "%d", bit);
  }
  dprintf(1, "\n");

  return 0;
}

