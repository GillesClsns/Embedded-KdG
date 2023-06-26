#include <string.h>
#include <stdlib.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "button_lib.h"
#include "buzzer_lib.h"
#include "display_lib.h"
#include "embedded_lib.h"
#include "led_library.h"
#include "music_lib.h"
#include "potentiometer_lib.h"
#include "usart.h"
#include "timer_lib.h"

#define MAX_GUESSES 5
#define WORD_LENGTH 4
#define MAX_WORDS 50
#define NUM_LEDS 4

typedef struct
{
  char selectedWord[WORD_LENGTH + 1]; // Stores the selected word for the game
  char guessedWord[WORD_LENGTH + 1];  // Stores the current guessed word
  int selectedIndex;                  // Index of the selected letter in the guessed word
  int numGuesses;                     // Number of guesses made so far
  char **guesses;                     // Stores the previous guesses
} GameState;

GameState game;

int playing = 0;       // Game state, used for time playing
int totalPlayTime = 0; // Used to store playertime
int tps = 0;           // Timer ticks

const char *words[MAX_WORDS] = {
    "BOOK", "TREE", "HOME", "TIME", "WORK", "PLAY", "LOVE", "BIRD", "DOOR",
    "CITY", "AUTO", "HOND", "BOOT", "KAAS", "LAMP", "VLIEG", "OGEN", "NEUS",
    "HOED", "HUIS", "RING", "BOOT", "HART", "FILM", "CODE", "RAIN", "BIRD",
    "FIRE", "GAME", "FISH", "HAIR", "IDEA", "MOON", "BABY", "STAR", "WAVE",
    "TREE", "LEAF", "SNOW", "ROAD", "WIND", "SHIP", "KISS", "WINE", "FROG",
    "DUCK", "BEAR", "LION", "SONG", "BIRD"};

// Interrupt Service Routine for Timer0 overflow
ISR(TIMER0_OVF_vect)
{
  if (playing)
  {
    tps++;
  }
  if (tps >= (1000 / 16.384)) // Every second.. 16.384 -> timerverloopfrequentie (= clockspeed / prescaler (=1024))
  {
    totalPlayTime++;
    tps = 0;
  }
}

/**
 * @brief Function to get a random word from the list of words
 * @return A randomly selected word from the list
 */
const char *getRandomWord()
{
  int randomIndex = getRandomNumber(0, MAX_WORDS - 1);
  return words[randomIndex];
}

/**
 * @brief Function to display the selected word on the segment display
 * @param word The word to be displayed
 */
void displaySelectedWord(const char *word)
{
  if (strlen(word) != WORD_LENGTH)
  {
    printf("[ERROR: Invalid word length]");
    return;
  }

  for (int i = 0; i < WORD_LENGTH; i++)
    writeCharToSegment(i, word[i]);
}

/**
 * @brief Function to update the LEDs by validating letters
 */
void updateLeds()
{
  lightDownAllLeds();

  for (int i = 0; i < WORD_LENGTH; i++)
  {
    if (game.guessedWord[i] == game.selectedWord[i])
    {
      lightUpLed(i); // Light up LED if letter is the same at the same index
    }
    else if (strstr(game.selectedWord, &game.guessedWord[i]) != NULL)
    {
      if (totalPlayTime % 2 == 0) // Flash
      {
        lightUpLed(i);
      }
      else
        lightDownLed(i);
    }
    else
    {
      lightDownLed(i); // Turn off LED if letter doesn't appear in the word
    }
  }
}

/**
 * @brief Function to update the LEDs by selected index
 * @param selectedIndex The index of the currently selected letter in the guessed word
 * @deprecated Assignment unclear, see "Beschrijving" & "Werking" -> led display?
 */
void updateLedsIndex(int selectedIndex)
{
  lightDownAllLeds();
  if (selectedIndex >= 0 && selectedIndex < NUM_LEDS)
    lightUpLed(selectedIndex);
}

/**
 * @brief Function to print the game description on the serial monitor
 */
void printGameDescription()
{
  clearSerialMonitor();
  printf("\n\n[DEBUG: %s]\n", game.selectedWord);
  printf("\n\n[?] Welcome to the exciting word guessing game!\n\n");
  printf(">>> Game Description\n    ");
  printf("In this game, your challenge is to guess a secret 4-letter word.\n    "
         "You'll be provided with the first letter of the word, which is displayed on the game board.\n    "
         "Your task is to guess a word that starts with that letter.\n\n"
         ">>> Good luck!\n\n");
}

/**
 * @brief Function to print the previous guesses on the serial monitor
 */
void printGuesses()
{
  printf("%s\n", (game.numGuesses <= 0) ? "[!] You haven't made any guesses yet!" : "\n>> Previous guesses:");
  for (int i = 0; i < game.numGuesses; i++)
    printf("    %d. %s\n", i + 1, game.guesses[i]);
}

