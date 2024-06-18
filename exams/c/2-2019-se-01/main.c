#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    time_t curr = time(NULL); 
    time_t start = time(NULL);
    if (argc < 3) errx(1, "provide at least 2 args");
    
    while (curr < start + atoi(argv[1])) {
        int pfd[2];
        if (pipe(pfd)== -1) err(1, "failed pipe");

        pid_t pid = fork();
        if (pid == -1) err(1, "failed fork");
        if (pid == 0) {
            close(pfd[0]);
            dup2(pfd[1], 1);
            close(pfd[1]);
            
            char* arr[argc - 1];
            for (int j = 0; j < argc - 2; j++) {
                arr[j] = argv[j + 2];
            }
            arr[argc - 2] = (char*)NULL;

            execvp(argv[2], arr);
            err(1, "failed execvp");
        }

        close(pfd[1]);
        char buf;
        if (read(pfd[0], &buf, sizeof(char)) == -1) err(1, "failed read"); 
        curr = time(NULL);
        close(pfd[0]);

        int stat;
        if (wait(&stat) == -1) err(1, "failed wait");
        if (WIFEXITED(stat)) {
            if (WEXITSTATUS(stat) != 0) {
                warnx("ps exited wit status %d", WEXITSTATUS(stat));
            }
        } else {
            warnx("ps killed");
        }

        char record[4096];
        sprintf(record, "%ld %ld %d\n", start, curr, WEXITSTATUS(stat));
        int fd = open("run.log", O_WRONLY | O_APPEND | O_CREAT, 0644);
        if (fd == -1) err(1, "failed open");
        if (write(fd, &record, strlen(record)) == -1) err(1, "failed to write");
        close(fd);
    }
    
    return 0;
}
