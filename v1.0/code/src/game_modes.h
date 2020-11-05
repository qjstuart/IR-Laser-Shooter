/*!
 * \file      game_modes.h
 * \brief     Header file which contains methods needed for the game modes.
 */
#ifndef GAME_MODES_H
#define GAME_MODES_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "timer.h"
#include "shiftregister.h"
#include "lcd.h"
#include "rtc.h"
#include "eeprom.h"
#include "piezo.h"
#include "platform.h"
#include "util.h"
	
/*!
 * \brief Initialiser for game modes
 */
void gamemodes_init(void);

/*!
 * \brief A function which extracts the bit i the index position from the number passed
 * \param number number from which to extract
 * \param index to extract 
 */
int bitExtracted(int number, int index);

/*!
 * \brief Method to convert a number to binary
 * \param number to convert
 */
long long convert(int number);

/*!
 * \brief Method to getColour Binaries (100, 010, 001)
 * \param index to set
 */
long long getColour(int index);

/*!
 * \brief Method to set cell in matrix randomly
 */
int *getMatrixRandom(void);

/*!
 * \brief Method to change target after timeout
 */
void change_target(void);

/*!
 * \brief Method handler for hitting target
 */
void target_hit(void);

/*!
 * \brief Method to set target with new cell
 */
void set_target(void);
	
/*!
 * \brief Method for training mode
 */
void training_mode(void);

/*!
 * \brief Method for writing high score
 * \param to_write score to write
 */
void save_high_score(int to_write);

/*!
 * \brief Method to handle interrupts ( reste button or ir receiver)
 * \param sources to hsow from where the interrupt is coming
 */
void training_mode_isr(uint32_t sources);

/*!
 * \brief Method for 2 player mode
 */
void players_2_mode(void);


/*!
 * \brief Method to test 2 interrupts at the same time
 * \param sources to hsow from where the interrupt is coming
 */
void action_isr_TEST(uint32_t sources);


/*!
 * \brief Method to check if a colour can be shown to the user
 * \param colour to check
 */
int check_colour(int colour);
	
/*!
 * \brief Method to get the current score multiplier
 * \param current_colour colour to get score for
 */
int get_multiplier(int current_colour);

/*!
 * \brief Method to switch target matrix off
 */
void switch_target_off(void);


#endif // GAME_MODES_H
