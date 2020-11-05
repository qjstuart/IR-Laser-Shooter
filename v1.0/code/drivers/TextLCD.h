/* mbed TextLCD Library, for a 4-bit LCD based on HD44780
 * Copyright (c) 2014, http://mbed.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* Modified for use with LPC4088 experiment bundle;
 * Copyright 2016-2017 Johann A. Briffa
 */

#ifndef MBED_TEXTLCD_H
#define MBED_TEXTLCD_H

#include <platform.h>

// Pin definitions for serial to parallel converter
#define PIN_SER  P1_24
#define PIN_SCK  P1_20
#define PIN_RCK  P1_2

//Definitions for default (WH) mapping between serial port expander pins and LCD controller
//This hardware supports the I2C bus expander (PCF8574/PCF8574A or MCP23008) and SPI bus expander (74595) interfaces
//See https://mbed.org/cookbook/Text-LCD-Enhanced
//
//Note: LCD RW pin must be connected to GND
//      E2 is used for LCD40x4 (second controller)
//      BL may be used to control backlight
#define D_LCD_PIN_D4   0
#define D_LCD_PIN_D5   1
#define D_LCD_PIN_D6   2
#define D_LCD_PIN_D7   3
#define D_LCD_PIN_RS   4
#define D_LCD_PIN_E    5

//Bitpattern Defines for I2C PCF8574/PCF8574A, MCP23008 and SPI 74595 Bus expanders
//
#define D_LCD_D4       (1<<D_LCD_PIN_D4)
#define D_LCD_D5       (1<<D_LCD_PIN_D5)
#define D_LCD_D6       (1<<D_LCD_PIN_D6)
#define D_LCD_D7       (1<<D_LCD_PIN_D7)
#define D_LCD_RS       (1<<D_LCD_PIN_RS)
#define D_LCD_E        (1<<D_LCD_PIN_E)

/** A TextLCD interface for driving 4-bit HD44780-based LCDs
 *
 * @code
 * #include "TextLCD.h"
 *
 * TextLCD_SPI lcd(PIN_SER, PIN_SCK, PIN_RCK, TextLCD::LCD16x2);
 *
 * int main() {
 *   lcd.printf("Hello World!\n");
 * }
 * @endcode
 */

//The TextLCD_Config.h file selects hardware interface options to reduce memory footprint
//and provides Pin Defines for I2C PCF8574/PCF8574A or MCP23008 and SPI 74595 bus expander interfaces.
//The LCD and serial portexpanders should be wired accordingly.

/* LCD Type information on Rows, Columns and Variant. This information is encoded in
 * an int and used for the LCDType enumerators in order to simplify code maintenance */
// Columns encoded in b7..b0
#define LCD_T_COL_MSK  0x000000FF
#define LCD_T_C6       0x00000006
#define LCD_T_C8       0x00000008
#define LCD_T_C10      0x0000000A
#define LCD_T_C12      0x0000000C
#define LCD_T_C16      0x00000010
#define LCD_T_C20      0x00000014
#define LCD_T_C24      0x00000018
#define LCD_T_C32      0x00000020
#define LCD_T_C40      0x00000028

// Rows encoded in b15..b8
#define LCD_T_ROW_MSK  0x0000FF00
#define LCD_T_R1       0x00000100
#define LCD_T_R2       0x00000200
#define LCD_T_R3       0x00000300
#define LCD_T_R4       0x00000400

/* LCD Ctrl information on interface support and features. This information is encoded in
 * an int and used for the LCDCtrl enumerators in order to simplify code maintenance */
// Interface encoded in b31..b24
#define LCD_C_BUS_MSK  0xFF000000
#define LCD_C_PAR      0x01000000  /*Parallel 4 or 8 bit data, E pin, RS pin, RW=GND            */


/** A TextLCD interface for driving 4-bit HD44780-based LCDs
 *
 * @brief Currently supports 8x1, 8x2, 12x2, 12x3, 12x4, 16x1, 16x2, 16x3, 16x4, 20x2, 20x4, 24x2, 24x4, 40x2 and 40x4 panels
 *        Interface options include direct mbed pins, I2C portexpander (PCF8474/PCF8574A or MCP23008) or SPI bus shiftregister (74595) and some native I2C or SPI devices
 *
 */
class TextLCD_Base{
public:

    /** LCD panel format */
    // The commented out types exist but have not yet been tested with the library
    enum LCDType {
        LCD16x2    = (LCD_T_C16 | LCD_T_R2),    /**< 16x2 LCD panel (default) */
        LCD20x4    = (LCD_T_C20 | LCD_T_R4),    /**< 20x4 LCD panel */
    };

    /** LCD Controller Device */
    enum LCDCtrl {
        HD44780     =  0 |  LCD_C_PAR,                                                                    /**<  HD44780 or full equivalent (default)         */
    };

    /** LCD Cursor control */
    enum LCDCursor {
        CurOff_BlkOff = 0x00,  /**<  Cursor Off, Blinking Char Off */
        CurOn_BlkOff  = 0x02,  /**<  Cursor On, Blinking Char Off */
        CurOff_BlkOn  = 0x01,  /**<  Cursor Off, Blinking Char On */
        CurOn_BlkOn   = 0x03   /**<  Cursor On, Blinking Char On */
    };

    /** LCD Display control */
    enum LCDMode {
        DispOff = 0x00,  /**<  Display Off */
        DispOn  = 0x04   /**<  Display On */
    };

   /** Write a character to the LCD
     *
     * @param c The character to write to the display
     */
   int putc(int c);

