#include <stdio.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <pwd.h>
#include <sys/time.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
  if (argc == 1) errx(1, "provide args"); 

  // user
  struct passwd* us = getpwuid(getuid());
  if (us == NULL) err(1, "failed to getpwuid");

  // time
  struct timeval tv;
  if (gettimeofday(&tv, NULL) == -1) err(1, "failed to gettimeofday"); 
  char ts[200];
  time_t t;
  struct tm *tmp;
  t = time(NULL);
  tmp = localtime(&t);
  if (tmp == NULL) err(1, "failed to localtime");
  if (strftime(ts, sizeof(ts), "%y-%m-%d %H:%M:%S", tmp) == 0) err(1, "failed strftime");

  // write log
  int fd = open("foo.log", O_WRONLY | O_APPEND | O_CREAT, 0644);
  if (fd == -1) err(1, "failed to open"); 
  dprintf(fd, "%s.%ld %s %s\n", ts, tv.tv_usec, us->pw_name, *argv);

  // stop all ps-s of the user
  int pfd1[2];
  int pfd2[2];
  int pfd3[2];
  int pfd4[2];
  int pfd5[2];

  if (pipe(pfd1) == -1) err(1, "failed to pipe"); 
  if (pipe(pfd2) == -1) err(1, "failed to pipe"); 
  if (pipe(pfd3) == -1) err(1, "failed to pipe"); 
  if (pipe(pfd4) == -1) err(1, "failed to pipe"); 
  if (pipe(pfd5) == -1) err(1, "failed to pipe"); 

  pid_t pid1 = fork();
  if (pid1 == -1) err(1, "failed to fork");
  if (pid1 == 0) {
    close(pfd1[0]);
    dup2(pfd1[1], 1);
    close(pfd1[1]);
    execlp("ps", "ps", "-e", "-o", "pid,user", (char*)NULL);
    err(1, "failed to execlp");
  }
  close(pfd1[1]);

  pid_t pid2 = fork();
  if (pid2 == -1) err(1, "failed to fork");
  if (pid2 == 0) {
    close(pfd1[1]);
    close(pfd2[0]);
    dup2(pfd1[0], 0);
    dup2(pfd2[1], 1);
    close(pfd1[0]);
    close(pfd2[1]);
    execlp("sed", "sed", "-E", "s/^[[:space:]]*//g", (char*)NULL);
    err(1, "failed to execlp");
  }
  close(pfd1[0]);
  close(pfd2[1]);

  pid_t pid3 = fork();
  if (pid3 == -1) err(1, "failed to fork");
  if (pid3 == 0) {
    close(pfd2[1]);
    close(pfd3[0]);
    dup2(pfd2[0], 0);
    dup2(pfd3[1], 1);
    close(pfd2[0]);
    close(pfd3[1]);
    execlp("tail", "tail", "-n", "+2", (char*)NULL);
    err(1, "failed to execlp");
  }
  close(pfd2[0]);
  close(pfd3[1]);

  pid_t pid4 = fork();
  if (pid4 == -1) err(1, "failed to fork");
  if (pid4 == 0) {
    close(pfd3[1]);
    close(pfd4[0]);
    dup2(pfd3[0], 0);
    dup2(pfd4[1], 1);
    close(pfd3[0]);
    close(pfd4[1]);
    execlp("grep", "grep", "-E", us->pw_name, (char*)NULL);
  if (pipe(pfd4) == -1) err(1, "failed to pipe"); 
    err(1, "failed to execlp");
  }
  close(pfd3[0]);
  close(pfd4[1]);

  pid_t pid5 = fork();
  if (pid5 == -1) err(1, "failed to fork");
  if (pid5 == 0) {
    close(pfd4[1]);
    close(pfd5[0]);
    dup2(pfd4[0], 0);
    dup2(pfd5[1], 1);
    close(pfd4[0]);
    close(pfd5[1]);
    execlp("cut", "cut", "-d", " ", "-f", "1", (char*)NULL);
    err(1, "failed to execlp");
  }
  close(pfd4[0]);
  close(pfd5[1]);

  ssize_t rs;
  char buf;
  while((rs = read(pfd5[0], &buf, sizeof(char))) > 0) {
    // could kill them from the pipe
    // or with the kill
    dprintf(1, "%c", buf);
  }
  if (rs == -1) err(1, "failed to read");
  close(pfd5[0]);

  return 0;
} 
