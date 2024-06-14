#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
  char del = ' ';
  if (argc != 3) errx(1, "provide 2 args");

  int N = atoi(argv[1]);
  if (N == 0) errx(1, "wrong value N");
  int D = atoi(argv[2]);
  if (D == 0) errx(1, "wrong value D");

  int pfd1[2];
  if (pipe(pfd1) == -1) err(1, "failed to pipe");

  int pfd2[2];
  if (pipe(pfd2) == -1) err(1, "failed to pipe");

  pid_t pid = fork();
  if (pid == -1) err(1, "failed to fork");
  if (pid == 0) {
    close(pfd1[1]);
    close(pfd2[0]);
    for (int i = 0; i < N; i++) {
      char buf;
      if (read(pfd1[0], &buf, sizeof(char)) == -1) err(1, "failed to read");
      dprintf(1, "DONG\n");
      if (write(pfd2[1], &del, sizeof(char)) == -1) err(1, "failed to write"); 
    }
    close(pfd1[0]);
    close(pfd2[1]);
  }
  close(pfd1[0]);
  close(pfd2[1]);
  for (int i = 0; i < N; i++) { 
    dprintf(1, "DING\n");
    if (write(pfd1[1], &del, sizeof(char)) == -1) err(1, "failed to write"); 
    if (i != N - 1) {
      char buf;
      if (read(pfd2[0], &buf, sizeof(char)) == -1) err(1, "failed to read");
      sleep(D);
    }
  }
  close(pfd1[1]);
  close(pfd2[0]);

  return 0;
}
