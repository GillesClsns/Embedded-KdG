#include <stdio.h>
#include "usart.h"

#define MAX 5
#define ZEVENVOUD(x) ((x)*7)

void printArray(int *arr)
{
  printf("Array elements:\n");
  for (int i = 0; i < MAX; i++)
  {
    printf("Element %d: %d\tAddress: %p\n", i, arr[i], &arr[i]);
  }
  printf("\n");
}

void parseArray(int *arr)
{
  // Fill the array with multiples of 7
  for (int i = 0; i < MAX; i++)
  {
    arr[i] = ZEVENVOUD(i);
  }
}

int main()
{
  initUSART();
  int arr[MAX] = {0, 1, 2, 3, 4};

  printArray(&arr);
  parseArray(&arr);
  printArray(&arr);

  return 0;
}