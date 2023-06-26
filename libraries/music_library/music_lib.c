#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

#include "buzzer_lib.h"
#include "music_lib.h"

void playWinningSound() {

  initBuzzer();

  // Define the frequencies and durations of the tones
  float frequencies[] = {523.25, 587.33, 659.26, 698.46, 783.99, 987.77};
  uint32_t durations[] = {250, 250, 250, 250, 250, 500};

  // Play the winning sound
  for (int i = 0; i < sizeof(frequencies) / sizeof(frequencies[0]); i++) {
    playTone(frequencies[i], durations[i]);
    _delay_ms(50); // Pause between tones
  }
}

void playLosingSound() {

  initBuzzer();

  // Define the frequencies and durations of the tones
  float frequencies[] = {800, 600, 400};
  uint32_t durations[] = {100, 150, 200};

  // Play the losing sound
  for (int i = 0; i < sizeof(frequencies) / sizeof(frequencies[0]); i++) {
    playTone(frequencies[i], durations[i]);
    _delay_ms(50); // Pause between tones
  }
}

void playNotificationSound() {

  initBuzzer();

  // Define the frequency and duration of the tone
  float frequency = 1000.0;
  uint32_t duration = 200;

  // Play the notification sound
  playTone(frequency, duration);
  _delay_ms(duration); // Pause after playing the tone
}
