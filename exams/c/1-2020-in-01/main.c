#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

typedef struct __attribute__((packed)) {
  uint32_t magic; 
  uint8_t header_version; 
  uint8_t data_version; 
  uint16_t count; 
  uint32_t reserved1;
  uint32_t reserved2;
} patch_header;

typedef struct __attribute__((packed)) {
  uint16_t offset; 
  uint8_t original_word; 
  uint8_t new_word; 
} patch_data_v0;

typedef struct __attribute__((packed)) {
  uint32_t offset; 
  uint16_t original_word; 
  uint16_t new_word; 
} patch_data_v1;

int main(int argc, char* argv[]) {
  if (argc != 4) errx(1, "provide 3 args");

  int patch = open(argv[1], O_RDONLY);
  if (patch == -1) err(1, "failed open");
  int f1 = open(argv[2], O_RDONLY);
  if (f1 == -1) err(1, "failed open");
  int f2 = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
  if (f2 == -1) err(1, "failed open");

  // read patch 
  patch_header pheader;
  if(read(patch, &pheader, sizeof(patch_header)) == -1) err(1, "failed read"); 
  if (pheader.magic != 0xEFBEADDE) errx(1, "wrong fmt");
  if (pheader.header_version != 0x01) errx(1, "wrong fmt");
  if (pheader.data_version != 0x00 && pheader.data_version != 0x01) errx(1, "wrong fmt");

  uint8_t buf_word_v0;
  uint16_t buf_word_v1;
  if (pheader.data_version == 0x00) {
    patch_data_v0 buf_v0;
    for (int i = 0; i < pheader.count; i++) {
      // copy cnt of f1 to f2
      ssize_t rs;
      uint8_t buf;
      while ((rs = read(f1, &buf, sizeof(uint8_t))) > 0) {
        if (write(f2, &buf, sizeof(uint8_t)) == -1) err(1, "failed write");
      }
      if (rs == -1) err(1, "failed read");
      
      // apply patch
      if (read(patch, &buf_v0, sizeof(patch_data_v0)) == -1) err(1, "failed read");
      if (lseek(f2, buf_v0.offset, SEEK_SET) == -1) err(1, "failed lseek");
      if (read(f2, &buf_word_v0, sizeof(uint8_t)) == -1) err(1, "failed read");
      if (buf_word_v0 != buf_v0.original_word) errx(1, "wrong word");
      if (lseek(f2, buf_v0.offset, SEEK_SET) == -1) err(1, "failed lseek");
      if (write(f2, &buf_v0.new_word, sizeof(uint8_t)) == -1) err(1, "failed write");
    }
  } else if (pheader.data_version == 0x01) {
    patch_data_v1 buf_v1;
    for (int i = 0; i < pheader.count; i++) {
      // copy cnt of f1 to f2
      ssize_t rs;
      uint16_t buf;
      while ((rs = read(f1, &buf, sizeof(uint16_t))) > 0) {
        if (write(f2, &buf, sizeof(uint16_t)) == -1) err(1, "failed write");
      }
      if (rs == -1) err(1, "failed read");

      // apply patch
      if (read(patch, &buf_v1, sizeof(patch_data_v0)) == -1) err(1, "failed read");
      if (lseek(f2, buf_v1.offset, SEEK_SET) == -1) err(1, "failed lseek");
      if (read(f2, &buf_word_v1, sizeof(uint16_t)) == -1) err(1, "failed read");
      if (buf_word_v0 != buf_v1.original_word) errx(1, "wrong word");
      if (lseek(f2, buf_v1.offset, SEEK_SET) == -1) err(1, "failed lseek");
      if (write(f2, &buf_v1.new_word, sizeof(uint16_t)) == -1) err(1, "failed write");
    }
  }
  
  close(f2);
  close(f1);
  close(patch);
  return 0;
}
