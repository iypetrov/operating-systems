#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
  if (argc != 2) err(1, "provide 1 arg");

  // Writing
  int wfd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
  if (wfd == 0) err(1, "failed open");

  const char* log = "very important log\n";
  if (write(wfd, log, strlen(log)) == -1) err(1, "failed write"); 

  close(wfd);

  // Reading
  int rfd = open(argv[1], O_RDONLY);
  if (rfd == 0) err(1, "failed open");

  ssize_t rs;
  char buf;
  while ((rs = read(rfd, &buf, sizeof(char))) > 0) {
    dprintf(1, "%c", buf);
  }
  if (rs == -1) err(1, "failed read");

  return 0;
}
