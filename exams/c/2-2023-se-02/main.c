#include <stdio.h>
#include <err.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct {
  int fd;
  pid_t pid;
} work_res;

work_res worker(char* cmd);
void wait_ps(void);
void listen(work_res wr, work_res arr[], int len);

work_res worker(char* cmd) {
  int pfd[2];
  if (pipe(pfd) == -1) err(1, "failed pipe");
  pid_t pid = fork();
  if (pid == -1) err(1, "failed to fork");
  if (pid == 0) {
    close(pfd[0]);
    dup2(pfd[1], 1);
    close(pfd[1]);
    execlp(cmd, cmd, (char*)NULL);
    err(1, "failed to execlp");
  }
  close(pfd[1]);
  work_res res;
  res.fd = pfd[0];
  res.pid = pid; 
  return res;
}

void wait_ps(void) {
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

void listen(work_res wr, work_res arr[], int len) {
  pid_t pid = fork();
  if (pid == -1) err(1, "failed to fork");
  if (pid == 0) {
    ssize_t rs;
    char buf;
    int cnt = 0;
    char word[4096];
    while ((rs = read(wr.fd, &buf, sizeof(char))) > 0) {
      dprintf(1, "%c", buf);
      if (buf == '\n') {
        word[cnt] = '\0';
        if (!strcmp("foo!", word)) {
          for (int i = 0; i < len; i++) {
            if (arr[i].pid != wr.pid) {
              close(arr[i].fd);
              if (kill(arr[i].pid, SIGTERM) == -1) err(1, "failed kill");
            }
          } 
          exit(0);
        } else {
          cnt = 0;
          for (int i = 0; i < 4096; i++) {
            word[i] = '\0';
          }
        }
      } else {
        word[cnt] = buf;
        cnt++;
      }
    }
    exit(0);
  }
}

int main(int argc, char* argv[]) {
  work_res arr[argc - 1];
  for (int i = 0; i < argc - 1; i++){
    arr[i] = worker(argv[i + 1]);
  } 

  for (int i = 0; i < argc - 1; i++){
    listen(arr[i], arr, argc - 1);
  } 

  for (int i = 0; i < argc - 1; i++) {
    wait_ps();
  }

  return 0;
}

