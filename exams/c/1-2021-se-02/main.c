#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
  int src = open(argv[1], O_RDONLY);

  uint16_t num;
  read(src, &num, sizeof(uint16_t));

  for (int i = 15; i >= 0; i-=2) {
    int bit1 = (num >> i) & 1;
    int bit2 = (num >> i - 1) & 1;

    if (!bit1 && bit2) {
      dprintf(1, "%d", 1);
    } else if (bit1 && !bit2) {
      dprintf(1, "%d", 0);
    } 
  }
  dprintf(1, "\n");

  close(src);

  return 0;
}

