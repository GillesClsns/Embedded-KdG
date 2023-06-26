#pragma once

/**
 * @brief Enable the buzzer.
 */
void initBuzzer();

/**
 * @brief Play a tone on the buzzer.
 *
 * @param frequency The frequency of the tone in Hz.
 * @param duration The duration of the tone in milliseconds.
 */
void playTone(float frequency, uint32_t duration);
