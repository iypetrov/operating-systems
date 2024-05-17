#include<fcntl.h>
#include<unistd.h>
#include<err.h>
#include<stdio.h>
#include<stdint.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>

off_t get_file_size(int fd);
off_t get_file_size(int fd) {
  struct stat info;
  if (fstat(fd, &info) == -1) err(1, "failed to stat");
  return info.st_size;
}

int main(int argc, char* argv[]) {
  if (argc != 4) errx(1, "provide 3 args");
  char* word = argv[1];
  int dic = open(argv[2], O_RDONLY);
  if (dic == -1) err(1, "failed to open file");
  int idx = open(argv[3], O_RDONLY);
  if (idx == -1) err(1, "failed to open file");

  off_t idx_size = get_file_size(idx);
  if (idx_size % sizeof(uint32_t) != 0) errx(1, "idx file not in the correct format"); 

  int start = 0;
  int end = idx_size / sizeof(uint32_t);
  while (start <= end) {
    int mid = start + (end - start) / 2;
    if (lseek(idx, mid * sizeof(uint32_t), SEEK_SET) == -1) err(1, "failed to lseek");

    uint32_t idx_buf;
    if (read(idx, &idx_buf, sizeof(uint32_t)) == -1) err(1, "failed to read"); 
    if (lseek(dic, idx_buf + 1, SEEK_SET) == -1) err(1, "failed to lseek");
    
    char word_buf[63];
    if (read(dic, &word_buf, 63 * sizeof(char)) == -1) err(1, "failed to read");
    for (int i = 0; i < 63; i++) {
      if (word_buf[i] == '\n') {
        word_buf[i] = '\0';
        break;
      }
    }

    int result = strcmp(word, word_buf);
    if (result == 0) {
      if (lseek(dic, idx_buf + 1, SEEK_SET) == -1) err(1, "failed to lseek");

      char buf;
      ssize_t offset;
      while ((offset = read(dic, &buf, sizeof(char))) > 0) {
        if (buf == '\0') {
          dprintf(1, "\n");
          break;
        }
        dprintf(1, "%c", buf);
      }

      break;
    } else if (result > 0) {
      start = mid + 1;
    } else if (result < 0) {
      end = mid - 1;
    }
  }

  close(dic);
  close(idx);

  return 0;
}
