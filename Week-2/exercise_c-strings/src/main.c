#include <string.h>
#include "usart.h"

#define MAX 7
#define LENGTH 10

void printFirstLetters(char names[MAX][LENGTH])
{
  printf("First letters: ");
  for (int i = 0; i < MAX; i++)
  {
    printf("%c", names[i][0]);
  }
  printf("\n");
}

void printLastLetters(char names[MAX][LENGTH])
{
  printf("Last letters: ");
  for (int i = 0; i < MAX; i++)
  {
    int len = strlen(names[i]);
    printf("%c", names[i][len - 1]);
  }
  printf("\n");
}

char *findSmallestName(char names[MAX][LENGTH])
{
  int smallestIndex = 0;
  for (int i = 1; i < MAX; i++)
  {
    if (strcmp(names[i], names[smallestIndex]) < 0)
    {
      smallestIndex = i;
    }
  }
  return names[smallestIndex];
}

int main()
{

  initUSART();
  char names[MAX][LENGTH] = {"Hans", "Anniek", "Wim", "Gilles", "Luka", "Pierre", "Kenzo"};

  printFirstLetters(names);
  printLastLetters(names);

  char *smallest = findSmallestName(names);
  printf("Smallest name: %s\n", smallest);

  return 0;
}
