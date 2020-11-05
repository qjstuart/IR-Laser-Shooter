#include "util.h"

void print_second_line(char *line)
{
	//set cursor on second line
	lcd_set_cursor(0,1);
	//clear lcd
	lcd_print("                ");
	//set cursor on second line
	lcd_set_cursor(0,1);
	//print new
	lcd_print(line);
}
