# Constants

- STDIN_FILENO `#include <unistd.h>`
- STDOUT_FILENO `#include <unistd.h>`
- STDERR_FILENO `#include <unistd.h>`

# Logging

### Print

- int dprintf(int fd, const char \*format, ...); `#include <stdio.h>`
- int sprintf(char *str, const char *format, ...); `#include <stdio.h>`

```c
#include <stdio.h>
#include <unistd.h>

int main(void) {
    const char* foo = "foo";

    // use dprintf for logging to fd
    dprintf(STDOUT_FILENO, "foo -> %s\n", foo);

    // use sprintf for writing in buffer
    char buf[1024];
    sprintf(buf, "%s", foo);
    dprintf(1, "foo -> %s\n", buf);

    return 0;
}
```

### Error & Warning

`err`/`warn` are for errors from system calls
`errx`/`warnx` are for errors from BL

- void err(int eval, const char \*fmt, ...); `#include <err.h>`
- void warn(const char \*fmt, ...); `#include <err.h>`

```c
#include <stdio.h>
#include <fcntl.h>
#include <err.h>
#include <sys/stat.h>

int main(void) {
    struct stat info;
    const char *file = "text.txt";

    if (stat(file, &info) == -1) {
        err(1, "can not stat file");
        return 99;
    }

    int result = open(file, O_RDONLY);
    if (result < 0) {
        err(1, "could not open file");
        return 99;
    }

    if (info.st_size == 0) {
        warn("file is empty");
        return 99;
    }

    return 0;
}
```

# Stat

Basic structure of stat

```c
struct stat {
    dev_t     st_dev;         /* ID of device containing file */
    ino_t     st_ino;         /* Inode number */
    mode_t    st_mode;        /* File type and mode */
    nlink_t   st_nlink;       /* Number of hard links */
    uid_t     st_uid;         /* User ID of owner */
    gid_t     st_gid;         /* Group ID of owner */
    dev_t     st_rdev;        /* Device ID (if special file) */
    off_t     st_size;        /* Total size, in bytes */
    blksize_t st_blksize;     /* Block size for filesystem I/O */
    blkcnt_t  st_blocks;      /* Number of 512B blocks allocated */

    struct timespec st_atim;  /* Time of last access */
    struct timespec st_mtim;  /* Time of last modification */
    struct timespec st_ctim;  /* Time of last status change */

    #define st_atime st_atim.tv_sec      /* Backward compatibility */
    #define st_mtime st_mtim.tv_sec
    #define st_ctime st_ctim.tv_sec
}
```

- int stat(const char *pathname, struct stat *statbuf); `#include <sys/stat.h>`
- int fstat(int fd, struct stat \*statbuf); `#include <sys/stat.h>`

```c
#include <stdio.h>
#include <sys/stat.h>

int main(void) {
    struct stat info;
    const char *file = "text.txt";

    if (stat(file, &info) == -1) {
        return 99;
    }

    dprintf("id of device containing file -> %ld\n", info.st_dev);
    dprintf("inode number -> %ld\n", info.st_ino);
    dprintf("file type and mode -> %d\n", info.st_mode);
    dprintf("number of hard links -> %ld\n", info.st_nlink);
    dprintf("user id of owner -> %d\n", info.st_uid);
    dprintf("group id of owner -> %d\n", info.st_gid);
    dprintf("device id (if special file) -> %ld\n", info.st_rdev);
    dprintf("total size, bytes -> %ld\n", info.st_size);
    dprintf("block size for filesystem i/o -> %ld\n", info.st_blksize);
    dprintf("number of 512B blocks allocated -> %ld\n", info.st_blocks);

    return 0;
}
```

# File Descriptors

- O_RDONLY `#include <fcntl.h>` opens fd for reading
- O_WRONLY `#include <fcntl.h>` opens fd for writing
- O_RDWR `#include <fcntl.h>` opens fd for reading & writing
- O_CREAT `#include <fcntl.h>` creates file if it doesn't exist (only case when you should provide file permissions with `open`)
- O_TRUNC `#include <fcntl.h>` truncs the file if it exists before opening it
- O_APPEND `#include <fcntl.h>` the starting position is at the end of file if it exists

### Reading

```c
#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    const char* file = "text.txt";
    int fd = open(file, O_RDONLY);

    if (fd < 0) {
        err(1, "failed to open a fd");
        return 99;
    }

    char buf[10];
    int offset;
    while((offset = read(fd, buf, sizeof(buf))) > 0) {
        dprintf(1, "%s", buf);
    }

    if (offset < 0) {
        err(1, "failed to read the file");
        return 99;
    }

    if (close(fd) < 0) {
        err(1, "failed to close a fd %d", fd);
        return 99;
    }

    return 0;
}
```

### Writing

```c
#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    const char* file = "text.txt";
    int fd = open(
        file,
        O_WRONLY|O_CREAT|O_APPEND,
        0644
    );

    if (fd < 0) {
        err(1, "failed to open a fd");
        return 99;
    }

    const char* msg = "Hello World!\n";
    int offset = write(fd, msg, strlen(msg));

    if (offset < 0) {
        err(1, "failed to write in the file");
        return 99;
    }

    if (close(fd) < 0) {
        err(1, "failed to close a fd %d", fd);
        return 99;
    }

    return 0;
}
```

### lseek

`lseek` allows to jump on different offset in file

- off_t lseek(int fd, off_t offset, int whence); `#include <unistd.h>`

The whence options are:

- SEEK_SET - absolute movement
- SEEK_CUR - relative movement based on the current position
- SEEK_END - relative movement at the end of file

### binary

- when you want to read number bit by bit
```c
uint16_t num = 233;
for (int i = 0; i < 16; i++) {
    int bit = (num >> i) & 1;
}
```

- when you want to change the i-th bit of a number, without affecting other
```c
uint16_t num = 233;
int i = 5;

// set i-th bit to 1
uint32_t result1 |= 1 << i;

// set i-th bit to 0
uint32_t result2 &= ~(1 << i);
```
