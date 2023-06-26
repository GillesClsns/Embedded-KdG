#define __DELAY_BACKWARD_COMPATIBLE__

#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "led_library.h"
#include "button_lib.h"
#include "usart.h"

// Interrupt Service Routine for Pin Change Interrupt 1 (PCINT1)
ISR(PCINT1_vect)
{
  // Check if button 0 is pushed
  if (buttonPushed(0))
  {
    _delay_us(1000);
    if (buttonPushed(0))
    {
      lightUpLeds(0b00000110);
      _delay_ms(5000);

      lightDownLeds(0b00000110);
      _delay_ms(5000);
    }
  }
  // Check if button 1 is pushed
  else if (buttonPushed(1))
  {
    _delay_us(1000);
    if (buttonPushed(1))
    {
      // Blink LED 2 seven times
      for (int i = 0; i < 7; i++)
      {
        lightUpLed(2);
        _delay_ms(500);
        lightDownLed(2);
        _delay_ms(500);
      }
    }
  }
}

int main()
{

  initLeds();
  initButtons();

  // Enable Pin Change Interrupt 1 (PCINT1) and set corresponding pins for interrupt
  PCICR |= _BV(PCIE1);
  PCMSK1 |= _BV(PC1) | _BV(PC2);

  // Enable interrupts globally
  sei();

  while (1)
  {
    // Light up LEDs for 2 seconds
    lightUpLeds(0b00001100);
    _delay_ms(2000);
    // Turn off LEDs for 2 seconds
    lightDownLeds(0b00001100);
    _delay_ms(2000);
  }

  return 0;
}