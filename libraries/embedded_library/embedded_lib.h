/**
 * @file embedded_lib.h
 * @brief Library for general useful functions.
 */
#pragma once

#include <avr/io.h>

// Macro om een bit in een register te zetten
#define set_bit(reg, bit) ((reg) |= (1 << (bit)))

// Macro om een bit in een register te wissen
#define clear_bit(reg, bit) ((reg) &= ~(1 << (bit)))

// Function to generate a random number within a range
int getRandomNumber(int min, int max);

// Function to clear the serial monitor by printing new lines
void clearSerialMonitor();
