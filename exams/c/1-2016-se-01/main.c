#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>

off_t get_size_file(int fd);
void swap(char* a, char* b);
int partition(char arr[], off_t start, off_t end);
void quicksort(char* arr, off_t start, off_t end);

off_t get_size_file(int fd) {
  struct stat info;
  if (fstat(fd, &info) < 0) {
    err(1, "failed to stat");
  }
  return info.st_size;
}

void swap(char* a, char* b) {
  char tmp = *a;
  *a = *b;
  *b = tmp;
}

int partition(char arr[], off_t start, off_t end) {
  int pivot = arr[end];

  int cnt = start;
  for (int i = start; i < end; i++ ) {
    if (arr[i] < pivot) {
      swap(&arr[cnt], &arr[i]);
      cnt++;
    }
  }
  swap(&arr[cnt], &arr[end]);

  return cnt;
}

void quicksort(char* arr, off_t start, off_t end) {
  if (start >= end) {
    return;
  }

  int pivot = partition(arr, start, end);  
  quicksort(arr, start, pivot - 1);
  quicksort(arr, pivot + 1, end);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
      errx(1, "provide 1 arg");
    }

    int src = open(argv[1], O_RDONLY);
    if (src < 0) {
      err(1, "failed to open a file");
    }

    off_t len = get_size_file(src);
    char arr[len];

    char buf;
    ssize_t offset;
    off_t cnt = 0;
    while ((offset = read(src, &buf, sizeof(char))) > 0) {
      arr[cnt] = buf;
      cnt++;
    }
    if (offset < 0) {
      err(1, "failed to read");
    }

    close(src);

    quicksort(arr, 0, len - 1);

    int dst = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (dst < 0) {
      err(1, "failed to open a file");
    }

    for (off_t i = 0; i < len; i++) {
      if(write(dst, &arr[i], sizeof(char)) < 0) {
        err(1, "failed to write");
      }
    }
    close(dst);

    return 0;
}

