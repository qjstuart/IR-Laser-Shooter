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
#include "TextLCD.h"
#include <platform.h>
#include <gpio.h>
#include "delay.h"

/** Create a TextLCD_Base interface
  *
  * @param type  Sets the panel size/addressing mode (default = LCD16x2)
  * @param ctrl  LCD controller (default = HD44780)
  */
TextLCD_Base::TextLCD_Base(LCDType type, LCDCtrl ctrl) : _type(type), _ctrl(ctrl) {

  // Extract LCDType data

  // Columns encoded in b7..b0
  _nr_cols = (_type & 0xFF);

  // Rows encoded in b15..b8
  _nr_rows = ((_type >> 8) & 0xFF);
}


/**  Init the LCD Controller(s)
  *  Clear display
  */
void TextLCD_Base::_init() {

  // Select and configure primary LCD controller
  _initCtrl();                   // Init primary controller

  // Clear whole display and Reset Cursor location
  // Note: This will make sure that some 3-line displays that skip topline of a 4-line configuration
  //       are cleared and init cursor correctly.
  cls();
}

/**  Init the LCD controller
  *  4-bit mode, number of lines, fonttype, no cursor etc
  *
  *  Note: some configurations are commented out because they have not yet been tested due to lack of hardware
  */
void TextLCD_Base::_initCtrl() {
    this->_setRS(false); // command mode

    delay_ms(20);         // Wait 20ms to ensure powered up

    // The Controller could be in 8 bit mode (power-on reset) or in 4 bit mode (warm reboot) at this point.
    // Follow this procedure to make sure the Controller enters the correct state. The hardware interface
    // between the uP and the LCD can only write the 4 most significant bits (Most Significant Nibble, MSN).
    // In 4 bit mode the LCD expects the MSN first, followed by the LSN.
    //
    //    Current state:               8 bit mode                |  4 bit mode, MSN is next      | 4 bit mode, LSN is next
                         //-------------------------------------------------------------------------------------------------
    _writeNibble(0x3);   //  set 8 bit mode (MSN) and dummy LSN, |   set 8 bit mode (MSN),       |    set dummy LSN,
                         //  remains in 8 bit mode               |    change to 8 bit mode       |  remains in 4 bit mode
    delay_ms(15);         //

    _writeNibble(0x3);   //  set 8 bit mode and dummy LSN,       | set 8 bit mode and dummy LSN, |    set 8bit mode (MSN),
                         //  remains in 8 bit mode               |   remains in 8 bit mode       |  remains in 4 bit mode
    delay_ms(15);         //

    _writeNibble(0x3);   //  set 8 bit mode and dummy LSN,       | set 8 bit mode and dummy LSN, |    set dummy LSN,
                         //  remains in 8 bit mode               |   remains in 8 bit mode       |  change to 8 bit mode
    delay_ms(15);         //

    // Controller is now in 8 bit mode

    _writeNibble(0x2);   // Change to 4-bit mode (MSN), the LSN is undefined dummy
    delay_us(40);         // most instructions take 40us

    // Display is now in 4-bit mode
    // Note: 4/8 bit mode is ignored for most native SPI and I2C devices. They dont use the parallel bus.
    //       However, _writeNibble() method is void anyway for native SPI and I2C devices.

    // Devices fully compatible to HD44780 that do not use any DC/DC Voltage converters but external VLCD, no icons etc
    // Initialised as 2 Line displays (including LCD16x1C and LCD40x4)
    _function = 0x08;    // Function set 001 DL N F - -
                         //  DL=0 (4 bits bus)
                         //  Note: 4 bit mode is ignored for native SPI and I2C devices
                         //   N=1 (2 lines)
                         //   F=0 (5x7 dots font, only option for 2 line display)
                         //    -  (Don't care)
    _writeCommand(0x20 | _function);

    // Controller general initialisations
    _writeCommand(0x02); // Return Home
                         //   Cursor Home, DDRAM Address to Origin

    _writeCommand(0x06); // Entry Mode 0000 0 1 I/D S
                         //   Cursor Direction and Display Shift
                         //   I/D=1 (Cur incr)
                         //     S=0 (No display shift)

    _writeCommand(0x14); // Cursor or Display shift 0001 S/C R/L x x
                         //   S/C=0 Cursor moves
                         //   R/L=1 Right
                         //

    setCursor(CurOff_BlkOff);
    setMode(DispOn);
}


/** Clear the screen, Cursor home.
  */
void TextLCD_Base::cls() {

  // Primary LCD controller Clearscreen
  _writeCommand(0x01);    // cls, and set cursor to 0
  delay_ms(10);            // The CLS command takes 1.64 ms.
                          // Since we are not using the Busy flag, Lets be safe and take 10 ms
  setAddress(0, 0);  // Reset Cursor location
                     // Note: This is needed because some displays (eg PCF21XX) don't use line 0 in the '3 Line' mode.
}

/** Locate cursor to a screen column and row
  *
  * @param column  The horizontal position from the left, indexed from 0
  * @param row     The vertical position from the top, indexed from 0
  */
