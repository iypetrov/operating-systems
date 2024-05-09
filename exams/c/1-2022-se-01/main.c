#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>

typedef struct __attribute__((packed)) {
  uint16_t type;
  uint16_t reserved1;
  uint16_t reserved2;
  uint16_t reserved3;
  uint32_t offset1;
  uint32_t offset2;
} offset;

int main(int argc, char* argv[]) {
  if (argc != 3) {
    errx(1, "shuld provide 2 args");
  }

  int data = open(argv[1], O_RDONLY);
  int comp = open(argv[2], O_RDONLY);

  // data
  uint32_t magic_data;
  read(data, &magic_data, sizeof(uint32_t)); 
  if (magic_data != 0x21796F4A) {
    errx(1, "wrong magic value");
  }
  
  uint32_t count_data;
  read(data, &count_data, sizeof(uint32_t)); 

  uint64_t arr[count_data];
  read(data, &arr, count_data * sizeof(uint64_t)); 

  // comp 
  uint32_t magic_comp_1;
  read(comp, &magic_comp_1, sizeof(uint32_t)); 
  if (magic_comp_1 != 0xAFBC7A37) {
    errx(1, "wrong magic value");
  }

  uint16_t magic_comp_2;
  read(comp, &magic_comp_2, sizeof(uint16_t)); 
  if (magic_comp_2 != 0x1C27) {
    errx(1, "wrong magic value");
  }

  uint16_t reserved;
  read(comp, &reserved, sizeof(uint16_t)); 

  uint64_t count_comp;
  read(comp, &count_comp, sizeof(uint64_t)); 

  offset offsets[count_comp];
  read(comp, &offsets, count_comp * sizeof(offset)); 
  for (uint64_t i = 0; i < count_comp; i++) {
    if (offsets[i].type == 0) {
      if (arr[offsets[i].offset1] > arr[offsets[i].offset2]) {
        uint64_t tmp = arr[offsets[i].offset1];
        arr[offsets[i].offset1] = arr[offsets[i].offset2];
        arr[offsets[i].offset2] = tmp;
      }
    }
    if (offsets[i].type == 1) {
      if (arr[offsets[i].offset1] < arr[offsets[i].offset2]) {
        uint64_t tmp = arr[offsets[i].offset1];
        arr[offsets[i].offset1] = arr[offsets[i].offset2];
        arr[offsets[i].offset2] = tmp;
      }
    }
  }

  close(data);
  close(comp);

  int data_new = open(argv[1], O_WRONLY | O_TRUNC | O_TRUNC, 0644);

  write(data_new, &magic_data, sizeof(uint32_t));
  write(data_new, &count_data, sizeof(uint32_t));
  for (uint32_t i = 0; i < count_data; i++) {
    write(data_new, &arr[i], sizeof(uint64_t));
  }

  close(data_new);

  return 0;
}

