#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/interrupt.h>
#include <avr/io.h>
#include <button_lib.h>
#include <led_library.h>
#include <stdio.h>
#include <stdlib.h>
#include <usart.h>
#include <util/delay.h>

volatile int ledStates = 0; // volatile -> Indicates to the compiler that a variable's value may change unexpectedly.

ISR(PCINT1_vect)
{
  _delay_us(1000);

  for (int i = 0; i < 3; i++)
  {
    if (buttonPushed(i))
    {
      ledStates ^= (1 << i); // Toggle the state of the corresponding LED
    }
  }
}

int main()
{
  initLeds();
  initButtons();
  enableAllButtonInterrupts();

  while (1)
  {
    for (int i = 0; i < 3; i++)
    {
      if (ledStates & (1 << i))
      {
        lightUpLed(i);
      }
    }

    _delay_ms(500);
    lightDownAllLeds();
    _delay_ms(500);
  }

  return 0;
}
