#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct {
  int cnt;
  int arr[1024];
} fd_ctx;

void make_pipe(fd_ctx ctx, int pfd[2]);
void close_pipes(fd_ctx ctx);
void close_pipes_except(fd_ctx ctx, int index, int NC, int WC);

void make_pipe(fd_ctx ctx, int pfd[2]) {
  if (pipe(pfd) == -1) err(1, "failed to pipe");
  ctx.arr[ctx.cnt] = pfd[0];
  ctx.arr[ctx.cnt + 1] = pfd[1];
  ctx.cnt+=2;
}

void close_pipes(fd_ctx ctx) {
  for (int i = 0; i < ctx.cnt; i++) {
    close(ctx.arr[i]);
  }
}

int main(int argc, char* argv[]) {
  fd_ctx ctx = { .cnt = 0 };
  if (argc != 3) err(1, "provide 2 args"); 
  int NC = atoi(argv[1]);
  if (NC == 0) err(1, "failed atoi"); 
  int WC = atoi(argv[2]);
  if (WC == 0) err(1, "failed atoi"); 

  for (int i = 0; i < NC; i++) {
    int pfd[2];
    make_pipe(ctx, pfd);
  }

  for (int i = 0; i < WC; i++) {
    if ( i % (NC + 1) == 0) {
      if (i % 3 == 0) {
        dprintf(1, "tic\n");
      } else if (i % 3 == 1) {
        dprintf(1, "tac\n");
      } else if (i % 3 == 2) {
        dprintf(1, "toe\n");
      } 
    } else {
      char buf[5]; 
      if (i % 3 == 0) {
        strcpy(buf, "tic\n");
      } else if (i % 3 == 1) {
        strcpy(buf, "tac\n");
      } else if (i % 3 == 2) {
        strcpy(buf, "toe\n");
      } 
      if (write(ctx.arr[2 * ((i % (NC)) - 1) + 1], &buf, 5 * sizeof(char)) == -1) err(1, "failed write");
    }
  }

  for (int i = 0; i < NC; i++) {
    pid_t pid = fork();
    if (pid == -1) err(1, "failed fork");
    if (pid == 0) {
      if (i < WC) {
        close_pipes(ctx);
        exit(0);
      }

      close(ctx.arr[2 * i + 1]);
      int lim = WC / (NC + 1);
      if (i < WC % (NC + 1)) lim++; 

      char buf[5];
      for (int j = 0; j < lim; j++) {
        if (read(ctx.arr[2 * i], &buf, 5 * sizeof(char)) == -1) err(1, "failed read"); 
        dprintf(1, "%s", buf);
      }
      close_pipes(ctx);
      exit(0);
    } 
  }

  close_pipes(ctx);

  for (int i = 0; i < NC; i++) {
    int stat;
    if (wait(&stat) == -1) err(1, "faild to wait");
    if (WIFEXITED(stat)) {
      if (WEXITSTATUS(stat) != 0) {
        warnx("failed with stat %d", WEXITSTATUS(stat));
      }
    } else {
      warnx("ps killed");
    }
  }

  return 0;
}
