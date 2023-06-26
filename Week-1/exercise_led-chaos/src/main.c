#define __DELAY_BACKWARD_COMPATIBLE__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#include "embedded_lib.h"
#include "led_library.h"
#include "usart.h"

int main()
{

  initUSART();
  initLeds();

  while (1)
  {

    int randomIndex = getRandomNumber(0, 3);
    lightUpLed(randomIndex);
    int randomDelay = getRandomNumber(100, 1000);

    _delay_ms(randomDelay);
    lightDownLed(randomIndex);
  }

  return 0;
}
