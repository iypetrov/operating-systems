#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>

off_t get_file_size(int fd);

off_t get_file_size(int fd) {
  struct stat info;
  if (fstat(fd, &info) == -1) err(1, "failed to fstat");
  return info.st_size;
}

int main(int argc, char* argv[]) {
  if (argc != 4) errx(1, "provide 3 args"); 

	int scl = open(argv[1], O_RDONLY);
	if(scl == -1) err(1, "failed to open");
	int sdl = open(argv[2], O_RDONLY);
	if(sdl == -1) err(1, "failed to open");
	int out = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(out == -1) err(1, "failed to open");

  off_t scl_size = get_file_size(scl);
  off_t sdl_size = get_file_size(sdl);

  if(sdl_size % sizeof(uint16_t) != 0) errx(1, "bad sdl file");
	int num_elems = sdl_size / sizeof(uint16_t);
	int needed_scl = num_elems / 8;
	if(num_elems % 8 != 0) needed_scl++;
	if(scl_size != needed_scl)	errx(1, "bad scl file");

	uint8_t scl_byte;
	uint16_t sdl_pos;
	ssize_t offset;
	while((offset = read(scl, &scl_byte, sizeof(uint8_t))) > 0) {
		for(int i = 7; i >= 0; i--) {
			if(read(sdl, &sdl_pos, sizeof(uint16_t)) == -1) err(4, "failed to read");
			uint8_t bit = (scl_byte >> i) & 1;

			if(bit == 1) {
				if(write(out, &sdl_pos, sizeof(uint16_t)) == -1) err(4, "failed to write");
			}
		}
	}
	if(offset == -1)	err(5, "failed to read");

	close(scl);
	close(sdl);
	close(out);

	return 0;
}

