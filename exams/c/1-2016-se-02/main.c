#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdint.h>

typedef struct __attribute__((packed)) pair {
	uint32_t offset;
	uint32_t length;
} pair;

int get_file_size(int);

int get_file_size(int fd) {
	struct stat st;

	if(fstat(fd, &st) == -1) {
		err(1, "stat");
	}

	return st.st_size;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
      errx(1, "provide 4 args");
    }

    int f1 = open(argv[1], O_RDONLY);
    if (f1 < 0) {
      err(1, "failed to open file");
    }
    int f2 = open(argv[2], O_RDONLY);
    if (f2 < 0) {
      err(1, "failed to open file");
    }
    int f3 = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (f3 < 0) {
      err(1, "failed to open file");
    }

    uint32_t f1_size = get_file_size(f1);
	  uint32_t f2_size = get_file_size(f2);
    
    if(f1_size % sizeof(pair) != 0) {
   		errx(1, "%s: bad file size", argv[1]);
	  }

	  if(f2_size % sizeof(uint32_t) != 0) {
		  errx(1, "%s: bad file size", argv[2]);
	  }

    pair p;
    ssize_t read_size;

    while ((read_size = read(f1, &p, sizeof(pair))) > 0) {
      if ((p.offset + p.length) * sizeof(uint32_t) > f2_size) {
        errx(1, "not valid data in f1");
      }

      if(lseek(f2, p.offset * sizeof(uint32_t), SEEK_SET) < 0) {
        err(1, "failed to lseek");
      }
      uint32_t num;
      for(uint32_t i = 0; i < p.length; i++) {
			  if (read(f2, &num, sizeof(uint32_t)) < 0) {
          err(1, "failed to read");
        }
			  if (write(f3, &num, sizeof(uint32_t)) < 0) {
          err(1, "faield to write");
        }
		  }
    }
    if (read_size < 0) {
      err(1, "failed to read");
    }

    if (close(f1) < 0) {
      err(1, "failed to close a file");
    }
    if (close(f2) < 0) {
      err(1, "failed to close a file");
    }
    if (close(f3) < 0) {
      err(1, "failed to close a file");
    }
    
    return 0;
}

