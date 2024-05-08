#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <err.h>

typedef struct __attribute__((packed)) {
  uint32_t curr;
  uint32_t next;
} positions;

off_t get_size_file(int fd);
void read_word(int dic, char word[64]);
int fill_possitions(int dic, int idx, uint32_t arr_pos[], char* search, positions* p, int i, int lim);
void find_word(int start, int end, int dic, int idx, uint32_t arr_pos[], char* search, positions* p, int lim);
positions find_word_index(char* search, int dic, int idx); 

off_t get_size_file(int fd) {
  struct stat info;
  if (fstat(fd, &info) < 0) {
    err(1, "failed fstat");
  }
  return info.st_size;
}

void read_word(int dic, char word[64]) {
  ssize_t offset_dic = read(dic, word, 64 * sizeof(char));
  if (offset_dic < 0) {
    err(1, "failed read");
  }
  for (size_t i = 0; i < 64; i++) {
    if (word[i] == '\n') {
      word[i] = '\0';
    }
  }
}

int fill_possitions(int dic, int idx, uint32_t arr_pos[], char* search, positions* p, int i, int lim) {
    char word[64];
    if (lseek(dic, arr_pos[i] + 1, SEEK_SET) < 0) {
      err(1, "failed lseek");
    }
    read_word(dic, word);

    int res = strcmp(search, word);
    if (res == 0) {
      p->curr = arr_pos[i];
      p->next = get_size_file(idx);
      if (i != lim) {
        p->next = arr_pos[i + 1];
      }
    }

    return res;
}

void find_word(int start, int end, int dic, int idx, uint32_t arr_pos[], char* search, positions* p, int lim) {
      int mid = start + (end - start) / 2;
      int res = fill_possitions(dic, idx, arr_pos, search, p, mid, lim);

      if(res == 0) {
        return;
      } else if (start == end) {
        return;
      } else if (res < 0) {
        find_word(start, mid - 1, dic, idx, arr_pos, search, p, lim);
      } else if (res > 0){
        find_word(mid + 1, end, dic, idx, arr_pos, search, p, lim); 
      }
}

positions find_word_index(char* search, int dic, int idx) {
  positions p;
  p.curr = 0;
  p.next = 0;

  int indexes = get_size_file(idx) / sizeof(uint32_t);
  uint32_t arr_pos[indexes]; 

  ssize_t offset_idx;
  uint32_t curr;
  int cnt = 0;
  while((offset_idx = read(idx, &curr, sizeof(uint32_t))) > 0) {
    arr_pos[cnt] = curr;
    cnt++;
  }
  
  find_word(0, indexes - 1, dic, idx, arr_pos, search, &p, indexes - 1);

  return p;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
      errx(1, "provide 3 args");
    }

    char* search = argv[1];
    int dic = open(argv[2], O_RDONLY);
    if (dic < 0) {
      err(1, "failed open");
    }
    int idx = open(argv[3], O_RDONLY);
    if (idx < 0) {
      err(1, "failed open");
    }

    positions pos = find_word_index(search, dic, idx);
    if (pos.next == 0) {
      errx(1, "no seach word in the dic");
    }

    uint32_t curr = pos.curr;
    uint32_t next = pos.next;
    if (lseek(dic, curr + 1, SEEK_SET) < 0) {
      err(1, "failed lseek");
    }
   
    // word
    char word[64];
    read_word(dic, word);
    dprintf(1, "%s", word);

    // descr
    ssize_t offset_dic;
    size_t word_len = strlen(word);
    uint32_t cnt = curr + word_len + 1;
    if (lseek(dic, cnt, SEEK_SET) < 0) {
      err(1, "failed lseek");
    }

    char buf;
    while((offset_dic = read(dic, &buf, sizeof(char))) > 0) {
      if (cnt == next) {
        break;
      }
      
      dprintf(1, "%c", buf);
      cnt++;
    }
    if (offset_dic < 0) {
      err(1, "failed read");
    }
    dprintf(1, "\n");

    close(dic);
    close(idx);

    return 0;
}

