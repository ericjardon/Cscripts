#include <stdio.h>

// BUBBLE SORT IN C
// In C we can pass functions as parameters.

typedef int (*comp)(int,int);  // defining a special type
// this type points to a function that returns an integer and receives two integers.

// We use this type to send a 'comparison function' 
// for when we want ascending or descending order

void swap(int *val1, int *val2) {
    // to swap in C you need pointers
    int temp = *val1;
    *val1 = *val2;
    *val2 = temp; 
}

int asc(int a, int b) {
  if (a<b) return -1;
  return 1;
}

int desc(int a, int b) {
  return -1 * desc(a, b);
}

void bubble(int *arr, int size, comp c) {
  int elems = size;
  int didSwap = 1;
  while (didSwap && elems > 1) {
    didSwap = 0;
    elems--;    // last index we can swap with
    for(int i=0; i<elems; i++) {
      if (c(arr[i], arr[i+1]) > 0) {
        swap((arr+i), (arr+i+1)); 
        didSwap = 1;
      }
    }
  }
} 

int printArr(int *arr, int size) {
  for (int i=0; i<size; i++) {
    printf("%d, ", *(arr + i));
  }
  return 0;
}

int main(void) {
    int size = 0;
    printf("Type the number of array elements:\n");
    scanf("%d", &size);
    printf("Type the elements in order, one by line:\n");
    int arr[size];
    //int size = sizeof(arr) / sizeof(int);
    for(int i=0; i<size; i++) {
        scanf("%d", &arr[i]);
    }

    printArr(arr, size);
    bubble(arr, size, asc);
    printf("After sorting:\n");
    printArr(arr, size);
    return 0;
}