#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>

void swap(uint32_t* a, uint32_t* b);
uint32_t partition(uint32_t arr[], int start, int end);
void quicksort(uint32_t arr[], int start, int end);

void swap(uint32_t* a, uint32_t* b) {
  uint32_t tmp = *a;
  *a = *b;
  *b = tmp;
}

uint32_t partition(uint32_t arr[], int start, int end) {
  uint32_t pivot = arr[end];

  uint32_t cnt = start;
  for (int i = start; i < end; i++ ) {
    if (arr[i] < pivot) {
      swap(&arr[cnt], &arr[i]);
      cnt++;
    }
  }
  swap(&arr[cnt], &arr[end]);

  return cnt;
}

void quicksort(uint32_t arr[], int start, int end) {
  if (start >= end) {
    return;
  }

  uint32_t pivot = partition(arr, start, end);  
  quicksort(arr, start, pivot - 1);
  quicksort(arr, pivot + 1, end);
}

int main(int argc, char* argv[]) {
  if (argc != 3) errx(1, "provide 2 args");
  
  int in = open(argv[1], O_RDONLY);
  if (in == -1) err(1, "failed to open");

  int out = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
  if (out == -1) err(1, "failed to open");

  uint32_t buf;
  ssize_t offset;
  int cnt = 0;
  uint32_t data[4096];
  while ((offset = read(in, &buf, sizeof(uint32_t))) > 0) {
    data[cnt] = buf; 
    cnt++;
  }
  if (offset == -1) err(1, "failed to read"); 

  quicksort(data, 0, cnt - 1);

  for (int i = 0; i < cnt; i++) {
    if (read(out, &data[i], sizeof(uint32_t)) == -1) err(1, "failed to read");
  }

  close(in);
  close(out);

  return 0;
}

