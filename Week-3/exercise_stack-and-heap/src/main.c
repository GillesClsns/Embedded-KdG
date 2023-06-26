#include <string.h>
#include "usart.h"

/**
 * Copies the given string to the heap and prints it.
 *
 * This function allocates memory on the heap to store the given string,
 * copies the string to the allocated memory, prints the string, and
 * deallocates the memory from the heap.
 *
 * @param zin The string to be copied and printed.
 */
void schrijfStrafOpHeap(const char *zin)
{
  size_t length = strlen(zin);
  char *heapZin = (char *)malloc((length + 1) * sizeof(char)); // Allocate memory on the heap for the sentence
  strcpy(heapZin, zin);                                        // Copy the string to the allocated memory on the heap
  printf("%s\n", heapZin);                                     // Print the sentence from the heap
  free(heapZin);                                               // Deallocate the memory from the heap
}

int main()
{

  initUSART();
  const char *sentence = "Ik mag niet praten in de les.";

  for (int i = 1; i <= 100; i++)
  {
    printf("%d:", i);
    schrijfStrafOpHeap(sentence);
  }

  return 0;
}