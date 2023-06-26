#include <avr/interrupt.h>
#include <avr/io.h>

#include "button_lib.h"

void enableButton(int button)
{
  if (button < 0 || button > 2)
    return;
  DDRC &= ~_BV(PC1 + button); // Set button pin as input
  PORTC |= _BV(PC1 + button); // Enable internal pull-up resistor for the button pin
}

void initButtons()
{
  DDRC &= ~_BV(PC1) & ~_BV(PC2) & ~_BV(PC3); // Set all button pins as inputs
  PORTC |= _BV(PC1) | _BV(PC2) | _BV(PC3);   // Enable internal pull-up resistors for all button pins
}

int buttonPushed(int button)
{
  if (button < 0 || button > 2)
    return 0;
  if (bit_is_clear(PINC, (PC1 + button))) // Check if the button is pushed
  {
    return 1; // Button is pushed
  }

  return 0; // Button is not pushed
}

int buttonReleased(int button)
{
  if (button < 0 || button > 2)
    return 0;
  if (bit_is_set(PINC, (PC1 + button))) // Check if the button is released
  {
    return 1; // Button is released
  }

  return 0; // Button is not released
}

void enableButtonInterrupt(int button)
{
  if (button < 0 || button > 2)
    return;
  PCICR |= _BV(PCIE1);         // Enable Pin Change Interrupt Control Register 1 (Port C)
  PCMSK1 |= _BV(PC1 + button); // Enable interrupt for the specific button (PCMSK = Pin Change Mask)
  sei();                       // Enable global interrupts
}

void enableAllButtonInterrupts()
{
  PCICR |= _BV(PCIE1);                      // Enable Pin Change Interrupt 1
  PCMSK1 |= _BV(PC1) | _BV(PC2) | _BV(PC3); // Enable interrupts for all buttons
  sei();                                    // Enable global interrupts
}