#include <err.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  if (argc != 2) errx(1, "provide 1 arg"); 

  int fd = open("kurec", O_RDONLY);
  if (fd == -1) err(1, "failed open");
  if(dup2(fd, 0) == -1) err(1, "failed dup");

  execl(argv[1], argv[1], (char*)NULL);
  err(1, "%s", argv[1]);

  return 0;
}
