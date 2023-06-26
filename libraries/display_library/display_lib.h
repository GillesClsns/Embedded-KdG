#pragma once

#include <avr/io.h>

// Pin Definitions
#define LATCH_DIO PD4
#define CLK_DIO PD7
#define DATA_DIO PB0

// Signal Levels
#define LOW 0
#define HIGH 1

// Shift Order
#define LSBFIRST 0
#define MSBFIRST 1

// Number of Segments
#define NUMBER_OF_SEGMENTS 8

// Macro for setting a bit in a register
#define sbi(register, bit) (register |= _BV(bit))

// Macro for clearing a bit in a register
#define cbi(register, bit) (register &= ~_BV(bit))

/**
 * @brief Initialize the display.
 */
void initDisplay();

/**
 * @brief Write a number to a specific segment.
 * @param segment The segment to write the number to.
 * @param value The number to be written.
 */
void writeDigitToSegment(uint8_t segment, uint8_t value);

/**
 * @brief Write a number (0-9999) to the display.
 * @param number The number to be written.
 */
void writeNumber(int number);

/**
 * @brief Write a number (0-9999) to the display and wait for a specified delay.
 * @param number The number to be written.
 * @param delay The delay in milliseconds.
 */
void writeNumberAndWait(int number, int delay);

/**
 * @brief Write a character to a specific segment.
 * @param segment The segment to write the character to.
 * @param character The character to be written.
 */
void writeCharToSegment(uint8_t segment, char character);

/**
 * @brief Write a string to the display.
 * @param str The string to be written.
 */
void writeString(char *str);

/**
 * @brief Write a string to the display and wait for a specified delay.
 * @param str The string to be written.
 * @param delay The delay in milliseconds.
 */
void writeStringAndWait(char *str, int delay);