void TextLCD_Base::locate(int column, int row) {

   // setAddress() does all the heavy lifting:
   //   check column and row sanity,
   //   switch controllers for LCD40x4 if needed
   //   switch cursor for LCD40x4 if needed
   //   set the new memory address to show cursor at correct location
   setAddress(column, row);
}


/** Write a single character (Stream implementation)
  */
int TextLCD_Base::_putc(int value) {
  int addr;

    if (value == '\n') {
      //No character to write

      //Update Cursor
      _column = 0;
      _row++;
      if (_row >= rows()) {
        _row = 0;
      }
    }
    else {
      //Character to write
      _writeData(value);

      //Update Cursor
      _column++;
      if (_column >= columns()) {
        _column = 0;
        _row++;
        if (_row >= rows()) {
          _row = 0;
        }
      }
    } //else

    //Set next memoryaddress, make sure cursor blinks at next location
    addr = getAddress(_column, _row);
    _writeCommand(0x80 | addr);

    return value;
}


/** Write a character to the LCD
  *
  * @param c The character to write to the display
  */
int TextLCD_Base::putc(int c){
  return _putc(c);
}

/** Write a raw string to the LCD
  *
  * @param string text, may be followed by variables to emulate formatting the string.
  *                     However, printf formatting is NOT supported and variables will be ignored!
  */
int TextLCD_Base::printf(const char* text, ...) {

  while (*text !=0) {
    _putc(*text);
    text++;
  }
  return 0;
}


// Write a nibble using the 4-bit interface
void TextLCD_Base::_writeNibble(int value) {

// Enable is Low
    this->_setEnable(true);
    this->_setData(value & 0x0F);   // Low nibble
    delay_us(1); // Data setup time
    this->_setEnable(false);
    delay_us(1); // Datahold time

// Enable is Low
}

// Write a byte using the 4-bit interface
void TextLCD_Base::_writeByte(int value) {

// Enable is Low
    this->_setEnable(true);
    this->_setData(value >> 4);   // High nibble
    delay_us(1); // Data setup time
    this->_setEnable(false);
    delay_us(1); // Data hold time

    this->_setEnable(true);
    this->_setData(value >> 0);   // Low nibble
    delay_us(1); // Data setup time
    this->_setEnable(false);
    delay_us(1); // Datahold time

// Enable is Low
}

// Write a command byte to the LCD controller
void TextLCD_Base::_writeCommand(int command) {

    this->_setRS(false);
    delay_us(1);  // Data setup time for RS

    this->_writeByte(command);
    delay_us(40); // most instructions take 40us
}

// Write a data byte to the LCD controller
void TextLCD_Base::_writeData(int data) {

    this->_setRS(true);
    delay_us(1);  // Data setup time for RS

    this->_writeByte(data);
    delay_us(40); // data writes take 40us
}

// This is new method to return the memory address based on row, column and displaytype.
//
/** Return the memoryaddress of screen column and row location
   *
   * @param column  The horizontal position from the left, indexed from 0
   * @param row     The vertical position from the top, indexed from 0
   * @param return  The memoryaddress of screen column and row location
   *
   */
int TextLCD_Base::getAddress(int column, int row) {
  //Default addressing mode for 1, 2 and 4 rows (except 40x4)
  //The two available rows are split and stacked on top of
  //eachother. Addressing for 3rd and 4th line continues where lines
  //1 and 2 were split.
  //Displays top rows when less than four are used.
  switch (row) {
    case 0:
      return 0x00 + column;
    case 1:
      return 0x40 + column;
    case 2:
      return 0x00 + _nr_cols + column;
    case 3:
      return 0x40 + _nr_cols + column;
    // Should never get here.
    default:
      return 0x00;
    }
}


/** Set the memoryaddress of screen column and row location
  *
  * @param column  The horizontal position from the left, indexed from 0
  * @param row     The vertical position from the top, indexed from 0
  */
void TextLCD_Base::setAddress(int column, int row) {

// Sanity Check column
    if (column < 0) {
      _column = 0;
    }
    else if (column >= columns()) {
      _column = columns() - 1;
    } else _column = column;

// Sanity Check row
    if (row < 0) {
      _row = 0;
    }
    else if (row >= rows()) {
      _row = rows() - 1;
    } else _row = row;


// Compute the memory address
// For LCD40x4:  switch controllers if needed
//               switch cursor if needed
    int addr = getAddress(_column, _row);

    _writeCommand(0x80 | addr);
}


/** Return the number of columns
  *
  * @param return  The number of columns
  *
  * Note: some configurations are commented out because they have not yet been tested due to lack of hardware
  */
int TextLCD_Base::columns() {

  // Columns encoded in b7..b0
  //return (_type & 0xFF);
  return _nr_cols;
}

/** Return the number of rows
  *
  * @param return  The number of rows
  *
  * Note: some configurations are commented out because they have not yet been tested due to lack of hardware
  */
int TextLCD_Base::rows() {

  // Rows encoded in b15..b8
  //return ((_type >> 8) & 0xFF);
  return _nr_rows;
}

