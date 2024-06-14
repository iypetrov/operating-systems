#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <sys/types.h>
#include <unistd.h>

void gen_hash(char filename[]);

void gen_hash(char filename[]) {
  int m_pfd[2];
  if (pipe(m_pfd) == -1) err(1, "failed to pipe");

  pid_t m_pid = fork();
  if (m_pid == -1) err(1, "failed to fork");

  if (m_pid == 0) {
    close(m_pfd[0]);
    dup2(m_pfd[1], 1);
    close(m_pfd[1]);
    execlp("md5sum", "md5sum", filename, (char*)NULL);
    err(1, "failed to md5sum");
  }
  close(m_pfd[1]);

  char* hashfilename = strcat(filename, ".hash");
  int fd = open(hashfilename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
  if (fd == -1) err(1, "failed to open");

  ssize_t rs;
  char buf;
  while ((rs = read(m_pfd[0], &buf, sizeof(char))) > 0) {
    if (write(fd, &buf, sizeof(char)) == -1) err(1, "failed to write");
  }
  if (rs == -1) err(1, "failed to read");

  close(fd);
}

int main(int argc, char* argv[]) {
  if (argc != 2) errx(1, "provide 1 arg");

  int f_pfd[2];
  if (pipe(f_pfd) == -1) err(1 , "failed to pipe");

  pid_t f_pid = fork();
  if (f_pid == -1) err(1, "failed to fork");
  if (f_pid == 0) {
    close(f_pfd[0]);
    dup2(f_pfd[1], 1);
    close(f_pfd[1]);
    execlp("find", "find", argv[1], "-type", "f", "-not", "-name", "*.hash", "-print0", (char*)NULL);
    err(1, "failed to find");
  }
  close(f_pfd[1]);
  
  ssize_t rs;
  int cnt = 0;
  char filename[4096];
  char buf;
  int worker_cnt = 0;
  while ((rs = read(f_pfd[0], &buf, sizeof(char))) > 0) {
    if (buf != '\0' ) {
      filename[cnt] = buf;
      cnt++;
    } else {
      gen_hash(filename);
      for (int i = 0; i < 4096; i++) {
        filename[i] = '\0';
      }
      cnt = 0;
      worker_cnt++;
    }
  }
  if (rs == -1) err(1, "failed to read");

  for (int i = 0; i < worker_cnt + 1; i++) {
    int status;
    if (wait(&status) == -1) err(1, "failed to wait");
    if (WIFEXITED(status)) {
      if (WEXITSTATUS(status) != 0) {
        warnx("ps ended with status %d", WEXITSTATUS(status));
      }
    } else {
      warnx("ps was killed");
    }
  }

  return 0;
}

