#include <platform.h>
#include <gpio.h>
#include "rtc.h"
#include "delay.h"
#include "lcd.h"
#include "leds.h"
#include "eeprom.h"
#include "game_modes.h"
#include "shiftregister.h"
#include "piezo.h"
#include "ir.h"
#include "menu.h"
#include <stdlib.h>

/*! 
 *  \brief Method to set EEPROM on game start, if there is no highscore set, set to 0
 */
void setEEPROM_start()
{
	char* high_score_holder;
	uint32_t *read_buffer = (uint32_t*) malloc(sizeof(uint32_t));
	EEPROM_Read(0,0, read_buffer, MODE_32_BIT, sizeof(read_buffer));
	EEPROM_Erase(0);
	
	high_score_holder = (char *)read_buffer;	
	
	if(high_score_holder[0] == NULL)
	{
		
		EEPROM_Erase(0);
		save_high_score(0);
	}
		
}

int main(void) {

	/*!
	 * Initialisations
	 */	
	lcd_init();
	register_init();
	menu_init();
	ir_init();
	piezo_init();
	
	gpio_set(P_PIEZO, 0);
	lcd_print("IR Shooting game");
	lcd_set_cursor(0,1);
	lcd_print("                 ");
	
  while (1) 
	{
		show_menu();
  }
} 
