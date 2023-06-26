#include "display_lib.h"
#include "potentiometer_lib.h"

int main()
{
  initDisplay();
  initADC();

  while (1)
  {
    writeNumber(readPotentiometer());
  }
  
}