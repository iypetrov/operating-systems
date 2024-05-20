#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <err.h>
#include <sys/types.h>

int main(void) {
  int pfd[2];
  if (pipe(pfd) == -1) err(1, "failed to pipe"); 

  pid_t pid = fork();
  if (pid == -1) err(1, "failed to fork");
  if (pid == 0) {
    close(pfd[0]);
    dup2(pfd[1], 1);
    close(pfd[1]);
    execlp("ps", "ps", "-e", (char*)NULL);
    err(1, "failed to execlp");
  }

  close(pfd[1]);
  dup2(pfd[0], 0);
  close(pfd[0]);
  execlp("grep", "grep", "nvim", (char*)NULL);
  err(1, "failed to execlp");

	return 0;
}