/** Set the Cursormode
  *
  * @param cursorMode  The Cursor mode (CurOff_BlkOff, CurOn_BlkOff, CurOff_BlkOn, CurOn_BlkOn)
  */
void TextLCD_Base::setCursor(LCDCursor cursorMode) {

  // Save new cursor mode, needed when 2 controllers are in use or when display is switched off/on
  _currentCursor = cursorMode;

  // Configure only current LCD controller
  _setCursorAndDisplayMode(_currentMode, _currentCursor);
}

/** Set the Displaymode
  *
  * @param displayMode The Display mode (DispOff, DispOn)
  */
void TextLCD_Base::setMode(LCDMode displayMode) {

  // Save new displayMode, needed when 2 controllers are in use or when cursor is changed
  _currentMode = displayMode;

  // Configure primary LCD controller
  _setCursorAndDisplayMode(_currentMode, _currentCursor);
}


/** Low level method to restore the cursortype and display mode for current controller
  */
void TextLCD_Base::_setCursorAndDisplayMode(LCDMode displayMode, LCDCursor cursorType) {

    // Configure current LCD controller
    _writeCommand(0x08 | displayMode | cursorType);
}

//--------- End TextLCD_Base -----------


//--------- Start TextLCD_SPI -----------
/* SPI Expander SN74595          */

 /** Create a TextLCD interface using an SPI 74595 portexpander
   *
   * @param spi             SPI Bus
   * @param cs              chip select pin (active low)
   * @param type            Sets the panel size/addressing mode (default = LCD16x2)
   * @param ctrl            LCD controller (default = HD44780)
   */
TextLCD_SPI::TextLCD_SPI(Pin ser, Pin sck, Pin rck, LCDType type, LCDCtrl ctrl) :
                         TextLCD_Base(type, ctrl),
                         _ser(ser),
                         _sck(sck),
                         _rck(rck) {

  // set the relevant pins as output
  gpio_set_mode(_ser, Output);
  gpio_set_mode(_sck, Output);
  gpio_set_mode(_rck, Output);
  // set all pins to low
  gpio_set(_ser, 0);
  gpio_set(_sck, 0);
  gpio_set(_rck, 0);

  // Init the portexpander bus
  _lcd_bus = 0;

  // write the new data to the portexpander
  _writeBus();

  _init();
}

// Set E pin (or E2 pin)
// Used for mbed pins, I2C bus expander or SPI shiftregister
void TextLCD_SPI::_setEnable(bool value) {

	if (value) {
		_lcd_bus |= D_LCD_E;     // Set E bit
	}
	else {
		_lcd_bus &= ~D_LCD_E;    // Reset E bit
	}

  // write the new data to the SPI portexpander
  _writeBus();
}

// Set RS pin
// Used for mbed pins, I2C bus expander or SPI shiftregister
void TextLCD_SPI::_setRS(bool value) {

  if (value) {
    _lcd_bus |= D_LCD_RS;    // Set RS bit
  }
  else {
    _lcd_bus &= ~D_LCD_RS;   // Reset RS bit
  }

  // write the new data to the SPI portexpander
  _writeBus();
}

// Place the 4bit data on the databus
// Used for mbed pins, I2C bus expander or SPI shiftregister
void TextLCD_SPI::_setData(int value) {
  int data;

  // Set bit by bit to support any mapping of expander portpins to LCD pins

  data = value & 0x0F;
  if (data & 0x01) {
    _lcd_bus |= D_LCD_D4;   // Set Databit
  }
  else {
    _lcd_bus &= ~D_LCD_D4;  // Reset Databit
  }

  if (data & 0x02) {
    _lcd_bus |= D_LCD_D5;   // Set Databit
  }
  else {
    _lcd_bus &= ~D_LCD_D5;  // Reset Databit
  }

  if (data & 0x04) {
    _lcd_bus |= D_LCD_D6;   // Set Databit
  }
  else {
    _lcd_bus &= ~D_LCD_D6;  // Reset Databit
  }

  if (data & 0x08) {
    _lcd_bus |= D_LCD_D7;   // Set Databit
  }
  else {
    _lcd_bus &= ~D_LCD_D7;  // Reset Databit
  }

  // write the new data to the SPI portexpander
  _writeBus();
}

// Low level writes to LCD serial bus only (serial expander)
void TextLCD_SPI::_writeBus() {

  uint8_t c = _lcd_bus;

  for (int i = 0; i < 8; i++) { // shift data MSB first.
    if (c & 0x80)
      gpio_set(_ser, 1);
    else
      gpio_set(_ser, 0);
    // clock data into shift register.
    delay_cycles(2);
    gpio_set(_sck, 1);
    delay_cycles(2);
    gpio_set(_sck, 0);
    // shift the next bit
    c = c << 1;
  }
  // clock data to output Latches.
  delay_cycles(2);
  gpio_set(_rck, 1);
  delay_cycles(2);
  gpio_set(_rck, 0);
}

//---------- End TextLCD_SPI ------------
