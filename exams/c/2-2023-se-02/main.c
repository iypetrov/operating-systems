#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  pid_t arr[argc -1];
  for (int i = 0; i < argc - 1; i++){
    pid_t pid = fork();
    if (pid == -1) err(1, "failed to fork");
    if (pid == 0) {
      int pfd[2];
      if (pipe(pfd) == -1) err(1, "failed to pipe");

      pid_t pid_w = fork();
      if (pid_w == -1) err(1, "faild to fork");
      if (pid_w == 0) {
        close(pfd[0]);
        dup2(pfd[1], 1);
        close(pfd[1]);
        execlp(argv[i + 1], argv[i + 1], (char*)NULL);
        err(1, "failed to execlp");
      }
      arr[i] = pid_w;

      close(pfd[1]);
      int cnt = 0;
      char trg[4096];
      ssize_t rs;
      char buf;
      while ((rs = read(pfd[0], &buf, sizeof(char))) > 0) {
        if (buf == '\n') {
          dprintf(1, "%s\n", trg);
          if (!strcmp(trg, "foo!")) {
            for(int j = 0; j < argc - 1; j++) {
              if (arr[j] != pid_w) {
                if (kill(arr[j], SIGKILL) == -1) err(1, "failed to kill");
              }
            }
            close(pfd[0]);
            exit(0);
          } else {
            for (int j = 0; j < 4096; j++) {
              trg[j] = '\0';
            } 
            cnt = 0;
          }
        } else {
          trg[cnt] = buf;  
          cnt++;
        }
      }
      if (rs == -1) err(1, "failed to read");
    }
  } 

  for (int i = 0; i < argc - 1; i++) {
    int status;
    if (wait(&status) == -1) err(1, "failed to wait");
    if (WIFEXITED(status)) {
      if (WEXITSTATUS(status) != 0) {
        warn("process failed with status %d", WEXITSTATUS(status));
      }
    } else {
      warnx("process was killed");
    }
  }

  return 0;
}

