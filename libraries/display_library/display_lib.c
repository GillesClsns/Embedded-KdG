#include <avr/io.h>
#include <util/delay.h>

#include "display_lib.h"

const uint8_t SEGMENT_MAP[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0X80, 0X90};                                                                                                  // Segment byte maps for numbers 0 to 9
const uint8_t SEGMENT_SELECT[] = {0xF1, 0xF2, 0xF4, 0xF8};                                                                                                                                   // Byte maps to select digits 1 to 4
const uint8_t ALPHABET_MAP[] = {0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E, 0xC2, 0x89, 0xCF, 0xE1, 0x8A, 0xC7, 0xEA, 0xC8, 0xC0, 0x8C, 0x4A, 0xCC, 0x92, 0x87, 0xC1, 0xC1, 0xD5, 0x89, 0x91, 0xA4}; // Segment byte maps for alphabet characters

const uint8_t SPACE = 0xFF;

void initDisplay()
{
  sbi(DDRD, LATCH_DIO); // Set LATCH_DIO pin as output
  sbi(DDRD, CLK_DIO);   // Set CLK_DIO pin as output
  sbi(DDRB, DATA_DIO);  // Set DATA_DIO pin as output
}

void shift(uint8_t val, uint8_t bitorder)
{
  uint8_t bit;

  for (uint8_t i = 0; i < NUMBER_OF_SEGMENTS; i++)
  {
    if (bitorder == LSBFIRST)
    {
      bit = !!(val & (1 << i));
    }
    else
    {
      bit = !!(val & (1 << (7 - i)));
    }

    // Write bit to register
    if (bit == HIGH)
      sbi(PORTB, DATA_DIO);
    else
      cbi(PORTB, DATA_DIO);

    // Trigger the clock pin to update the display
    sbi(PORTD, CLK_DIO);
    cbi(PORTD, CLK_DIO);
  }
}

void writeDigitToSegment(uint8_t segment, uint8_t value)
{ // Write a digit to a specific segment. Segment 0 is the leftmost.
  cbi(PORTD, LATCH_DIO);
  shift(SEGMENT_MAP[value], MSBFIRST);
  shift(SEGMENT_SELECT[segment], MSBFIRST);
  sbi(PORTD, LATCH_DIO);
}

void writeNumber(int number)
{ // Write a number between 0 and 9999 to the display
  if (number < 0 || number > 9999)
    return;
  writeDigitToSegment(0, number / 1000);
  writeDigitToSegment(1, (number / 100) % 10);
  writeDigitToSegment(2, (number / 10) % 10);
  writeDigitToSegment(3, number % 10);
}

void writeNumberAndWait(int number, int delay)
{ // Write a number between 0 and 9999 to the display and keep it for a certain number of milliseconds
  if (number < 0 || number > 9999)
    return;
  for (int i = 0; i < delay / 20; i++)
  {
    writeDigitToSegment(0, number / 1000);
    _delay_ms(5);
    writeDigitToSegment(1, (number / 100) % 10);
    _delay_ms(5);
    writeDigitToSegment(2, (number / 10) % 10);
    _delay_ms(5);
    writeDigitToSegment(3, number % 10);
    _delay_ms(5);
  }
}

void writeCharToSegment(uint8_t segment, char character)
{
  if ((int)character > 96 && (int)character < 123)
  {
    character = (int)character - 32; // Convert lowercase letter to uppercase
  }

  uint8_t value;

  if ((int)character < 65 || (int)character > 90)
  {
    if ((int)character > 47 && (int)character < 58)
    {
      value = SEGMENT_MAP[(int)character - 48]; // Map numbers 0 to 9
    }
    else
    {
      value = SPACE; // Map other characters to space
    }
  }
  else
  {
    value = ALPHABET_MAP[(int)character - 65]; // Map uppercase letters A to Z
  }

  cbi(PORTD, LATCH_DIO);
  shift(value, MSBFIRST);
  shift(SEGMENT_SELECT[segment], MSBFIRST);
  sbi(PORTD, LATCH_DIO);
}

void writeString(char *str)
{ // Write a 4-character string to the display
  writeCharToSegment(0, str[0]);
  writeCharToSegment(1, str[1]);
  writeCharToSegment(2, str[2]);
  writeCharToSegment(3, str[3]);
}

void writeStringAndWait(char *str, int delay)
{ // Write a 4-character string to the display and keep it for a certain number of milliseconds
  for (int i = 0; i < delay / 20; i++)
  {
    writeCharToSegment(0, str[0]);
    _delay_ms(5);
    writeCharToSegment(1, str[1]);
    _delay_ms(5);
    writeCharToSegment(2, str[2]);
    _delay_ms(5);
    writeCharToSegment(3, str[3]);
    _delay_ms(5);
  }
}
