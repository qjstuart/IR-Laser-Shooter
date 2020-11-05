#include "Gpio.h"

#define PCONP_PCGPIO    (1UL<<15)

uint32_t IRQ_status;
uint32_t IRQ_pin_index;

static void (*GPIO_callback)(uint32_t status);

void gpio_toggle(Pin pin) {
	
	LPC_GPIO_TypeDef* p = GET_GPIO_PORT(pin);
	uint32_t pin_index = GET_PIN_INDEX(pin);	
	short state = gpio_get(pin);
	if (state == 1){p->CLR |=  (1u << pin_index);}
	else{p->SET |=  (1u << pin_index);}

}

void gpio_set(Pin pin, int value) {
	
	LPC_GPIO_TypeDef* p = GET_GPIO_PORT(pin);
	uint32_t pin_index = GET_PIN_INDEX(pin);
	
		if (value == 1){p->SET |=  (1u << pin_index);}
		else {p->CLR |=  (1u << pin_index);}
		
}

int gpio_get(Pin pin) {
	
	LPC_GPIO_TypeDef* p = GET_GPIO_PORT(pin);
	uint32_t pin_index = GET_PIN_INDEX(pin);
	return (p->PIN >> pin_index) & 0x1;

}

void gpio_set_range(Pin pin_base, int count, int value) {
	
	LPC_GPIO_TypeDef* p = GET_GPIO_PORT(pin_base);
	uint32_t pin_index = GET_PIN_INDEX(pin_base);
  
	p->SET |=  (value << pin_index);
	p->CLR |=  ~(value << pin_index);
	
}

unsigned int gpio_get_range(Pin pin_base, int count) {
	
	LPC_GPIO_TypeDef* p = GET_GPIO_PORT(pin_base);
	uint32_t pin_index = GET_PIN_INDEX(pin_base);
	return ((p->PIN >> pin_index) & ((1 << count) - 1));

}

void gpio_set_mode(Pin pin, PinMode mode) {
	
	LPC_GPIO_TypeDef* p = GET_GPIO_PORT(pin);
	uint32_t pin_index = GET_PIN_INDEX(pin);
	uint32_t* pIOCON = GET_IOCON(pin);
	
	LPC_SC->PCONP |= PCONP_PCGPIO;  //Power/clock control gpio pheriperals
	
	switch(mode) {
		case Reset:
			LPC_SC->RSTCON0 |= (1UL << 15);
			break;
		case Input:
			p->DIR &= ~(1 << pin_index); // Set as input.
			*pIOCON &= ~(3UL << 0 ); // Function 0 = GPIO
			break;
		case Output:
			p->DIR |= (1 << pin_index); // Set as output.
			*pIOCON &= ~(3UL << 0 ); // Function 0 = GPIO
			break;
		case PullUp:
			p->DIR &= ~(1 << pin_index); // Set as input.
			*pIOCON &= ~(3UL << 0 ); // Function 0 = GPIO
			*pIOCON |= (1UL << 4 ); // Enable pull-up resistor.
		  *pIOCON &= ~(1UL << 3 ); 
			break;
		case PullDown:
			p->DIR &= ~(1 << pin_index); // Set as input.
			*pIOCON &= ~(3UL << 0 ); // Function 0 = GPIO
			*pIOCON |= (1UL << 3 ); // Enable pull-down resistor.
		  *pIOCON &= ~(1UL << 4 ); 
			break;
	}

}

void gpio_set_trigger(Pin pin, TriggerMode trig) {
		
	uint32_t pin_index = GET_PIN_INDEX(pin);
	
	if(GET_PORT_INDEX(pin) == 0){
	  switch(trig) {
		  case None:
				LPC_GPIOINT ->IO0IntEnR &= ~(1UL << pin_index);
			  LPC_GPIOINT ->IO0IntEnF &= ~(1UL << pin_index);
			  break;
		  case Rising:
			  LPC_GPIOINT ->IO0IntEnR |= (1UL << pin_index);
			  break;
		  case Falling:
			  LPC_GPIOINT ->IO0IntEnF |= (1UL << pin_index);
			  break;
	  }	
	} else if(GET_PORT_INDEX(pin) == 2){
	  switch(trig) {
		  case None:
				LPC_GPIOINT ->IO2IntEnR &= ~(1UL << pin_index);
			  LPC_GPIOINT ->IO2IntEnF &= ~(1UL << pin_index);
			  break;
		  case Rising:
			  LPC_GPIOINT ->IO2IntEnR |= (1UL << pin_index);
			  break;
		  case Falling:
			  LPC_GPIOINT ->IO2IntEnF |= (1UL << pin_index);
			  break;
	  }
	}	else{
			while(1);
		}

}


void gpio_set_callback(Pin pin, void (*callback)(uint32_t status)) {
	
  IRQ_status = 0;
	// Mark Pins that can trigger an Interrupt.
  IRQ_pin_index |= (1UL << GET_PIN_INDEX(pin));
	GPIO_callback = callback;
	
	// Enable Interrupts
	NVIC_SetPriority(GPIO_IRQn, 3);
	NVIC_ClearPendingIRQ(GPIO_IRQn);
  NVIC_EnableIRQ(GPIO_IRQn);
	__enable_irq();

}

/*!
 * 	\brief Passes the pins that caused interrupts to the specified 
 * 				 callback function on port 0 and port 2.
 * 
 * 	Modified to send an unsigned 32 bit integer with all the pins 
 * 	that caused the interrupts. Seperate unsigned 32 bit interger
 * 	will be sent for both port 0 and port 2.
 */
void GPIO_IRQHandler(void) {

	// Get all pins that requested an interrupt on Port 0.
	IRQ_status = (LPC_GPIOINT -> IO0IntStatR | LPC_GPIOINT -> IO0IntStatF);
	// Check if those interrupts can acually trigger an interrupt.
	IRQ_status &= IRQ_pin_index; 
	if (IRQ_status > 0) {
		GPIO_callback(IRQ_status);
		// Clear interrupt flag, since all status are sent at once all bits need to be cleared.	
		LPC_GPIOINT -> IO0IntClr = IRQ_status;
	}
	// Get all pins that requested an interrupt on Port 2.
	IRQ_status = (LPC_GPIOINT -> IO2IntStatR | LPC_GPIOINT -> IO2IntStatF);
	IRQ_status &= IRQ_pin_index; 
	if (IRQ_status > 0) {
		GPIO_callback(IRQ_status);
		// Clear interrupt flag, since all status are sent at once all bits need to be cleared.	
		LPC_GPIOINT -> IO2IntClr = IRQ_status;
	}
}
// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************   
