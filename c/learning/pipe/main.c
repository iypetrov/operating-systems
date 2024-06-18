// ps -e -o pid,cmd | tail -n +2 | grep init

#include <err.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
  int pfd1[2];
  if (pipe(pfd1) == -1) err(1, "failed pipe");
  pid_t pid1 = fork();
  if (pid1 == -1) err(1, "failed fork");
  if (pid1 == 0) {
    close(pfd1[0]);
    dup2(pfd1[1], 1);
    close(pfd1[1]);
    execlp("ps", "ps", "-e", "-o", "pid,cmd", (char*)NULL);
    err(1, "failed exec");
  }
  close(pfd1[1]);

  int pfd2[2];
  if (pipe(pfd2) == -1) err(1, "failed pipe");
  pid_t pid2 = fork();
  if (pid2 == -1) err(1, "failed fork");
  if (pid2 == 0) {
    close(pfd2[0]);
    dup2(pfd1[0], 0);
    dup2(pfd2[1], 1);
    close(pfd1[0]);
    close(pfd2[1]);
    execlp("tail", "tail", "-n", "+2", (char*)NULL);
    err(1, "failed exec");
  }
  close(pfd1[0]);
  close(pfd2[1]);

  int pfd3[2];
  if (pipe(pfd3) == -1) err(1, "failed pipe");
  pid_t pid3 = fork();
  if (pid3 == -1) err(1, "failed fork");
  if (pid3 == 0) {
    close(pfd3[0]);
    dup2(pfd2[0], 0);
    dup2(pfd3[1], 1);
    close(pfd2[0]);
    close(pfd3[1]);
    execlp("grep", "grep", "init", (char*)NULL);
    err(1, "failed exec");
  }
  close(pfd2[0]);
  close(pfd3[1]);

  ssize_t rs;
  char buf;
  while ((rs = read(pfd3[0], &buf, sizeof(char))) > 0) {
    dprintf(1, "%c", buf);
  }
  if (rs == -1) err(1, "failed read");
  close(pfd3[0]);

  // we forked 3 ps, that's why we wait for 3 ps
  for (int i = 0; i < 3; i++) {
    int stat;
    if (wait(&stat) == -1) err(1, "failed wait");
    if (WIFEXITED(stat)) {
      if (WEXITSTATUS(stat) != 0) {
        warnx("ps ended with status %d", stat);
      }
    } else {
      warnx("ps killed");
    }
  }

  return 0;
}
