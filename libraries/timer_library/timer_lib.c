#include <avr/interrupt.h>

/**
 * Initializes Timer0 for interrupt-based timing.
 */
void initTimer0()
{
  TCCR0A |= _BV(WGM00) | _BV(WGM01); // Fast PWM Mode
  TCCR0B |= _BV(CS02) | _BV(CS00);   // Prescaler = 1024 (every 64µs)
  TIMSK0 |= _BV(TOIE0);              // Enable overflow interrupt
  sei();                             // Enable global interrupts
}