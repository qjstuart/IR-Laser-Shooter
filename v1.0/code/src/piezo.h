/*!
 * \file      piezo.h
 * \brief     Controls the piezo speaker
 */
#ifndef PIEZO_H
#define PIEZO_H

#include <platform.h>
#include <gpio.h>
#include "delay.h"

/*! 
 * DURATION for piezo to turn on
 * \def DURATION
 */
#define DURATION 200

/*! 
 *	\brief Initialises piezo speaker
 */
void piezo_init(void);

/*! 
 *	\brief Method to make piezo work and make sound
 */
void piezo_play(void);

#endif // PIEZO_H
