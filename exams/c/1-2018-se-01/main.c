#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <err.h>

int main(int argc, char* argv[]) {
  if (argc != 3 && argc != 4) errx(1, "provide 2 or 3 args");
  if (strcmp(argv[1], "-s") && strcmp(argv[1], "-d")) errx(1, "options should be -s or -d"); 
  if (argc == 4 && !strcmp(argv[1], "-d")) errx(1, "option -d, requires 2 args in total"); 
  if (argc == 3 && !strcmp(argv[1], "-s")) errx(1, "option -s, requires 3 args in total"); 

  char data[2048];
  if (read(0, &data, 2048 * sizeof(char)) == -1) err(1, "failed too read");
  size_t data_len = strlen(data); 

  if (!strcmp(argv[1], "-d")) {
    size_t rem_symbs_len = strlen(argv[2]);
    for (size_t i = 0; i < data_len; i++) {
      bool match = false;
      for (size_t j = 0; j < rem_symbs_len; j++) {
        if (data[i] == argv[2][j]) {
          match = true;
          break;
        }
      }
      if (!match) {
        dprintf(1, "%c", data[i]);
      }
    }
  } else if (!strcmp(argv[1], "-s")) {
    size_t first_symbs_len = strlen(argv[2]);
    size_t second_symbs_len = strlen(argv[3]);
    if (first_symbs_len != second_symbs_len) errx(1, "sets must be with the same size"); 

    for (size_t i = 0; i < data_len; i++) {
      bool match = false;
      for (size_t j = 0; j < first_symbs_len; j++) {
        if (data[i] == argv[2][j]) {
          dprintf(1, "%c", argv[3][j]);
          match = true;
          break;
        }
      }
      if (!match) {
        dprintf(1, "%c", data[i]);
      }
    }
  }
  return 0;
}

