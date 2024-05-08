#include <stdio.h>

void swap(int* a, int* b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

int partition(int arr[], int start, int end) {
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

void quicksort(int arr[], int start, int end) {
  if (start >= end) {
    return;
  }

  int pivot = partition(arr, start, end);  
  quicksort(arr, start, pivot - 1);
  quicksort(arr, pivot + 1, end);
}

int main(void) {
    int arr[] = {19, 17, 15, 12, 16, 18, 4, 11, 13};
    int arr_len = sizeof(arr) / sizeof(int);
    
    quicksort(arr, 0, arr_len - 1);

    for (int i = 0; i < arr_len; i++) {
      dprintf(1, "%d ", arr[i]);
    }
    dprintf(1, "\n");

    return 0;
}

