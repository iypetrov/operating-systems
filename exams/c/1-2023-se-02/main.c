#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct __attribute__((packed)) {
  uint32_t curr;
  uint32_t next;
} positions;

void read_word(int dic, char word[64]) {
  ssize_t offset_dic = read(dic, word, 64 * sizeof(char));
  for (size_t i = 0; i < 64; i++) {
    if (word[i] == '\n') {
      word[i] = '\0';
    }
  }
}

positions find_word_index(char* search, int dic, int idx) {
  positions i;
  bool is_last= false;
  uint32_t curr;
  ssize_t offset_idx;
  while((offset_idx = read(idx, &curr, sizeof(uint32_t))) > 0) {
    lseek(dic, curr + 1, SEEK_SET);
    char word[64];

    read_word(dic, &word);

    if (is_last) {
      i.next = curr;
      break;
    }
    if (!strcmp(search, word)) {
      i.curr = curr;
      is_last = true;
    }

    curr += offset_idx;
  }

  return i;
}

int main(int argc, char* argv[]) {
    char* search = argv[1];
    int dic = open(argv[2], O_RDONLY);
    int idx = open(argv[3], O_RDONLY);

    positions pos = find_word_index(search, dic, idx);
    uint32_t curr = pos.curr;
    uint32_t next = pos.next;
    lseek(dic, curr + 1, SEEK_SET);
   
    // word
    char word[64];
    read_word(dic, &word);
    dprintf(1, "%s", word);

    // descr
    ssize_t offset_dic;
    size_t word_len = strlen(word);
    uint32_t cnt = curr + word_len + 1;
    lseek(dic, cnt, SEEK_SET);

    char buf;
    while((offset_dic = read(dic, &buf, sizeof(char))) > 0) {
      if (cnt == next) {
        break;
      }
      
      dprintf(1, "%c", buf);
      cnt++;
    }
    dprintf(1, "\n");

    close(dic);
    close(idx);

    return 0;
}

