#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
  if (argc != 2) errx(1, "provide 1 arg"); 

  if (mkfifo("kurec", 0644) == -1) err(1, "failed to mkfifo");

  int fd = open("kurec", O_WRONLY);
  if (fd == -1) err(1, "failed open");

  if (dup2(fd, 1) == -1) err(1, "failed dup");
  close(fd);
  execlp("cat", "cat", argv[1], (char*)NULL);
  err(1, "failed dup2");

  return 0;
}
