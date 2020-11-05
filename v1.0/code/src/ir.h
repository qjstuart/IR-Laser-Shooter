/*!
 * \file      ir.h
 * \brief     Controls the ir receiver
 */
#ifndef IR_H
#define IR_H

#include "platform.h"
#include "gpio.h"
#include "delay.h"
#include "lcd.h"

/*! 
 *	\brief Initialises IR receiver
 */
void ir_init(void);

/*! 
 *	\brief Method to test IR receiver
 */
void read_IR(void);

#endif // IR_H