    /** Write a raw string to the LCD
     *
     * @param string text, may be followed by variables to emulate formatting the string.
     *                     However, printf formatting is NOT supported and variables will be ignored!
     */
    int printf(const char* text, ...);

    /** Locate cursor to a screen column and row
     *
     * @param column  The horizontal position from the left, indexed from 0
     * @param row     The vertical position from the top, indexed from 0
     */
    void locate(int column, int row);

    /** Return the memoryaddress of screen column and row location
     *
     * @param column  The horizontal position from the left, indexed from 0
     * @param row     The vertical position from the top, indexed from 0
     * @param return  The memoryaddress of screen column and row location
     */
    int  getAddress(int column, int row);

    /** Set the memoryaddress of screen column and row location
     *
     * @param column  The horizontal position from the left, indexed from 0
     * @param row     The vertical position from the top, indexed from 0
     */
    void setAddress(int column, int row);

    /** Clear the screen and locate to 0,0
     */
    void cls();

    /** Return the number of rows
     *
     * @param return  The number of rows
     */
    int rows();

    /** Return the number of columns
     *
     * @param return  The number of columns
     */
    int columns();

    /** Set the Cursormode
     *
     * @param cursorMode  The Cursor mode (CurOff_BlkOff, CurOn_BlkOff, CurOff_BlkOn, CurOn_BlkOn)
     */
    void setCursor(LCDCursor cursorMode);

    /** Set the Displaymode
     *
     * @param displayMode The Display mode (DispOff, DispOn)
     */
    void setMode(LCDMode displayMode);

protected:

    /** Create a TextLCD_Base interface
      * @brief Base class, can not be instantiated
      *
      * @param type  Sets the panel size/addressing mode (default = LCD16x2)
      * @param ctrl  LCD controller (default = HD44780)
      */
    TextLCD_Base(LCDType type = LCD16x2, LCDCtrl ctrl = HD44780);

	/** Write a character to the LCD
	  *
	  * @param value The character to write to the display
	  */
    virtual int _putc(int value);
	/** Get character
	  *
	  */
    virtual int _getc();

/** Low level method for LCD controller
  */
    void _init();

/** Low level initialisation method for LCD controller
  */
    void _initCtrl();

/** Low level character address set method
  */
    int  _address(int column, int row);

/** Low level cursor enable or disable method
  */
    void _setCursor(LCDCursor show);

/** Low level method to restore the cursortype and display mode for current controller
  */
    void _setCursorAndDisplayMode(LCDMode displayMode, LCDCursor cursorType);

/** Low level nibble write operation to LCD controller (serial or parallel)
  */
    void _writeNibble(int value);

/** Low level command byte write operation to LCD controller.
  * Methods resets the RS bit and provides the required timing for the command.
  */
    void _writeCommand(int command);

/** Low level data byte write operation to LCD controller (serial or parallel).
  * Methods sets the RS bit and provides the required timing for the data.
  */
    void _writeData(int data);

/** Pure Virtual Low level writes to LCD Bus (serial or parallel)
  * Set the Enable pin.
  */
    virtual void _setEnable(bool value) = 0;

/** Pure Virtual Low level writes to LCD Bus (serial or parallel)
  * Set the RS pin ( 0 = Command, 1 = Data).
  */
    virtual void _setRS(bool value) = 0;

/** Pure Virtual Low level writes to LCD Bus (serial or parallel)
  * Set the databus value (4 bit).
  */
    virtual void _setData(int value) = 0;

/** Low level byte write operation to LCD controller (serial or parallel)
  * Depending on the RS pin this byte will be interpreted as data or command
  */
    virtual void _writeByte(int value);

//Display type
    LCDType _type;
    int _nr_cols;
    int _nr_rows;

//Display mode
    LCDMode _currentMode;

//Controller type
    LCDCtrl _ctrl;

// Cursor
    int _column;
    int _row;
    LCDCursor _currentCursor;

// Function mode saved to allow switch between Instruction sets after initialisation time
    int _function;

};

//--------- End TextLCD_Base -----------


//--------- Start TextLCD_SPI -----------
/** Create a TextLCD interface using an SPI 74595 portexpander
  *
  */
class TextLCD_SPI : public TextLCD_Base {
public:
    /** Create a TextLCD interface using an SPI 74595 portexpander
     *
     * @param ser             serial data in pin
     * @param sck             serial in clock pin (positive edge)
     * @param rck             parallel out clock pin (positive edge)
     * @param ser             serial data in pin
     * @param type            Sets the panel size/addressing mode (default = LCD16x2)
     * @param ctrl            LCD controller (default = HD44780)
     */
    TextLCD_SPI(Pin ser, Pin sck, Pin rck, LCDType type = LCD16x2, LCDCtrl ctrl = HD44780);

private:

/** Implementation of pure Virtual Low level writes to LCD Bus (serial expander)
  * Set the Enable pin.
  */
    virtual void _setEnable(bool value);

/** Implementation of pure Virtual Low level writes to LCD Bus (serial expander)
  * Set the RS pin (0 = Command, 1 = Data).
  */
    virtual void _setRS(bool value);

/** Implementation of pure Virtual Low level writes to LCD Bus (serial expander)
  * Set the databus value (4 bit).
  */
    virtual void _setData(int value);

/** Low level writes to LCD serial bus only (serial expander)
  */
    void _writeBus();

// SPI bus
    Pin _ser;
		Pin _sck;
		Pin _rck;

// Internal bus mirror value for serial bus only
    char _lcd_bus;
};
//---------- End TextLCD_SPI ------------

#endif
