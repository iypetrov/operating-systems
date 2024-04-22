#include <stdio.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define HEAD_MODE "head"
#define TAIL_MODE "tail"
#define LINE_TARGET "line"
#define BYTE_TARGET "byte"
#define DEFAULT_COUNT_LINES 10
#define DEFAULT_COUNT_BYTES 10

typedef struct {
  char pathname[4096];
  char mode[16];
  char target[16];
  int num_target;
  char err_settings[1024];
} settings;

settings get_settings(int argc, char* argv[]); 
void rev(char* data, int i); 

settings get_settings(int argc, char* argv[]) {
  settings ctx;

  if (argc < 5 || argc > 7) {
    strcpy(ctx.err_settings, "provide between 4 and 6 args");
    return ctx;
  }

  if (argc % 2 == 0) {
    strcpy(ctx.err_settings, "provide 4 or 6 args");
    return ctx;
  }

  if (strcmp(argv[1], "-m")) {
    strcpy(ctx.err_settings, "first flag should be -m");
    return ctx;
  }

  if (strcmp(argv[2], HEAD_MODE) && strcmp(argv[2], TAIL_MODE)) {
    strcpy(ctx.err_settings, "not valid mode, should be head/tail");
    return ctx;
  }
  strcpy(ctx.mode, argv[2]);

  if (strcmp(argv[3], "-s")) {
    strcpy(ctx.err_settings, "second flag should be -s");
    return ctx;
  }
  strcpy(ctx.pathname, argv[4]);

  if (argc == 5) {
    strcpy(ctx.target, LINE_TARGET);
    ctx.num_target = DEFAULT_COUNT_LINES;
  } else {
    if (strcmp(argv[5], "-n") && strcmp(argv[5], "-c")) {
      strcpy(ctx.err_settings, "not valid target, should be -n/-c");
      return ctx;
    }

    char target[16];
    strcpy(target, LINE_TARGET);
    if (!strcmp(argv[5], "-c")) {
      strcpy(target, BYTE_TARGET);
    }
    strcpy(ctx.target, target);
    ctx.num_target = atoi(argv[6]);
  }

  ctx.err_settings[0] = '\0';

  return ctx;
}
 
void rev(char* data, int i) {
  for (int j = i - 1; j >= 0; j--) {
    dprintf(1, "%c", data[j]);
  }
}

// the goal of this program is to dup the behaviour of head & tail
// 3 flags are allowed:
//    - "-m" - defines the mode (head/tail)
//    - "-s" - defines the source (file)
//    - "-n" (optional) - number of lines (default is 10 lines)
//    - "-c" (optional) - number of bytes (default is 10 bytes)
// example usages:
//    $ ./main -m head
//    $ ./main -m head -n 5
//    $ ./main -m tail -c 16
int main(int argc, char* argv[]) {
  settings ctx = get_settings(argc, argv);
  if (ctx.err_settings[0] != '\0') {
    errx(99, "%s", ctx.err_settings);
    return 99;
  }

  int fd = open(ctx.pathname, O_RDONLY);
  if (fd < 0) {
    errx(99, "%s", "failed to open fd for reading");
    return 99;
  }

  char buf;
  int offset;
  int cnt = 0;

  // head + line
  if (!strcmp(ctx.mode, HEAD_MODE) && !strcmp(ctx.target, LINE_TARGET)) {
    while ((offset = read(fd, &buf, sizeof(char))) > 0) {
      dprintf(1, "%c", buf);
      if (buf == '\n') {
        cnt++;
      } 

      if (cnt == ctx.num_target) {
        break;
      }
    }

    if (offset < 0) {
      errx(99, "%s", "failed to read");
      return 99;
    }
  }

  // head + byte 
  if (!strcmp(ctx.mode, HEAD_MODE) && !strcmp(ctx.target, BYTE_TARGET)) {
    while ((offset = read(fd, &buf, sizeof(char))) > 0) {
      dprintf(1, "%c", buf);
      cnt++;

      if (cnt == ctx.num_target) {
        break;
      }
    }
  }

  // tail + line
  if (!strcmp(ctx.mode, TAIL_MODE) && !strcmp(ctx.target, LINE_TARGET)) {
    char data[4096];
    int i = 0;
    off_t cur = lseek(fd, 0, SEEK_END);
    cur--;
    if (cur < 0) {
      errx(99, "%s", "failed to lseek");
      return 99;
    }

    while (cur-- > 0) {
      if(lseek(fd, cur, SEEK_SET) < 0) {
        errx(99, "%s", "failed to lseek");
        return 99;
      }

      if (read(fd, &buf, sizeof(char)) < 0) {
        errx(99, "%s", "failed to read");
        return 99;
      }

      data[i] = buf;
      i++;

      if (buf == '\n') {
        cnt++;
      } 

      if (cnt == ctx.num_target) {
        break;
      }
    }
    if (data[i - 1] == '\n') {
      data[i - 1] = '\0';
    }

    rev(data, i);
  }

  // tail + byte 
  if (!strcmp(ctx.mode, TAIL_MODE) && !strcmp(ctx.target, BYTE_TARGET)) {
    char data[4096];
    int i = 0;
    off_t cur = lseek(fd, 0, SEEK_END);
    cur--;
    if (cur < 0) {
      errx(99, "%s", "failed to lseek");
      return 99;
    }

    while (cur-- > 0) {
      if(lseek(fd, cur, SEEK_SET) < 0) {
        errx(99, "%s", "failed to lseek");
        return 99;
      }

      if (read(fd, &buf, sizeof(char)) < 0) {
        errx(99, "%s", "failed to read");
        return 99;
      }

      data[i] = buf;
      i++;

      cnt++;

      if (cnt == ctx.num_target) {
        break;
      }
    }
    
    rev(data, i);
  }

  if (close(fd) < 0) {
    errx(99, "%s", "failed to close fd for reading");
    return 99;
  } 

  return 0;
}
