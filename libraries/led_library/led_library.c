#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <util/delay.h>

#include "led_library.h"

const uint8_t LED_PINS[] = {10, 11, 12, 13};                     // Pin numbers for the LEDs
const uint8_t NUM_LEDS = sizeof(LED_PINS) / sizeof(LED_PINS[0]); // Number of LEDs

void enableLed(int ledNumber)
{
  if (ledNumber < NUM_LEDS)
  {
    DDRB |= (1 << LED_PINS[ledNumber] - 8); // Enable LED pin as output
    lightDownLed(ledNumber);
  }
}

void enableLeds(uint8_t ledMask)
{
  DDRB |= (ledMask << 2); // Enable multiple LED pins as outputs based on bitmask
  lightDownLeds(ledMask);
}

void initLeds()
{
  for (uint8_t i = 0; i < NUM_LEDS; i++)
  {
    DDRB |= (1 << LED_PINS[i] - 8); // Enable all LED pins as outputs
    lightDownAllLeds();
  }

  // DDRB |= 0b00111100;
}

void lightUpLed(int ledNumber)
{
  if (ledNumber < NUM_LEDS)
  {
    PORTB &= ~(1 << LED_PINS[ledNumber] - 8); // Turn on the specified LED
  }
}

void lightUpLeds(uint8_t ledMask)
{
  PORTB &= ~(ledMask << 2); // Turn on multiple LEDs based on bitmask
}

void lightUpAllLeds()
{
  PORTB &= 0b11000011; // Turn on all LEDs
}

void lightDownLed(int ledNumber)
{
  if (ledNumber >= 0 && ledNumber < NUM_LEDS)
  {
    PORTB |= (1 << LED_PINS[ledNumber] - 8); // Turn off the specified LED
  }
}

void lightDownLeds(uint8_t numLeds)
{
  if (numLeds <= NUM_LEDS)
  {
    uint8_t ledMask = (1 << numLeds) - 1;
    PORTB |= (ledMask << 2); // Turn off the specified number of LEDs starting from LED 0
  }
}

void lightDownAllLeds()
{
  PORTB |= 0b111100; // Turn off all LEDs
}

void toggleLed(int ledNumber)
{
  if (ledNumber < NUM_LEDS)
  {
    PORTB ^= (1 << LED_PINS[ledNumber] - 8); // Toggle the state of the specified LED
  }
}

void dimLed(int ledNumber, int percentage, int duration)
{
  if (ledNumber < NUM_LEDS && percentage >= 0 && percentage <= 100)
  {
    int time = percentage / 10;

    while (duration > 0)
    {
      lightUpLed(ledNumber); // Light up the LED
      _delay_ms(time);

      lightDownLed(ledNumber); // Turn off the LED
      _delay_ms(10 - time);

      duration -= 10;
    }
  }
}

void fadeInLed(int ledNumber, int duration)
{
  const int steps = 100;
  const int delayTime = duration / steps;

  for (int brightness = 0; brightness <= 255; brightness += 255 / steps)
  {
    lightUpLed(ledNumber);
    _delay_ms(delayTime);
    lightDownLed(ledNumber);
    _delay_ms(delayTime);
  }
}

void fadeOutLed(int ledNumber, int duration)
{
  const int steps = 100;
  const int delayTime = duration / steps;

  for (int brightness = 255; brightness >= 0; brightness -= 255 / steps)
  {
    lightUpLed(ledNumber);
    _delay_ms(delayTime);
    lightDownLed(ledNumber);
    _delay_ms(delayTime);
  }
}

void flashLed(int ledNumber)
{
  while (1)
  {
    toggleLed(ledNumber);
  }
}