
#include "shiftregister.h"


typedef uint8_t byte;
byte* register_state;

void register_init(void)
{
	int j; /*! Counter for loop */
	
	//set latch, data and clock pins as output
	gpio_set_mode(P_LATCH, Output);
	gpio_set_mode(P_CLOCK, Output);
	gpio_set_mode(P_DATA, Output);
	
	
	//Initialize array
	register_state = (byte*)malloc(2 * sizeof(int));
  for (j = 0; j < NUM_OF_REGISTERS; j++) {
    register_state[j] = 0;
  }
}

void shift_reg_output(uint8_t bit_order, uint8_t value)
{
     uint8_t i; /*! Counter for loop */

     for (i = 0; i < 8; i++)  {
			 if (bit_order == LSB)
					gpio_set(P_DATA, !!(value & (1 << i)));
			 else      
					gpio_set(P_DATA, !!(value & (1 << (7 - i))));

			 delay_us(1);					 
			 gpio_set(P_CLOCK, 1);
			 delay_us(1);
			 gpio_set(P_CLOCK, 0);            
     }
}

void reg_write(int output_index, bool state){
	
	int i;
  //Determines register
  int reg = output_index / 8;
  //Determines pin for actual register
  int actualPin = output_index - (8 * reg);

  //Begin session
  gpio_set(P_LATCH, 0);
  
  for (i = 0; i < NUM_OF_REGISTERS; i++){
    //Get actual states for register
    byte* states = &register_state[i];

    //Update state
    if (i == reg){
      bitWrite(*states, actualPin, state);
    }
    //Write
    shift_reg_output(MSB, *states);
  }

  //End session
  gpio_set(P_LATCH, 1);
}


void reg_write_multiple(int* outputs, bool* values){
	
	int i; /*! Counter for loop */
	int j; /*! Counter for loop */
	int reg; /*! To hold register's index */
	int actualPin; /*! To store actual pin for actual register */
	int value; /*! To store current value */
	
	byte* states;

  //Begin session
  gpio_set(P_LATCH, 0);
	
  
  for (i = 0; i < NUM_OF_REGISTERS; i++){
		
    //Get actual states for register
    states = &register_state[i];

		//size_t n = sizeof(pins)/sizeof(pins[0]);
		for(j =0; j < 16; j++)
		{
			//Determines register
			reg = outputs[j] / 8;
			//Determines pin for actual register
			actualPin = outputs[j] - (8 * reg);
			value = values[j];
			
			
			//Update state
			if (i == reg){
				bitWrite(*states, actualPin, value);
			}
		}
		
		//Write
		shift_reg_output(MSB, *states);

  }

  //End session
  gpio_set(P_LATCH, 1);
}

void reg_write_TO_TEST(bool* values){
	
	int i; /*! Counter for loop */
	int j; /*! Counter for loop */
	int reg; /*! To hold register's index */
	int actualPin; /*! To store actual pin for actual register */
	int value; /*! To store current value */
	
	byte* states;

  //Begin session
  gpio_set(P_LATCH, 0);
	
  
  for (i = 0; i < NUM_OF_REGISTERS; i++){
		
    //Get actual states for register
    states = &register_state[i];

		//size_t n = sizeof(pins)/sizeof(pins[0]);
		for(j =0; j < 16; j++)
		{
			//Determines register
			reg = j / 8;
			//Determines pin for actual register
			actualPin = j - (8 * reg);
			value = values[j];
			
			
			//Update state
			if (i == reg){
				bitWrite(*states, actualPin, value);
			}
		}
		
			//Write
			shift_reg_output(MSB, *states);

  }

  //End session
  gpio_set(P_LATCH, 1);
}

