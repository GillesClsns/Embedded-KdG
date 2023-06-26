#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "button_lib.h"
#include "buzzer_lib.h"
#include "display_lib.h"
#include "led_library.h"
#include "music_lib.h"
#include "potentiometer_lib.h"
#include "usart.h"
#include "timer_lib.h"

#define MAX_FUEL 1500
#define MAX_BURST 50

// Data structure to store flight data
typedef struct
{
  double rocketDistance;
  double rocketSpeed;
  double rocketBurst;
  int fuelReserve;
} FlightData;

FlightData *gameLog;
volatile uint32_t counter = 0;
int logEntryIndex = 0;

// Variables for rocket simulation
double rocketBurst = 0;
double rocketSpeed = 100;
double rocketDistance = 9999;
double gravity = 1.622;
int fuelReserve = 1500;

// Flags and settings
int boosting = 0;
int landed = 0;
int crashed = 0;
int selectedLed = 4;
double blinkSpeed = 31;

// Timing variables for interrupt handling
int ticked = 0;
int tickRate = 1000 / 16.384; // 1 second divided into 16 microseconds
int blinking = 0;
double blinkCounter = 0;

/**
 * Displays the current flight parameters on the display.
 * Also updates LEDs based on fuelReserve
 */
void showFlightParameters()
{
  // Adjust selectedLed and LEDs based on fuel reserve
  if (fuelReserve < 1500 && fuelReserve > 1125)
    selectedLed = 3;

  else if (fuelReserve < 1125 && fuelReserve > 750)
  {
    lightDownLed(3);
    selectedLed = 2;
  }
  else if (fuelReserve < 750 && fuelReserve > 375)
  {
    lightDownLed(2);
    selectedLed = 1;
  }
  else if (fuelReserve < 375 && fuelReserve > 0)
  {
    lightDownLed(1);
    selectedLed = 0;
  }
  writeNumber((int)rocketDistance); // Display the rocketDistance
}

/**
 * Calculates the new rocket situation based on current conditions.
 */
void calculateNewSituation()
{

  rocketSpeed += gravity - rocketBurst / 5;
  rocketDistance -= rocketSpeed;
  fuelReserve -= rocketBurst;

  fuelReserve = (fuelReserve - rocketBurst) < 0 ? 0 : fuelReserve - rocketBurst;
  blinkSpeed = (fuelReserve % (MAX_FUEL / 4)) / 16.384;

  if (rocketDistance <= 3)
  {
    if (rocketSpeed <= 5)
      landed = 1;
    else
      crashed = 1;
  }

  if (rocketDistance > 0)
  {
    // Store the current game data in the log
    gameLog[logEntryIndex].rocketBurst = rocketBurst;
    gameLog[logEntryIndex].rocketSpeed = rocketSpeed;
    gameLog[logEntryIndex].rocketDistance = rocketDistance;
    gameLog[logEntryIndex].fuelReserve = fuelReserve;
    logEntryIndex++;
  }

  rocketBurst = 0;
  boosting = 0;
}

// Interrupt Service Routine for button press interrupt
ISR(PCINT1_vect)
{
  if (buttonPushed(1))
  {
    _delay_us(1000);
    if (buttonPushed(1))
    {
      boosting = 1;
    }
  }
}

// Interrupt Service Routine for Timer0 overflow
ISR(TIMER0_OVF_vect)
{
  if (boosting && rocketBurst < MAX_BURST)
    rocketBurst++;

  if (!ticked)
  {
    counter++;
    if (counter >= tickRate)
    {
      // If one second has elapsed, calculate the new situation
      calculateNewSituation();
      counter = 0;
    }
  }

  if (blinkCounter > blinkSpeed)
  {
    if (!blinking)
    {
      lightUpLed(selectedLed);
      blinking = 1;
    }
    else
    {
      lightDownLed(selectedLed);
      blinking = 0;
    }
    blinkCounter = 0;
  }
  else
    blinkCounter++;
}

int main(void)
{
  initDisplay();
  initButtons();
  initTimer0();
  initUSART();
  initLeds();
  initADC();
  enableAllButtonInterrupts();

  lightUpAllLeds(); // Default state

  gameLog = malloc(100 * sizeof(FlightData)); // Allocate memory for game log data (approx. 100 log moments)

  // Rocket simulation loop
  while (!crashed && !landed)
  {
    showFlightParameters();
  }
  writeString("    "); // Clear the display

  printString(crashed ? "Unfortunately, the rocket has crashed!\n\n" : "Congratulations! You have successfully landed the rocket!\n\n");
  if (crashed)
  {
    playLosingSound();
  }
  else
  {
    playWinningSound();
  }

  // Display the game log
  printString("\n>> Rocket Logs:");
  for (int i = 0; i < logEntryIndex; i++)
  {
    printf("    %2d. Distance: %.2f, Speed: %.2f, Burst: %d, Fuel Reserve: %d\n",
           i + 1, gameLog[i].rocketDistance, gameLog[i].rocketSpeed,
           (int)gameLog[i].rocketBurst, gameLog[i].fuelReserve);
  }
  free(gameLog); // Free the memory allocated for game log data

  return 0;
}