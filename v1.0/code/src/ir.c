#include "ir.h"


void ir_init(void) {
	gpio_set_mode(P_IR_REC, PullUp);
}

void read_IR()
{
	//if the IR pin returns a 0, means that a hit is registered
	if(gpio_get(P_IR_REC) == 0)
	{
		//clear display
		lcd_clear();
		//light up LED
		gpio_set(P_LED_G, 1);
		//print to LCD
		lcd_print("Target hit!");
		//delay
		delay_ms(1000);
		delay_ms(5);
	}
	//if not hit
	else
	{
		//clear LCD
		lcd_clear();
		//print not hit to LCD
		lcd_print("Not hit");
		//delay
		delay_ms(5);
	}
}
