# Constants

- STDIN_FILENO `#include <unistd.h>`
- STDOUT_FILENO `#include <unistd.h>`
- STDERR_FILENO `#include <unistd.h>`

# Logging

### Print

- int printf(const char \*format, ...); `#include <stdio.h>`
- int dprintf(int fd, const char \*format, ...); `#include <stdio.h>`
- int sprintf(char *str, const char *format, ...); `#include <stdio.h>`

```c
#include <stdio.h>
#include <unistd.h>

int main(void) {
    const char* foo = "foo";

    // use printf for logging
    printf("foo -> %s\n", foo);

    // use dprintf for logging to fd
    dprintf(STDOUT_FILENO, "foo -> %s\n", foo);

    // use sprintf for writing in buffer
    char buf[1024];
    sprintf(buf, "%s", foo);
    printf("foo -> %s\n", buf);

    return 0;
}
```

### Error & Warning

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

    printf("id of device containing file -> %ld\n", info.st_dev);
    printf("inode number -> %ld\n", info.st_ino);
    printf("file type and mode -> %d\n", info.st_mode);
    printf("number of hard links -> %ld\n", info.st_nlink);
    printf("user id of owner -> %d\n", info.st_uid);
    printf("group id of owner -> %d\n", info.st_gid);
    printf("device id (if special file) -> %ld\n", info.st_rdev);
    printf("total size, bytes -> %ld\n", info.st_size);
    printf("block size for filesystem i/o -> %ld\n", info.st_blksize);
    printf("number of 512B blocks allocated -> %ld\n", info.st_blocks);

    return 0;
}
```
