#pragma once

#include <avr/io.h>

/**
 * @brief Enable a single LED as an output based on LED number.
 *
 * @param ledNumber The LED number to enable (0 to NUM_LEDS-1).
 */
void enableLed(int ledNumber);

/**
 * @brief Enable multiple LEDs as outputs based on a bitmask.
 *
 * @param ledMask The bitmask representing the LEDs to enable.
 */
void enableLeds(uint8_t ledMask);

/**
 * @brief Enable all LEDs as outputs.
 */
void initLeds();

/**
 * @brief Turn on an LED based on LED number.
 *
 * @param ledNumber The LED number to turn on (0 to NUM_LEDS-1).
 */
void lightUpLed(int ledNumber);

/**
 * @brief Turn on multiple LEDs based on a bitmask.
 *
 * @param ledMask The bitmask representing the LEDs to turn on.
 */
void lightUpLeds(uint8_t ledMask);

/**
 * @brief Turn on all LEDs.
 */
void lightUpAllLeds();

/**
 * @brief Turn off a specific LED based on LED number.
 *
 * @param ledNumber The LED number to turn off (0 to NUM_LEDS-1).
 */
void lightDownLed(int ledNumber);

/**
 * @brief Turn off a specified number of LEDs starting from LED 0.
 *
 * @param numLeds The number of LEDs to turn off (0 to NUM_LEDS).
 */
void lightDownLeds(uint8_t numLeds);

/**
 * @brief Turn off all LEDs.
 */
void lightDownAllLeds();

/**
 * @brief Toggle the state of an LED based on LED number.
 *
 * @param ledNumber The LED number to toggle (0 to NUM_LEDS-1).
 */
void toggleLed(int ledNumber);

/**
 * @brief Dim an LED based on LED number, percentage, and duration.
 *
 * @param ledNumber The LED number to dim (0 to NUM_LEDS-1).
 * @param percentage The brightness percentage (0 to 100).
 * @param duration The duration of the dimming effect (in milliseconds).
 */
void dimLed(int ledNumber, int percentage, int duration);

/**
 * Fades in an LED by gradually increasing its brightness.
 *
 * @param ledNumber The number of the LED to fade in.
 * @param duration The duration of the fade-in effect in milliseconds.
 */
void fadeInLed(int ledNumber, int duration);

/**
 * Fades out an LED by gradually decreasing its brightness.
 *
 * @param ledNumber The number of the LED to fade out.
 * @param duration The duration of the fade-out effect in milliseconds.
 */
void fadeOutLed(int ledNumber, int duration);

void flashLed(int ledNumber);