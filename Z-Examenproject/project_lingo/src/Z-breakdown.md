## Game Description
**Description:**
- The game randomly selects a 4-letter word from a word list.
- The first letter of the word is displayed on the game board.
- The player guesses a word that starts with the given letter.
- Each letter of the guessed word that is correct is indicated by a corresponding LED.
- If a letter exists in the word but is not in the correct position, the corresponding LED will blink.
- When the entire word is guessed correctly, a cheerful tune plays.
- The game ends after 5 attempts. If the word is not guessed correctly, a disappointing tune plays.

**Operation:**
- Create a table with at least 50 4-letter words.
- Use the value of the potentiometer as the seed for the random number generator to choose a word from the list.
- Initially, the first correct letter is displayed in the first position. The other positions contain the letter 'A'.
- Start at position 1 so that the first letter can be changed if desired. 
- The current position is indicated by a "dot" on the LED display.
- Use the following buttons to set the letters of the word:
  - Button 1: Go to the next letter in the alphabet. After 'Z', it wraps back to 'A'.
  - Button 2: Go to the previous letter in the alphabet. After 'A', it wraps back to 'Z'.
  - Button 3: Set the letter and move to the next position. After the last position, the word is displayed on the serial monitor and a confirmation is requested. If incorrect, start again at position 1 on the LED display while keeping the already chosen letters.
- After an incorrect guess, the correctly guessed letters are displayed on the screen, and the other positions are set to 'A'.

**Important Note:**
- Do not create a table with the letters of the alphabet. The computer knows the ASCII table!

## Requirements
1. The guesses must be stored on the heap.
2. Keep track of the different guesses. If the word is not found after 5 attempts, display this on the LED display.
3. At the end, display an overview of the guesses on the serial output. If the word is not found after 5 attempts, display the word below a dotted line. If the word is found, display the guesses and the number of attempts. Ask for and display the player's name on the LED display.
