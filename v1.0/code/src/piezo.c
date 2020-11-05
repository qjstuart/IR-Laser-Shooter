#include "piezo.h"

void piezo_init(void) {
	gpio_set_mode(P_PIEZO, Output);
	gpio_set(P_PIEZO, 0);
}

void piezo_play()
{
	gpio_set(P_PIEZO, 1);
	delay_ms(DURATION);
	gpio_set(P_PIEZO, 0);
}
