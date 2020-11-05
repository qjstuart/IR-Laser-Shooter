/*!
 * \file      shiftregister.h
 * \brief     Controls the shift register
 */
#ifndef SHIFTREGISTER_H
#define SHIFTREGISTER_H

#include "delay.h"
#include <stdint.h>
#include <stdbool.h>
#include <platform.h>
#include <gpio.h>
#include <stdlib.h> 

/*! 
 * LEAST SIGNIFICANT BIT
 * \def LSB 
 */
#define LSB 1 

/*! 
 * MOST SIGNIFICANT BIT
 * \def MSB 
 */
#define MSB 0

/*! 
 * Number of registers
 * \def NUM_OF_REGISTERS 
 */
#define NUM_OF_REGISTERS 2 

/*! 
 * Function to set a bit
 * \def bitSet 
 */
#define bitSet(value, bit) ((value) |= (1UL << (bit)))

/*! 
 * Function to clear a bit
 * \def bitClear 
 */
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))

/*! 
 * Function to write a bit
 * \def bitWrite 
 */
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

/*! \brief Method to intiliase shift register
 */
void register_init(void);
	
/*! \brief Method to output to shift register
 *  \param bit_order  bit order
 *  \param value   		value to pass
 */
void shift_reg_output(uint8_t bit_order, uint8_t value);

/*! \brief Method to write to shift register for one output
 *  \param output_index The index of the output to write to
 *  \param value 	The value to pass
 */
void reg_write(int output_index, bool value);

/*! \brief Method  to write to shift register for all outputs
 *  \param outputs The index of outputs to write to 
 *  \param values  The values to pass
 */
void reg_write_multiple(int* outputs, bool* values);

/*! \brief Method  to write to shift register for all outputs
 *  \param outputs The index of outputs to write to 
 *  \param values  The values to pass
 */
void reg_write_multiple_1sr(int* outputs, bool* values);

/*! \brief Method to write to shift register outputs STILL TO BE TESTED
 *  \param values 	The values to pass
 */
void reg_write_TO_TEST(bool* values);


/*! \brief Method to test shift register by going through the matrix, cell by cell
 *  \param speed time it takes between cell change
 */
void effect_test(int speed);

/*! \brief Method to test shift register by going through the matrix, cell by cell for 16 cells with 1 shift register
 *  \param speed time it takes between cell change
 */
void effect_test_1sr(int speed);

/*! \brief Method to test shift register by going through the matrix, cell by cell for 4 cells with 1 shift register
 *  \param speed time it takes between cell change
 */
void effect_test_1sr_4_cells(int speed);

#endif // SHIFTREGISTER_H
