#define __DELAY_BACKWARD_COMPATIBLE__

#include <stdlib.h>
#include <string.h>

#include <avr/io.h>
#include <util/delay.h>
#include <time.h>

#include "usart.h"
#include "led_library.h"

#define NUM_LEDS 4

// Morse code lookup table
const char *morseCodes[] = {
    ".-",   // a
    "-...", // b
    "-.-.", // c
    "-..",  // d
    ".",    // e
    "..-.", // f
    "--.",  // g
    "....", // h
    "..",   // i
    ".---", // j
    "-.-",  // k
    ".-..", // l
    "--",   // m
    "-.",   // n
    "---",  // o
    ".--.", // p
    "--.-", // q
    ".-.",  // r
    "...",  // s
    "-",    // t
    "..-",  // u
    "...-", // v
    ".--",  // w
    "-..-", // x
    "-.--", // y
    "--.."  // z
};

/**
 * Initializes the LEDs and performs a countdown animation.
 */
void countDownLeds()
{

  initLeds();

  for (int i = 0; i < NUM_LEDS; i++)
  {
    lightUpLed(i);
    _delay_ms(1000);
  }

  lightDownAllLeds();
}

/**
 * Blinks the LEDs based on the Morse code representation of a letter.
 *
 * @param letter The letter to blink in Morse code.
 */
void blink(char letter)
{
  if (letter >= 'a' && letter <= 'z')
  {
    const char *code = morseCodes[letter - 'a']; // Get the Morse code for the given letter

    for (int i = 0; code[i] != '\0'; i++)
    {
      if (code[i] == '.')
      {
        lightUpAllLeds();
        _delay_ms(100);
        lightDownAllLeds();
        _delay_ms(200);
      }
      else if (code[i] == '-')
      {
        lightUpAllLeds();
        _delay_ms(800);
        lightDownAllLeds();
        _delay_ms(200);
      }
    }
  }
}

/**
 * Blinks a random sequence of letters in Morse code.
 *
 * @param times The number of times to blink a random letter sequence.
 */
void blinkRandomLetters(int times)
{
  srand(time(NULL)); // ? - More concise and avoids the need to declare an additional variable > `srand((unsigned) time(&t))`

  for (int i = 0; i < times; i++)
  {
    int randomIndex = rand() % 26;         // Generate a random index between 0 and 25
    char randomLetter = randomIndex + 'a'; // Convert the random index to a letter

    blink(randomLetter);
    _delay_ms(1000);
  }
}

/**
 * Blinks a given string in Morse code.
 *
 * @param str The string to blink in Morse code.
 */
void blinkString(const char *str)
{
  int length = strlen(str);

  for (int i = 0; i < length; i++)
  {
    blink(str[i]);
    _delay_ms(1000);
  }
}

int main()
{

  countDownLeds();    // Perform the countdown animation
  _delay_ms(1000);    // Delay for 1000ms (1 second)
  blinkString("gilles"); // Blink the Morse code for given parameter

  return 0;
}