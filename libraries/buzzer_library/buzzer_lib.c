#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"
#include "buzzer_lib.h"

void initBuzzer()
{
  // Check if the buzzer is already enabled
  if (!(DDRD & (1 << PD3)))
  {
    // Set PD3 as output to enable the buzzer
    DDRD |= (1 << PD3);
  }
  else
  {
    // printf("DEBUG: Buzzer already enabled..\n");
    return;
  }
}

void playTone(float frequency, uint32_t duration)
{
  // Calculate the period of the tone in microseconds from the frequency
  uint32_t periodInMicro = (uint32_t)(1000000 / frequency);

  // Convert the duration from milliseconds to microseconds
  uint32_t durationInMicro = duration * 1000;

  // Generate the tone for the specified duration
  for (uint32_t time = 0; time < durationInMicro; time += periodInMicro)
  {
    // Turn on the buzzer
    PORTD &= ~(1 << PD3);

    // Wait for half of the period
    _delay_us(periodInMicro / 2);

    // Turn off the buzzer
    PORTD |= (1 << PD3);

    // Wait for the remaining half of the period
    _delay_us(periodInMicro / 2);
  }
}
