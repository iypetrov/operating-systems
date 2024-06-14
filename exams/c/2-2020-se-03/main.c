#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>

typedef struct __attribute__((packed)) data {
	char filename[8];
	uint32_t offset;
	uint32_t length;
} data;

void xor(data* info, int result_fd);

void xor(data* info, int result_fd) {
	int fd = open(info->filename, O_RDONLY);
	if(fd == -1) err(1, "failed to open");

	struct stat st;
	if(fstat(fd, &st) == -1) err(1, "failed to fstat");

	if((uint32_t)st.st_size < (info->offset + info->length) * sizeof(uint16_t)) errx(1, "wrong fmt");

	if(lseek(fd, info->offset * sizeof(uint16_t) , SEEK_SET) == -1) err(1, "failed to lseek");

	uint16_t result = 0;
	ssize_t rs = 0;
	uint16_t buf;
  for (int i = 0; i < (int)info->length; i++) {
	  rs = read(fd, &buf, sizeof(buf)); 
	  if(rs == -1) err(1, "failed to read");
		result ^= buf;
  }

	close(fd);
	if(write(result_fd, &result, sizeof(result)) == -1)	err(1, "failed to save");
}

int main(int argc, char* argv[]) {
	if(argc != 2) errx(1, "provide 1 args");

	int fd = open(argv[1], O_RDONLY);
	if(fd == -1) err(1, "failed to open");

	struct stat st;
	if(fstat(fd, &st) == -1) err(1, "failed to fstat");
	if(st.st_size % sizeof(data) != 0 || (uint8_t)st.st_size > 8 * sizeof(data)) errx(1, "wrong fmt");

	int pfd[2];
	if(pipe(pfd) == -1) err(1, "failed to pipe"); 

	data info;
	ssize_t rs;
	int cnt = 0;
	while((rs = read(fd, &info, sizeof(info))) > 0) {
		cnt++;
		pid_t pid = fork();
		if(pid == -1) err(1, "failed to fork"); 

		if(pid == 0) {
			close(fd);
			close(pfd[0]);

			xor(&info, pfd[1]);

			close(pfd[1]);
      return 0;
		}
	}
	if(rs == -1) err(1, "failed to read");
	// (!): Will deadlock without this. The read() below expects all
	//      write ends of the pipe to be closed.
	close(pfd[1]);

	uint16_t result = 0;
	uint16_t buf;

	while((rs = read(pfd[0], &buf, sizeof(uint16_t))) > 0) {
		result ^= buf;
	}
	if(rs == -1) err(1, "failed to read");

	close(pfd[0]);

	int status;
	for(int i = 0; i < cnt; i++) {
		pid_t pid;
		if((pid = wait(&status)) == -1) err(1, "failed to wait");

		if (WIFEXITED(status)) {
			if(WEXITSTATUS(status) != 0) {
				warnx("child %d exited with code %d", pid, WEXITSTATUS(status));
			}
		} else {
			warnx("child %d was killed", pid);
		}
	}

	dprintf(1, "result: %d\n", result);

	close(fd);
	return 0;
}

