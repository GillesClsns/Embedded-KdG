#define __DELAY_BACKWARD_COMPATIBLE__

#include <util/delay.h>
#include <stdlib.h>
#include <avr/io.h>

#include "led_library.h"
#include "embedded_lib.h"

#define DELAY_TIME 200
#define NUM_LEDS 4

void exercise1()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    lightUpLed(i);
    _delay_ms(DELAY_TIME);
    lightDownLed(i);
    i = (i % 3 == 0) ? 0 : i;
  }
}

void exercise2()
{
  int randomLed = getRandomNumber(0, NUM_LEDS - 1);
  for (int i = 0; i < 10; i++)
  {
    lightUpLed(randomLed);
    _delay_ms(DELAY_TIME);
    lightDownLed(randomLed);
    _delay_ms(DELAY_TIME);
  }
}

void exercise3(int led, int times)
{
  for (int i = 0; i < times; i++)
  {
    lightUpLed(led);
    _delay_ms(DELAY_TIME);
    lightDownLed(led);
    _delay_ms(DELAY_TIME);
  }
}

void exercise4(int led)
{
  for (int delayTime = 10; delayTime <= 1000; delayTime += 50)
  {
    lightUpLed(led);
    _delay_ms(delayTime);
    lightDownLed(led);
    _delay_ms(DELAY_TIME);
  }
}

void exercise5(const int leds[10])
{
  for (int i = 0; i < 10; i++)
  {
    lightUpLed(leds[i]);
    _delay_ms(DELAY_TIME);
    lightDownLed(leds[i]);
    _delay_ms(DELAY_TIME);
  }
}

void exercise6(int led)
{
  // Fill empty array
  int delays[20];
  int next = 0;

  for (int delayTime = 10; delayTime < 1000; delayTime += 50)
    delays[next++] = delayTime;

  // Use of array to light up LEDs
  for (int i = 0; i < 20; i++)
  {
    lightUpLed(led);
    _delay_ms(delays[i]);
    lightDownLed(led);
    _delay_ms(DELAY_TIME);
  }
}

void exercise7(const char *letters)
{
  int length = strlen(letters);

  // Iterate through each letter in the string
  for (int i = 0; i < length; i++)
  {
    int chosenLed = letters[i] - 'a'; // Convert the letter to an index for the LED
    lightUpLed(chosenLed);            // Light up the LED
    _delay_ms(DELAY_TIME);            // Delay for a certain amount of time
    lightDownLed(chosenLed);          // Turn off the LED
    _delay_ms(DELAY_TIME);            // Delay for the same amount of time
  }
}

void exercise8()
{
  char randomLetters[5];
  srand(0); // Seed the random number generator

  // Generate 5 random letters
  for (int i = 0; i < 5; i++)
  {
    randomLetters[i] = (char)(rand() % 26) + 'a';
  }

  // Iterate through each randomly generated letter
  for (int i = 0; i < 5; i++)
  {
    int amount = randomLetters[i] - 'a' + 1; // Calculate the amount based on the letter
    for (int j = 0; j < amount; j++)
    {
      lightUpAllLeds();      // Light up all LEDs
      _delay_ms(DELAY_TIME); // Delay for a certain amount of time
      lightDownAllLeds();    // Turn off all LEDs
      _delay_ms(DELAY_TIME); // Delay for a certain amount of time
    }
    _delay_ms(1000); // Wait 1 second for the next letter
  }
}
int main()
{
  initLeds();
  const int leds[10] = {1, 2, 3, 4, 8, 9, 4, 6, 1};
  const char *letters = "gilles";

  // exercise1();
  // exercise2();
  // exercise3(1, 3);
  // exercise4(1);
  // exercise5(leds);
  // exercise6(1);
  // exercise7(letters);
  // exercise8();

  return 0;
}