/**
 * @brief Function to handle button presses
 */
void handleButtonPress()
{
  if (buttonPushed(0) && buttonPushed(1)) // Extension 1.0
    printGuesses();                       // Print all the guesses on the serial monitor

  else if (buttonPushed(0) || buttonPushed(1))
  {
    // Update the selected letter in the guessedWord array
    char currentLetter = game.guessedWord[game.selectedIndex];
    char updatedLetter = currentLetter + (buttonPushed(1) ? 1 : -1);

    // Wrap around to 'A' if the letter goes below 'A' or above 'Z'
    updatedLetter = (updatedLetter < 'A') ? 'Z' : (updatedLetter > 'Z') ? 'A'
                                                                        : updatedLetter;
    game.guessedWord[game.selectedIndex] = updatedLetter;
  }

  else if (buttonPushed(2))
  {
    // Guess confirmation
    if (game.selectedIndex == WORD_LENGTH - 1)
    {
      clearSerialMonitor();
      printf(">> Is this your final guess? Guess: %s\n", game.guessedWord);
      printf(">> Press button 0 for yes or button 1 for no.\n");

      while (1)
      {
        displaySelectedWord(game.guessedWord); // Main loop is being interrupted - fix

        if (buttonPushed(0)) // Guess confirmation
        {

          game.guesses[game.numGuesses] = (char *)malloc((WORD_LENGTH + 1) * sizeof(char));
          strcpy(game.guesses[game.numGuesses], game.guessedWord);
          game.numGuesses++;

          // Guess validation
          if (strcmp(game.guessedWord, game.selectedWord) == 0)
          {
            printf("\n[!] Congratulations! You guessed correctly.\n    You did it in %d times. The word was indeed %s!\n", game.numGuesses, game.selectedWord);
            printGuesses();
            playWinningSound();
            printf("\n>> You guessed the word within %d seconds!\n", totalPlayTime);
          }
          else
          {
            printf("\n[!] Sorry, you lost. Try again! [%d/%d]\n", game.numGuesses, MAX_GUESSES);
            playLosingSound();

            if (game.numGuesses >= MAX_GUESSES)
            {
              printf("\n>> Maximum number of guesses reached. Game over.\n");
              printf("\n>> The correct word was: %s\n", game.selectedWord);
              break;
            }
          }

          // Create the defaultWord with the first letter from selectedWord and 'A's for the remaining letters
          memset(game.guessedWord, 'A', WORD_LENGTH);
          game.guessedWord[0] = game.selectedWord[0];
          game.guessedWord[WORD_LENGTH] = '\0';
          break;
        }

        else if (buttonPushed(1))
        {
          printf(">> Guess not finalized. Continue guessing...\n");
          playNotificationSound();
          break; // Exit the while loop
        }
      }
      game.selectedIndex = 0;
    }

    else
      game.selectedIndex = (game.selectedIndex + 1) % WORD_LENGTH;
  }
}

// Interrupt service routine for button presses
ISR(PCINT1_vect) { handleButtonPress(); }

/**
 * @brief Initializes the game by setting up the necessary components and variables.
 */
void initializeGame()
{
  initDisplay();
  initButtons();
  initTimer0();
  initUSART();
  initLeds();
  initADC();

  enableAllButtonInterrupts();
  srand(readPotentiometer());

  // Allocate memory for the guesses
  game.guesses = (char **)malloc(MAX_GUESSES * sizeof(char *)); // Dynamic memory (char** = Pointer naar een array van pointers naar strings)
  for (int i = 0; i < MAX_GUESSES; i++)
    game.guesses[i] = NULL;

  // Generate a random word and copy it to the selectedWord array
  const char *randomWord = getRandomWord();
  strncpy(game.selectedWord, randomWord, WORD_LENGTH);
  game.selectedWord[WORD_LENGTH] = '\0';

  // Create the defaultWord with the first letter from randomWord and 'A's for the remaining letters
  strncpy(game.guessedWord, randomWord, 1);
  memset(game.guessedWord + 1, 'A', WORD_LENGTH - 1);
  game.guessedWord[WORD_LENGTH] = '\0';

  printGameDescription();
  playing = 1; // Timer starts ticking..
}

int main()
{
  initializeGame();

  while (1)
  {
    if (game.numGuesses >= MAX_GUESSES)
    {
      displaySelectedWord(game.selectedWord); // Display the correct word on the segment display
    }
    else
    {
      displaySelectedWord(game.guessedWord); // Display the current guessed word on  the segment display
    }
    updateLedsIndex(game.selectedIndex);
  }

  playing = 0;
  return 0;
}