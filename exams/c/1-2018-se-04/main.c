#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>

void swap(uint16_t* a, uint16_t* b);
uint16_t partition(uint16_t arr[], int start, int end);
void quicksort(uint16_t arr[], int start, int end);

void swap(uint16_t* a, uint16_t* b) {
  uint16_t tmp = *a;
  *a = *b;
  *b = tmp;
}

uint16_t partition(uint16_t arr[], int start, int end) {
  uint16_t pivot = arr[end];

  uint16_t cnt = start;
  for (int i = start; i < end; i++ ) {
    if (arr[i] < pivot) {
      swap(&arr[cnt], &arr[i]);
      cnt++;
    }
  }
  swap(&arr[cnt], &arr[end]);

  return cnt;
}

void quicksort(uint16_t arr[], int start, int end) {
  if (start >= end) {
    return;
  }

  uint16_t pivot = partition(arr, start, end);  
  quicksort(arr, start, pivot - 1);
  quicksort(arr, pivot + 1, end);
}

int main(int argc, char* argv[]) {
  if (argc != 3) errx(1, "provide 2 args");
  
  int in = open(argv[1], O_RDONLY);
  if (in == -1) err(1, "failed to open");

  int out = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
  if (out == -1) err(1, "failed to open");

  uint16_t buf;
  ssize_t offset;
  int cnt = 0;
  uint16_t data[65535];
  while ((offset = read(in, &buf, sizeof(uint16_t))) > 0) {
    data[cnt] = buf; 
    cnt++;
  }
  if (offset == -1) err(1, "failed to read"); 

  quicksort(data, 0, cnt - 1);

  for (int i = 0; i < cnt; i++) {
    if (write(out, &data[i], sizeof(uint16_t)) == -1) err(1, "failed to read");
  }

  close(in);
  close(out);

  return 0;
}

