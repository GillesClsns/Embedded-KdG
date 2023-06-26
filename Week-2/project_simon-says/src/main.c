#define __DELAY_BACKWARD_COMPATIBLE__

#include <util/delay.h>
#include <avr/interrupt.h>
#include <time.h>

#include "button_lib.h"
#include "led_library.h"
#include "usart.h"
#include "embedded_lib.h"
#include "music_lib.h"

#define DELAY_MS 500
#define NUM_LEDS 4

int button_pushed = 0;

/**
 * Interrupt service routine for handling button presses.
 */
ISR(PCINT1_vect) // Used to detect button pressed during gameplay
{
  if (buttonPushed(0) || buttonPushed(1) || buttonPushed(2))
  {
    button_pushed = 1;
  }
}

/**
 * Generates a puzzle sequence with random numbers.
 *
 * @param numbers An array to store the generated numbers.
 * @param size    The size of the array.
 */
void generatePuzzle(uint8_t *numbers, int size)
{
  for (int i = 0; i < size; i++)
  {
    numbers[i] = getRandomNumber(0, 2);
  }
}

/**
 * Prints the puzzle sequence.
 *
 * @param numbers The array containing the puzzle sequence.
 * @param size    The size of the array.
 * @param debug   Flag indicating whether to print in debug mode.
 */
void printPuzzle(uint8_t *numbers, int size, int debug)
{

  if (debug)
  {
    printf("[DEBUG] ");
    for (int i = 0; i < size; i++)
    {
      printf("%d ", numbers[i] + 1);
    }
  }
  else
  {
    for (int i = 0; i < size; i++)
    {
      printf("%d ", numbers[i] + 1);
    }
  }
}

/**
 * Plays the puzzle sequence by lighting up the LEDs.
 *
 * @param numbers The array containing the puzzle sequence.
 * @param size    The size of the array.
 */
void playPuzzle(uint8_t *numbers, int size)
{

  button_pushed = 0;

  for (int i = 0; i < size; i++)
  {
    lightUpLed(numbers[i]);
    _delay_ms(DELAY_MS);
    lightDownLed(numbers[i]);
    _delay_ms(DELAY_MS);

    if (button_pushed && i > 0)
    {
      printf("[!] Please wait until the sequence is done.\n");
      button_pushed = 0; // Reset the button flag
    }
  }
}

/**
 * Reads the button press and checks if it matches the expected value.
 *
 * @param buttonIndex The index of the button being pressed.
 * @param numbers     The array containing the puzzle sequence.
 * @param size        The size of the array.
 *
 * @return 1 if the button press is correct, 0 otherwise.
 */
int readButtonPress(int buttonIndex, uint8_t *numbers, int size)
{
  lightUpLed(buttonIndex);
  _delay_ms(DELAY_MS);
  lightDownLed(buttonIndex);

  if (numbers[size] != buttonIndex)
  {
    printf("You pressed button %d, wrong!\n", buttonIndex + 1);
    return 0;
  }
  else
  {
    printf("You pressed button %d, correct!\n", buttonIndex + 1);
    return 1;
  }
}

/**
 * Reads the user input for the puzzle sequence.
 *
 * @param numbers The array containing the puzzle sequence.
 * @param size    The size of the array.
 *
 * @return 1 if the input is correct, 0 otherwise.
 */
int readInput(uint8_t *numbers, int size)
{
  int counter = 0;
  while (counter < size)
  {
    if (buttonPushed(0))
    {
      if (readButtonPress(0, numbers, counter) == 1)
      {
        counter++;
      }
      else
      {
        return 0;
      }
    }
    if (buttonPushed(1))
    {
      if (readButtonPress(1, numbers, counter) == 1)
      {
        counter++;
      }
      else
      {
        return 0;
      }
    }
    if (buttonPushed(2))
    {
      if (readButtonPress(2, numbers, counter) == 1)
      {
        counter++;
      }
      else
      {
        return 0;
      }
    }
    _delay_ms(200);
  }

  return 1;
}

/**
 * Starts the game by waiting for the player to press a button.
 */
void startGame()
{

  button_pushed = 0; // Reset the button push flag
  printf("\n>> Press button 1 to start the game\n\n");

  while (!buttonPushed(0))
  {
    for (size_t i = 0; i < NUM_LEDS; i++)
    {
      lightUpLed(i);
      _delay_ms(100);
      lightDownLed(i);
      _delay_ms(100);
    }
  }
  lightDownAllLeds();
}

/**
 * Plays the game by generating and displaying puzzle sequences.
 */
void playGame()
{
  int level = 1;

  while (1)
  {
    int sequenceLength = getRandomNumber(0, 11); // Generate random sequence length from 0 to 10
    uint8_t puzzle[sequenceLength];
    generatePuzzle(puzzle, sequenceLength);
    printf("[DEBUG] The sequence is:\n");
    printPuzzle(puzzle, sequenceLength, 1);
    _delay_ms(2000);
    printf("\n\nMemorize the LED sequence:\n");

    while (1)
    {
      playPuzzle(puzzle, sequenceLength);
      printf("\nNow play the sequence:\n");

      int result = readInput(puzzle, sequenceLength);

      if (result)
      {
        printf("\nCongratulations, you played the sequence correctly!\n\n");
        level++;
        if (level <= 10)
        {
          printf("Moving to Level %d\n\n", level);
          break; // Move to the next level
        }
        else
        {
          printf("You are the Simon Says Master!\n\n");
          return; // Automatically restart the game
        }
      }
      else
      {
        printf("\nOops, you played the sequence incorrectly.\n");
        printf("The correct sequence was: ");
        printPuzzle(puzzle, sequenceLength, 0);
        printf("\n\n");
        return; // Automatically restart the game
      }
    }
  }
}

int main()
{
  initUSART();
  initLeds();
  initButtons();
  enableAllButtonInterrupts();

  while (1)
  {
    srand(time(NULL)); // Initialize the random seed based on current time
    button_pushed = 0;
    startGame();
    playGame();
  }

  return 0;
}