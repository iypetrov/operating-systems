#include <stdio.h>

int binary_search(int arr[], int start, int end, int elem) {
  while (start <= end) {
    int mid = start + (end - start)/2;
    if (arr[mid] == elem) {
      return mid;
    } else if (arr[mid] < elem) {
      start = mid + 1;
    } else {
      end = mid - 1;
    }
  }

  return -1;
}

int main(void) {
    int arr[] = {4, 11, 12, 13, 15, 16, 17, 18, 19};
    int arr_len = sizeof(arr) / sizeof(int);
    
    int result = binary_search(arr, 0, arr_len - 1, 18);
    dprintf(1, "%d\n", result);

    return 0;
}

