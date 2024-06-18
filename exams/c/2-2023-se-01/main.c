#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void worker(char* file);
void wait_ps(void);

void worker(char* file) {
  int pfd[2];
  if (pipe(pfd) == -1) err(1, "failed pipe");
  pid_t pid = fork();
  if (pid == -1) err(1, "failed fork");
  if (pid == 0) {
    close(pfd[0]);
    dup2(pfd[1], 1);
    close(pfd[1]);
    execlp("md5sum", "md5sum", file, (char*)NULL);
    err(1, "failed exec");
  }
  close(pfd[1]);
  
  strcat(file, ".hash");
  if (file == (char*)NULL) err(1, "failed strcat"); 

  char buf[4096];
  if (read(pfd[0], &buf, 4096 * sizeof(char)) == -1) err(1, "failed read");
  close(pfd[0]);

  int fd = open(file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
  if (fd == -1) err(1, "failed open"); 
  if (write(fd, &buf, strlen(buf)) == -1) err(1, "failed write");
  close(fd);
}

void wait_ps(void) {
  int stat;
  if (wait(&stat) == -1) err(1, "failed wait");
  if (WIFEXITED(stat)) {
    if (WEXITSTATUS(stat) != 0) {
      warnx("ps ended with status %d", WEXITSTATUS(stat));
    }
  } else {
    warnx("ps killed");
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) errx(1, "provide 1 arg");  

  int pfd[2];
  if (pipe(pfd) == -1) err(1, "failed pipe");
  pid_t pid = fork();
  if (pid == -1) err(1, "failed fork");
  if (pid == 0) {
    close(pfd[0]);
    dup2(pfd[1], 1);
    close(pfd[1]);
    execlp("find", "find", argv[1], "-type", "f", "-not", "-name", "*.hash", "-print0", (char*)NULL);
    err(1, "failed exec");
  }
  close(pfd[1]);
  wait_ps();

  ssize_t rs;
  char buf;
  int cnt = 0;
  int file_cnt = 0;
  char file[4096];
  while ((rs = read(pfd[0], &buf, sizeof(char))) > 0) {
    if (buf == '\0') {
      cnt++;
      file_cnt = 0;
      worker(file);
      for (int i = 0; i < 4096; i++) {
        file[i] = '\0';
      }
    } else {
      file[file_cnt] = buf;
      file_cnt++;
    }
  }
  for (int i = 0; i < cnt; i++) {
    wait_ps();
  }

  return 0;
}
