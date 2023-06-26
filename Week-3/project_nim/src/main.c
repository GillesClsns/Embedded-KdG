#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <usart.h>
#include <util/delay.h>

#include "button_lib.h"
#include "display_lib.h"
#include "led_library.h"
#include "potentiometer_lib.h"
#include "music_lib.h"

#define START_COUNT 21
#define MAX_COUNT 3

// Global variables
int gameStarted;    // Flag indicating if the game has started
char currentPlayer; // Current player ('P' for player, 'C' for computer)
char winner;        // Winner of the game
int count;          // Count of remaining sticks
int choice;         // Player's choice of sticks to remove
int confirm;        // Confirmation flag for computer's choice

char *turns;     // Array to store player turns
int *choices;    // Array to store player choices
int *remaining;  // Array to store remaining sticks after each turn
int turnCounter; // Counter for the number of turns

// Interrupt Service Routine for button presses
ISR(PCINT1_vect)
{
  // First button
  if (buttonPushed(0))
  {
    _delay_us(1000);
    if (buttonPushed(0))
    {
      if (!gameStarted)
      {
        gameStarted = 1;
      }
      else
      {
        if (currentPlayer == 'P')
        {
          if (choice > 1)
          {
            choice--;
          }
        }
      }
    }
  }

  // Second button
  else if (buttonPushed(1))
  {
    _delay_us(1000);
    if (buttonPushed(1))
    {
      if (gameStarted)
      {
        if (currentPlayer == 'P')
        {
          turns[turnCounter] = currentPlayer;
          count -= choice;
          choices[turnCounter] = choice;
          remaining[turnCounter] = count;
          currentPlayer = 'C';
          turnCounter++;
        }
        else
        {
          if (confirm == 0)
          {
            choice = (count - 1) % (MAX_COUNT + 1);
            if (choice == 0)
            {
              choice = rand() % MAX_COUNT + 1;
            }
            confirm++;
          }
          else if (confirm == 1)
          {
            turns[turnCounter] = currentPlayer;
            count -= choice;
            choices[turnCounter] = choice;
            remaining[turnCounter] = count;
            currentPlayer = 'P';
            turnCounter++;
            confirm = 0;
          }
        }

        if (count <= 1)
        {
          if (currentPlayer == 'C')
          {
            winner = 'P';
          }
          else
          {
            winner = 'C';
          }
          gameStarted = 0;
        }
      }
    }
  }

  // Third button
  else if (buttonPushed(2))
  {
    _delay_us(1000);
    if (buttonPushed(2))
    {
      if (gameStarted)
      {
        if (currentPlayer == 'P')
        {
          if (choice < 3)
          {
            choice++;
          }
        }
      }
    }
  }
}

/**
 * Initializes the USART, display, potentiometer, buttons, LEDs, and variables.
 */
void initialize()
{
  initUSART();
  initDisplay();
  initADC();
  initButtons();
  enableAllButtonInterrupts();
  initLeds();

  count = START_COUNT;
  choice = MAX_COUNT;

  turns = calloc(START_COUNT, sizeof(char));
  choices = calloc(START_COUNT, sizeof(int));
  remaining = calloc(START_COUNT, sizeof(int));

  while (!gameStarted)
  {
    writeNumber(readPotentiometer());
  }

  srand(readPotentiometer());

  if (rand() % 2 == 1)
  {
    currentPlayer = 'P';
  }
  else
  {
    currentPlayer = 'C';
  }
}

/**
 * Game loop that updates the display.
 */
void gameLoop()
{
  while (gameStarted)
  {
    for (int i = 0; i < 1000; i++)
    {
      if (i < 500)
      {
        writeCharToSegment(1, currentPlayer);
      }
      else
      {
        writeCharToSegment(1, ' ');
      }
      writeDigitToSegment(0, choice);
      writeDigitToSegment(2, count / 10);
      writeDigitToSegment(3, count % 10);
      _delay_ms(1);
    }
  }
}

/**
 * Prints game statistics.
 */
void printGameStatistics()
{
  for (int i = 0; i < turnCounter; i++)
  {
    printf("%c took %2d match(es) on turn %2d, leaving %2d match(es) remaining.\n", turns[i], choices[i], i + 1, remaining[i]);
  }
}

/**
 * Cleans up allocated memory and displays the winner.
 */
void cleanupAndDisplayWinner()
{
  free(turns);
  free(choices);
  free(remaining);

  while (1)
  {
    if (winner == 'P')
    {
      writeStringAndWait("YOU", 1000);
      playWinningSound();
    }
    else if (winner == 'C')
    {
      writeStringAndWait("COMP", 1000);
      playLosingSound();
    }
    writeStringAndWait("GOAL", 1000);
  }
}

/**
 * Game logic function.
 */
void playGame()
{
  initialize();
  gameLoop();
  printGameStatistics();
  cleanupAndDisplayWinner();
}

int main(void)
{
  playGame();
}