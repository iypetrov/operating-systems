#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct __attribute__((packed)) {
  uint32_t uid;
  uint16_t empty1;
  uint16_t empty2;
  uint32_t start;
  uint32_t end;
} data;

typedef struct __attribute__((packed)) {
  uint32_t uid;
  uint64_t max_diff;
} user;

int main(int argc, char* argv[]) {
  if (argc != 2) errx(1, "provide 1 arg"); 

  int fd = open(argv[1], O_RDONLY);
  if (fd == -1) err(1, "failed to open"); 

  int cnt = 0;
  data info;
  ssize_t offset;
  uint64_t diffs[16384];
  uint64_t diff_sum = 0;
  while ((offset = read(fd, &info, sizeof(data))) > 0) {
    uint64_t diff = info.end - info.start; 
    diffs[cnt] = diff;
    diff_sum += diff;
    cnt++;
  }
  if (offset == -1) err(1, "failed to read");
  if (lseek(fd, 0, SEEK_SET) == -1) err(1, "failed to lseek");

  float avg = diff_sum / cnt;
  float disp = 0;
  for (int i = 0; i < cnt; i++) {
    float tmp = (diffs[i] - avg) * (diffs[i] - avg);
    disp += tmp;
  }
  disp /= cnt;

  int curr_user_index = 0;
  int users_cnt = 0;
  user users[2048];
  while ((offset = read(fd, &info, sizeof(data))) > 0) {
    uint64_t diff = info.end - info.start; 
    if (diff * diff > disp) {
      bool visited = false;
      for (int i = 0; i < users_cnt; i++) {
        if (users[i].uid == info.uid) {
          visited = true;
          curr_user_index = i;
          break;
        }
      }

      if (visited) {
        if (diff > users[curr_user_index].max_diff) {
          users[curr_user_index].max_diff = diff;
        }
      } else {
        users[users_cnt].uid = info.uid;
        users[users_cnt].max_diff = diff;
        users_cnt++;
      }
    }
  }
  if (offset == -1) err(1, "failed to read");

  for (int i = 0; i < users_cnt; i++) {
    dprintf(1, "uid: %d; max diff: %ld\n", users[i].uid, users[i].max_diff);
  }

  return 0;
}
