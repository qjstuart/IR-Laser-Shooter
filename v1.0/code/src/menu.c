#include "menu.h"

int menu_option; /* stores the menu option */

uint32_t *read_buffer;

void menu_init(void) {
	menu_option = -1;
	read_buffer = (uint32_t*) malloc(sizeof(uint32_t));
	switches_init();
	EEPROM_Init();
	gamemodes_init();
}

void show_menu() {
	char display[16];
	 //set cursor to second line
   lcd_set_cursor(0, 1);
	
	 //if switch is up
   if (switch_get(P_SW_UP)) 
	 {
		 //print high scores
     print_second_line("High Scores");
		 menu_option = 1;
   } else if (switch_get(P_SW_CR)) {
		 //on menu option click
		 switch(menu_option){
			 case 1:
			 {
				 
					EEPROM_Read(0,0, read_buffer, MODE_32_BIT, sizeof(read_buffer));
					sprintf(display, "High score: %d", *read_buffer);
					print_second_line(display);
				 
			 };break;
			 case 2:
			 {
				 training_mode();
				 menu_option = -1;
			 };break;
			 case 3:
			 {
				 players_2_mode();
				 menu_option = -1;
			 };break;
			 default: 
			 {
					print_second_line("Please choose an option");
			 };break;
		 }
     lcd_set_cursor(0,1);
   } else if (switch_get(P_SW_DN)) {
			print_second_line("2 Player");
			menu_option = 3;
   } else if (switch_get(P_SW_LT)) {
			print_second_line("Training Mode");
			menu_option = 2;
   } else if (switch_get(P_SW_RT)) {
   }
}
