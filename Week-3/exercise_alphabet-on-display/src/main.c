#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

#include "display_lib.h"

int main()
{
  initDisplay();

  char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int alphaCount = sizeof(alphabet) - 1; // Subtract 1 to exclude null terminater
  int currentIndex = 0;

  while (1)
  {
    writeStringAndWait(&alphabet[currentIndex], 500);
    currentIndex = (currentIndex + 1) % alphaCount;
  }

  return 0;
}