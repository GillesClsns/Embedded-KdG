#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

#include "display_lib.h"

int main()
{
  initDisplay();

  int getallen[] = {1234, 2345, 3456, 4567, 5678, 6789, 7890, 8901, 9012};

  while (1)
  {
    for (int i = 0; i >= 0; i++)
    {
      writeNumberAndWait(getallen[i], 500);
    }
    _delay_ms(500);
  }

  return 0;
}