void effect_test(int speed){
	int pins_array[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	int *pins = (pins_array);
	
	bool value_array[16] = {1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0};
	bool *values = value_array;
	
	reg_write_multiple(pins, values);
	delay_ms(speed);
	values[0] = 0;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	values[8] = 0;
	values[9] = 0;
	values[10] = 0;
	values[11] = 0;
	values[12] = 0;
	values[13] = 0;
	values[14] = 0;
	values[15] = 0;
	reg_write_multiple(pins, values);


	values[0] = 0;
	values[1] = 1;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	values[8] = 1;
	values[9] = 0;
	values[10] = 0;
	values[11] = 1;
	values[12] = 0;
	values[13] = 0;
	values[14] = 0;
	values[15] = 0;
	reg_write_multiple(pins, values);
	delay_ms(speed);
	values[0] = 0;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	values[8] = 0;
	values[9] = 0;
	values[10] = 0;
	values[11] = 0;
	values[12] = 0;
	values[13] = 0;
	values[14] = 0;
	values[15] = 0;
	reg_write_multiple(pins, values);
	
	values[0] = 0;
	values[1] = 0;
	values[2] = 1;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	values[8] = 1;
	values[9] = 0;
	values[10] = 1;
	values[11] = 1;
	values[12] = 0;
	values[13] = 0;
	values[14] = 0;
	values[15] = 0;
	reg_write_multiple(pins, values);
	delay_ms(speed);
	values[0] = 0;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	values[8] = 0;
	values[9] = 0;
	values[10] = 0;
	values[11] = 0;
	values[12] = 0;
	values[13] = 0;
	values[14] = 0;
	values[15] = 0;
	reg_write_multiple(pins, values);
	
	values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	values[8] = 0;
	values[9] = 1;
	values[10] = 1;
	values[11] = 0;
	values[12] = 0;
	values[13] = 0;
	values[14] = 0;
	values[15] = 0;
	reg_write_multiple(pins, values);
	delay_ms(speed);
	values[0] = 0;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	values[8] = 0;
	values[9] = 0;
	values[10] = 0;
	values[11] = 0;
	values[12] = 0;
	values[13] = 0;
	values[14] = 0;
	values[15] = 0;
	reg_write_multiple(pins, values);
	
	values[0] = 0;
	values[1] = 1;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	values[8] = 0;
	values[9] = 1;
	values[10] = 0;
	values[11] = 1;
	values[12] = 0;
	values[13] = 0;
	values[14] = 0;
	values[15] = 0;
	reg_write_multiple(pins, values);
	delay_ms(speed);
	values[0] = 0;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	values[8] = 0;
	values[9] = 0;
	values[10] = 0;
	values[11] = 0;
	values[12] = 0;
	values[13] = 0;
	values[14] = 0;
	values[15] = 0;
	reg_write_multiple(pins, values);
	
	values[0] = 0;
	values[1] = 0;
	values[2] = 1;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	values[8] = 0;
	values[9] = 1;
	values[10] = 1;
	values[11] = 1;
	values[12] = 0;
	values[13] = 0;
	values[14] = 0;
	values[15] = 0;
	reg_write_multiple(pins, values);
	delay_ms(speed);
	values[0] = 0;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	values[8] = 0;
	values[9] = 0;
	values[10] = 0;
	values[11] = 0;
	values[12] = 0;
	values[13] = 0;
	values[14] = 0;
	values[15] = 0;
	reg_write_multiple(pins, values);
	
	values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	values[8] = 1;
	values[9] = 1;
	values[10] = 1;
	values[11] = 0;
	values[12] = 0;
	values[13] = 0;
	values[14] = 0;
	values[15] = 0;
	reg_write_multiple(pins, values);
	delay_ms(speed);
	values[0] = 0;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	values[8] = 0;
	values[9] = 0;
	values[10] = 0;
	values[11] = 0;
	values[12] = 0;
	values[13] = 0;
	values[14] = 0;
	values[15] = 0;
	reg_write_multiple(pins, values);
	
	values[0] = 0;
	values[1] = 1;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	values[8] = 1;
	values[9] = 1;
	values[10] = 0;
	values[11] = 1;
	values[12] = 0;
	values[13] = 0;
	values[14] = 0;
	values[15] = 0;
	reg_write_multiple(pins, values);
	delay_ms(speed);
	values[0] = 0;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	values[8] = 0;
	values[9] = 0;
	values[10] = 0;
	values[11] = 0;
	values[12] = 0;
	values[13] = 0;
	values[14] = 0;
	values[15] = 0;
	reg_write_multiple(pins, values);
	
	values[0] = 0;
	values[1] = 0;
	values[2] = 1;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	values[8] = 1;
	values[9] = 1;
	values[10] = 1;
	values[11] = 1;
	values[12] = 0;
	values[13] = 0;
	values[14] = 0;
	values[15] = 0;
	reg_write_multiple(pins, values);
	delay_ms(speed);
	values[0] = 0;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	values[8] = 0;
	values[9] = 0;
	values[10] = 0;
	values[11] = 0;
	values[12] = 0;
	values[13] = 0;
	values[14] = 0;
	values[15] = 0;
	reg_write_multiple(pins, values);
	
}


void reg_write_multiple_1sr(int* outputs, bool* values){
	
	int j; /*! Counter for loop */
	int reg; /*! To hold register's index */
	int actualPin; /*! To store actual pin for actual register */
	int value; /*! To store current value */
	
	byte* states;

  //Begin session
  gpio_set(P_LATCH, 0);
		
	//Get actual states for register
	states = &register_state[0];

	//size_t n = sizeof(pins)/sizeof(pins[0]);
	for(j =0; j < 8; j++)
	{
		//Determines register
		reg = outputs[j] / 8;
		//Determines pin for actual register
		actualPin = outputs[j] - (8 * reg);
		value = values[j];
		
		
		//Update state
			bitWrite(*states, actualPin, value);
	}
	
	//Write
	shift_reg_output(MSB, *states);

  //End session
  gpio_set(P_LATCH, 1);
}

void effect_test_1sr(int speed){
	int pins_array[8] = {0,1,2,3,4,5,6,7};
	int *pins = (pins_array);
	
	bool value_array[8] = {0,0,0,0,0,1,0,0};
	bool *values = value_array;
	
	reg_write_multiple_1sr(pins, values);
	delay_ms(speed);
	values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	reg_write_multiple_1sr(pins, values);
	
	values[0] = 0;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 1;
	
	values[5] = 0;
	values[6] = 1;
	values[7] = 0;

	reg_write_multiple_1sr(pins, values);
	delay_ms(speed);
	
	values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	
	reg_write_multiple_1sr(pins, values);

	values[0] = 0;
	
	values[1] = 0;
	values[2] = 0;
	values[3] = 1;
	values[4] = 0;
	
	values[5] = 0;
	values[6] = 0;
	values[7] = 1;

	reg_write_multiple_1sr(pins, values);
	delay_ms(speed);
	
		values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	reg_write_multiple_1sr(pins, values);
	
	values[0] = 0;
	
	values[1] = 0;
	values[2] = 0;
	values[3] = 1;
	values[4] = 1;

	values[5] = 1;	
	values[6] = 0;
	values[7] = 0;

	reg_write_multiple_1sr(pins, values);
	delay_ms(speed);
	
		values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	reg_write_multiple_1sr(pins, values);
	
	values[0] = 0;
	
	values[1] = 0;
	values[2] = 1;
	values[3] = 0;
	values[4] = 0;
	
	values[5] = 0;
	values[6] = 1;
	values[7] = 0;

	reg_write_multiple_1sr(pins, values);
	delay_ms(speed);
	
		values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	reg_write_multiple_1sr(pins, values);
	
	values[0] = 0;
	
	values[1] = 0;
	values[2] = 1;
	values[3] = 0;
	values[4] = 1;
	
	values[5] = 0;
	values[6] = 0;
	values[7] = 1;

	reg_write_multiple_1sr(pins, values);
	delay_ms(speed);
		values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	reg_write_multiple_1sr(pins, values);
	
	values[0] = 0;
	
	values[1] = 0;
	values[2] = 1;
	values[3] = 1;
	values[4] = 0;
	
	values[5] = 1;
	values[6] = 0;
	values[7] = 0;

	reg_write_multiple_1sr(pins, values);
	delay_ms(speed);
		values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	reg_write_multiple_1sr(pins, values);
	
	values[0] = 0;
	
	values[1] = 0;
	values[2] = 1;
	values[3] = 1;
	values[4] = 1;
	
	values[5] = 0;
	values[6] = 1;
	values[7] = 0;

	reg_write_multiple_1sr(pins, values);
	delay_ms(speed);
	
		values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	reg_write_multiple_1sr(pins, values);
	
	values[0] = 0;
	
	values[1] = 1;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	
	values[5] = 0;
	values[6] = 0;
	values[7] = 1;

	reg_write_multiple_1sr(pins, values);
	delay_ms(speed);
	
		values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	reg_write_multiple_1sr(pins, values);
	
	values[0] = 0;
	
	values[1] = 1;
	values[2] = 0;
	values[3] = 0;
	values[4] = 1;
	
	values[5] = 1;
	values[6] = 0;
	values[7] = 0;

	reg_write_multiple_1sr(pins, values);
	delay_ms(speed);
	
		values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	reg_write_multiple_1sr(pins, values);
	
	values[0] = 0;
	
	values[1] = 1;
	values[2] = 0;
	values[3] = 1;
	values[4] = 0;
	
	values[5] = 0;
	values[6] = 1;
	values[7] = 0;

	reg_write_multiple_1sr(pins, values);
	delay_ms(speed);
	
		values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	reg_write_multiple_1sr(pins, values);
	
	values[0] = 0;
	
	values[1] = 1;
	values[2] = 0;
	values[3] = 1;
	values[4] = 1;
	
	values[5] = 0;
	values[6] = 0;
	values[7] = 1;
	
	reg_write_multiple_1sr(pins, values);
	delay_ms(speed);
	
		values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	reg_write_multiple_1sr(pins, values);
	
	values[0] = 0;
	
	values[1] = 1;
	values[2] = 1;
	values[3] = 0;
	values[4] = 0;
	
	values[5] = 1;
	values[6] = 0;
	values[7] = 0;

	reg_write_multiple_1sr(pins, values);
	delay_ms(speed);
	
		values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	reg_write_multiple_1sr(pins, values);
	
	values[0] = 0;
	
	values[1] = 1;
	values[2] = 1;
	values[3] = 0;
	values[4] = 1;
	
	values[5] = 0;
	values[6] = 1;
	values[7] = 0;

	reg_write_multiple_1sr(pins, values);
	delay_ms(speed);
	
		values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	reg_write_multiple_1sr(pins, values);
	values[0] = 0;
	
	values[1] = 1;
	values[2] = 1;
	values[3] = 1;
	values[4] = 0;
	
	values[5] = 0;
	values[6] = 0;
	values[7] = 1;

	reg_write_multiple_1sr(pins, values);
	delay_ms(speed);
	
		values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	reg_write_multiple_1sr(pins, values);
	
	
	values[0] = 0;
	
	values[1] = 1;
	values[2] = 1;
	values[3] = 1;
	values[4] = 1;
	
	values[5] = 1;
	values[6] = 0;
	values[7] = 0;

	reg_write_multiple_1sr(pins, values);
	delay_ms(speed);
	

	values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	reg_write_multiple_1sr(pins, values);
	
}


void effect_test_1sr_4_cells(int speed){
	int i;
	int pins_array[8] = {0,1,2,3,4,5,6,7};
	int *pins = (pins_array);
	
	bool value_array[8] = {0,0,0,0,0,0,0,0};
	bool *values = value_array;
	
	int val5 = 0;
	int val6 = 0;
	int val7 = 0;
	
	for(i =0; i < 3; i++)
	{
		if(i ==0)
		{
			val5 = 1;
			val6 = 0;
			val7 = 0;
		}
		else if(i ==1)
		{
			val5 = 0;
			val6 = 1;
			val7 = 0;
		}
		else
		{
			val5 = 0;
			val6 = 0;
			val7 = 1;
		}
	
		
		values[0] = 0;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = val5;	
	values[6] = val6;
	values[7] = val7;
	
	reg_write_multiple_1sr(pins, values);
	delay_ms(speed);
	values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	reg_write_multiple_1sr(pins, values);
	
	values[0] = 0;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 1;
	
	values[5] = val5;	
	values[6] = val6;
	values[7] = val7;
	reg_write_multiple_1sr(pins, values);
	delay_ms(speed);
	
	values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	
	reg_write_multiple_1sr(pins, values);

	values[0] = 0;
	
	values[1] = 0;
	values[2] = 0;
	values[3] = 1;
	values[4] = 0;
	
	values[5] = val5;	
	values[6] = val6;
	values[7] = val7;
	reg_write_multiple_1sr(pins, values);
	delay_ms(speed);
	
	values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	reg_write_multiple_1sr(pins, values);
	
	values[0] = 0;
	
	values[1] = 0;
	values[2] = 0;
	values[3] = 1;
	values[4] = 1;

	values[5] = val5;	
	values[6] = val6;
	values[7] = val7;
	reg_write_multiple_1sr(pins, values);
	delay_ms(speed);
	
		values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	reg_write_multiple_1sr(pins, values);
	
	values[0] = 0;
	
	values[1] = 0;
	values[2] = 1;
	values[3] = 0;
	values[4] = 0;

	values[5] = val5;	
	values[6] = val6;
	values[7] = val7;
	reg_write_multiple_1sr(pins, values);
	delay_ms(speed);
	
		values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	reg_write_multiple_1sr(pins, values);
	
	values[0] = 0;
	
	values[1] = 0;
	values[2] = 1;
	values[3] = 0;
	values[4] = 1;

	values[5] = val5;	
	values[6] = val6;
	values[7] = val7;
	
	reg_write_multiple_1sr(pins, values);
	delay_ms(speed);
	
		values[0] = 1;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	reg_write_multiple_1sr(pins, values);
	
	}
	
	
	
}
