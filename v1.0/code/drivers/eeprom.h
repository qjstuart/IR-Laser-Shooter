/*!
 * \file      eeprom.h
 * \brief     This header file includes methods for data reading and writing from the EEPROM.
 */
#ifndef EEPROM_H
#define EEPROM_H

// Utility
#include <stdint.h>

/*!
 *  Value passed to the EEPROM command register to show 
 *	that the next command to be executed is an 8-bit read.
 */	
#define EEPROM_CMD_8_BIT_READ           (0)

/*!
 *  Value passed to the EEPROM command register to show 
 *	that the next command to be executed is an 16-bit read.
 */	
#define EEPROM_CMD_16_BIT_READ          (1)

/*!
 *  Value passed to the EEPROM command register to show 
 *	that the next command to be executed is an 32-bit read.
 */	
#define EEPROM_CMD_32_BIT_READ          (2)

/*!
 *  Value passed to the EEPROM command register to show 
 *	that the next command to be executed is an 8-bit write.
 */	
#define EEPROM_CMD_8_BIT_WRITE          (3)

/*!
 *  Value passed to the EEPROM command register to show 
 *	that the next command to be executed is an 16-bit read.
 */	
#define EEPROM_CMD_16_BIT_WRITE         (4)

/*!
 *  Value passed to the EEPROM command register to show 
 *	that the next command to be executed is an 32-bit read.
 */	
#define EEPROM_CMD_32_BIT_WRITE         (5)

/*!
 *  Value passed to the EEPROM command register to show 
 *	that the next command to be executed is an erase.
 */	
#define EEPROM_CMD_ERASE_PRG_PAGE       (6)

/*!
	* Define which is used to write to cmd
	*/
#define EEPROM_CMD_RDPREFETCH (1<<3)

/*!
 * Defines the eeprom page size
 */
#define EEPROM_PAGE_SIZE                64

/*!
 * Defines the eeprom page number
 */
#define EEPROM_PAGE_NUM                 63

/*!
 *  Macro to calculate page offset
 */
#define EEPROM_PAGE_OFFSET(n)           (n&0x3F)

/*!
 *  Macro to calculate page address
 */
#define EEPROM_PAGE_ADRESS(n)           ((n&0x3F)<<6)


/*!
 * Required to set bits that set data as cleared.
 */
#define EEPROM_ENDOF_RW                 (26)
/*!
 * Required to set bits that set data as cleared.
 */
#define EEPROM_ENDOF_PROG               (28)
		
/*!
 * Enum showing different modes for writing and reading
 */
typedef enum
{
    MODE_8_BIT = 0,
    MODE_16_BIT,
    MODE_32_BIT
}EEPROM_Mode_Type;

/*! 
 *	\brief Initialises value and power registers
 */
void EEPROM_Init(void);

/*! 
 *	\brief Writes data in a given page address. 
 *	\param page_offset 	Offset within the page to be written to.
 *	\param page_address     Which page in the EEPROM to write to.
 *	\param data Buffer from which data to be written to EEPROM is read
 *	\param mode Read mode, should be:
 *                  - MODE_8_BIT    : read 8 bit mode
 *                  - MODE_16_BIT   : read 16 bit mode
 *                  - MODE_32_BIT   : read 32 bit mode
 *  \param count number read data (bytes)
 */
void EEPROM_Write(uint16_t page_offset, uint16_t page_address, void* data, EEPROM_Mode_Type mode, uint32_t count);

/*! 
 *	\brief Reads data in a certain page address.
 *	\param page_offset      Offset within the page to be written to.
 *	\param page_address     Which page in the EEPROM to write to.
 *	\param data             Buffer to which data to be read from EEPROM is read.
 * 	\param mode Read mode, should be:
 *                  - MODE_8_BIT    : read 8 bit mode
 *                  - MODE_16_BIT   : read 16 bit mode
 *                  - MODE_32_BIT   : read 32 bit mode
 *  \param count number read data (bytes)
 */
void EEPROM_Read(uint16_t page_offset, uint16_t page_address, void* data, EEPROM_Mode_Type mode, uint32_t count);

/*! 
 *	\brief Erase a page at the specific address
 *	\param page_address address to erase (0-62)
 */
void EEPROM_Erase(uint16_t page_address);
#endif
