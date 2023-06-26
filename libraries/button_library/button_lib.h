#pragma once

/**
 * @brief Enable a specific button.
 *
 * @param button The button index (0 to 2) to enable.
 */
void enableButton(int button);

/**
 * @brief Enable all buttons.
 */
void initButtons();

/**
 * @brief Check if a specific button is pushed.
 *
 * @param button The button index (0 to 2) to check.
 * @return 1 if the button is pushed, 0 otherwise.
 */
int buttonPushed(int button);

/**
 * @brief Check if a specific button is released.
 *
 * @param button The button index (0 to 2) to check.
 * @return 1 if the button is released, 0 otherwise.
 */
int buttonReleased(int button);

/**
 * @brief Enable interrupt for a specific button.
 *
 * @param button The button index (0 to 2) to enable interrupt for.
 */
void enableButtonInterrupt(int button);

/**
 * @brief Enable interrupts for all buttons.
 */
void enableAllButtonInterrupts();