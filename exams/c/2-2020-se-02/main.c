#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
  if (argc != 3) errx(1, "provide 2 args");

  int pfd[2];
  if (pipe(pfd) == -1) err(1, "failed pipe");

  pid_t pid = fork();
  if (pid == -1) err(1, "failed fork");
  if (pid == 0) {
    close(pfd[0]);
    dup2(pfd[1], 1);
    close(pfd[1]);
    execlp("cat", "cat", argv[1], (char*)NULL);
    err(1, "failed execlp");
  }

  close(pfd[1]);

  int fd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
  if (fd == -1) err(1, "failed open"); 

  bool enc = false;
  ssize_t rs;
  uint8_t buf;
  while ((rs = read(pfd[0], &buf, sizeof(uint8_t))) > 0) {
    if (enc) {
      uint8_t res = buf ^ 0x20;
      if (res == 0x00 || res == 0xFF || res == 0x55 || res == 0x7D) {
        if (write(fd, &res, sizeof(uint8_t)) == -1) err(1, "failed wriote");  
      }
      enc = false;
    }
    if (buf != 0x55) {
      if (buf == 0x7D) {
        enc = true;
      } else {
        if (write(fd, &buf, sizeof(uint8_t)) == -1) err(1, "failed wriote");  
      }
    }
  }
  close(pfd[0]);

  return 0;
}
