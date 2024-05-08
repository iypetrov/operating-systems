#include <stdio.h>

void swap(int* a, int* b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

int partition(int arr[], int start, int end) {
  int pivot = arr[end];
  int cnt = start;
  for (int i = start; i < end; i++) {
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

int binary_search(int arr[], int start, int end, int elem) {
  while (start <= end) {
    int mid = start + (end - start) / 2;
    if (arr[mid] == elem) {
      return mid;
    } else if (arr[mid] > 0) {
      start = mid + 1; 
    } else {
      end = mid - 1;
    }
  }

  return -1;
}

int main(void) {
    int arr[] = {5, 4, 3, 2, 1};
    int arr_len = sizeof(arr) / sizeof(int);

    quicksort(arr, 0, arr_len - 1);
  
    for (int i = 0; i < arr_len; i++) {
      dprintf(1, "%d ", arr[i]);
    }
    dprintf(1, "\n");

    dprintf(1, "%d\n", binary_search(arr, 0, arr_len - 1, 5));

    return 0;
}

