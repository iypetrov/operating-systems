#include <stdio.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  if (argc != 3 && argc != 5) errx(1, "provide 2 or 4 args");
  if (argc == 3 && !strcmp(argv[2], "-d")) errx(1, "-d requrest 4 args in total");
  if (argc == 5 && !strcmp(argv[2], "-c")) errx(1, "-c requrest 2 args in total");

  char data [4096];
  if (read(0, &data, 4096 * sizeof(char)) == -1) err(1, "failed to read");

  if (!strcmp(argv[1], "-c")) {
    int dash_idx = -1;
    int symbs_len = strlen(argv[2]);
    for (int i = 0; i < symbs_len; i++) {
      if (argv[2][i] == '-') {
        dash_idx = i;
        break;
      }
    }

    if (dash_idx == -1) {
      int idx = atoi(argv[2]);
      dprintf(1, "%c", data[idx]); 
    } else {
      char idx_one_str[4096];
      for (int i = 0; i < dash_idx; i++) {
        idx_one_str[i] = argv[2][i]; 
      }
      char idx_two_str[4096];
      for (int i = dash_idx + 1; i < symbs_len; i++) {
        idx_two_str[i - dash_idx - 1] = argv[2][i]; 
      }

      int idx_one = atoi(idx_one_str);
      int idx_two = atoi(idx_two_str);
      for (int i = idx_one; i <= idx_two; i++) {
        dprintf(1, "%c", data[i]); 
      }
    } 
  } else {
    char del = argv[2][0];

    int dash_idx = -1;
    int symbs_len = strlen(argv[4]);
    for (int i = 0; i < symbs_len; i++) {
      if (argv[4][i] == '-') {
        dash_idx = i;
        break;
      }
    }

    if (dash_idx == -1) {
      int idx = atoi(argv[4]);
      int del_cnt = 0;
      for (int i = 0; i < (int)strlen(data); i++) {
        if (data[i] == del) {
          del_cnt++;
        }
        if (idx == del_cnt && data[i] != del) {
          dprintf(1, "%c", data[i]);
        }
      }
    } else {
      char idx_one_str[4096];
      for (int i = 0; i < dash_idx; i++) {
        idx_one_str[i] = argv[4][i]; 
      }
      char idx_two_str[4096];
      for (int i = dash_idx + 1; i < symbs_len; i++) {
        idx_two_str[i - dash_idx - 1] = argv[4][i]; 
      }

      int idx_one = atoi(idx_one_str);
      int idx_two = atoi(idx_two_str);

      int del_cnt = 0;
      for (int i = 0; i < (int)strlen(data); i++) {
        if (data[i] == del) {
          del_cnt++;
        }
        if (idx_one <= del_cnt && idx_two >= del_cnt) {
          dprintf(1, "%c", data[i]);
        }
      }
    } 
  }

  return 0; 
}

