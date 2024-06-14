// df -h | tail -n +2 | cut -d ' ' -f 1 | sort -u

#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

void run_cmd(int ifd, int ofd, char *const arg[]);

void run_cmd(int ifd, int ofd, char *const arg[]) {
    pid_t pid = fork();
    if (pid == -1) err(1, "failed to fork");

    if (pid == 0) {
        if (ifd != -1) {
            dup2(ifd, 0);
            close(ifd);
        }
        if (ofd != -1) {
            dup2(ofd, 1);
            close(ofd);
        }

        execvp(arg[0], arg);
        err(1, "failed to execvp");
    }
}

int main(void) {
    int pfd1[2], pfd2[2], pfd3[2];

    if (pipe(pfd1) == -1) err(1, "faild to pipe");
    if (pipe(pfd2) == -1) err(1, "faild to pipe");
    if (pipe(pfd3) == -1) err(1, "faild to pipe");

    char *const cmd1[] = { "df", "-h", (char*)NULL };
    run_cmd(-1, pfd1[1], cmd1);
    close(pfd1[1]);

    char *const cmd2[] = { "tail", "-n", "+2", (char*)NULL };
    run_cmd(pfd1[0], pfd2[1], cmd2);
    close(pfd1[0]);
    close(pfd2[1]);

    char *const cmd3[] = { "cut", "-d", " ", "-f", "1", (char*)NULL };
    run_cmd(pfd2[0], pfd3[1], cmd3);
    close(pfd2[0]);
    close(pfd3[1]);

    char *const cmd4[] = { "sort", "-u", (char*)NULL };
    run_cmd(pfd3[0], -1, cmd4);
    close(pfd3[0]);

    for(int i = 0; i < 4; i++) {
        wait(NULL);
    }

    return 0;
}

