/*!
 * \file      game_modes.c
 * \brief     Source file which contains methods needed for the game modes.
 */
 
#include "game_modes.h"

/*! 
 * M for LCG
 * \def M 
 */
#define M 2147483647 

/*! 
 * A for LCG 
 * \def A
 */
#define A 1103515245 

/*! 
 * C for LCG  
 * \def C
 */
#define C 12345 

/*! 
 * Cycles per second 
 * \def CYCLES_SEC 
 */
#define CYCLES_SEC 120000000

/*! Flag to check if reset button was pressed */
bool exit_flag= false;
/*! Flag to check if score can be incremented in case of interrupt */
bool can_read = false;
/*! Flag to check if the target is hit */
bool is_hit = false;	
/*! Stores score */
int score = 0; 
/*! Stores score for player1 */
int score_p1 = 0; 
/*! Stores score for player2 */
int score_p2 = 0; 
/*! Flag to be set when score expires */
bool is_expired = false; 
/*! Stores seed */
int seed = 1; 
 /*! Stores real time clock */
rtc_t rtc;
/*! Stores quantity left for red led colour to be shown */
int red = 5;
/*! Stores quantity left for green led colour to be shown */
int green = 3;
/*! Stores quantity left for blue led colour to be shown */
int blue = 2;
/*! Stores current colour
 *	1 - RED
 *	2 - GREEN
 *	3 - BLUE
 */
int current_colour = -1;
/*! Stores current target */
int current_target = -1;

void change_target(void)
{
	is_expired = true;
}

void target_hit()
{
	piezo_play();
	is_hit = true;
}

void switch_target_off(void)
{	
	int pins_array[8] = {0,1,2,3,4,5,6,7};
	int *pins = (pins_array);
	
	bool value_array[8] = {1,0,0,0,0,0,0,0};
	bool *values = value_array;
	
	reg_write_multiple_1sr(pins, values);

}

void gamemodes_init()
{
	is_hit = false;
	timer_set_callback(change_target);
	
	//initialise register
	register_init();
	//initialise rtc
	RTC_Init();
	//initialise eeprom
	EEPROM_Init();
	//initialise piezo
	piezo_init();

	
	//set triggers
	gpio_set_mode(P_INT_BTN, PullUp);
	gpio_set_trigger(P_INT_BTN, Falling);
	gpio_set_mode(P_IR_REC, Input);
	gpio_set_trigger(P_IR_REC, Falling);
	
	RTC_GetDateTime(&rtc);
	seed = (int)rtc.sec + ((int)rtc.min * 60) + ((int)rtc.hour * 60 * 60);
}

int bitExtracted(int number, int index)
{
	return (((1 << 1) - 1) & (number >> (index - 1)));
}

long long convert(int number) 
{
	//set binary to 0
	long long binary = 0;
	//intiialsie remainder, step and index
	int remainder, index = 1;
	
	//while number is not 0, loop
	while (number != 0) 
	{
		//get remiander by using modulus
		remainder = number % 2;
		// add to binary
		binary += index * remainder;
		// multiply index by 10
		index *= 10;
		//half the number
		number /= 2;
	}

	return binary;
}

long long getColour(int index)
{
	//set value to 0
	int value = 0;
	//set bit at index position to 1
	value|= 1UL << index;
	//return the number converted to binary
	return convert(value);
}

int check_colour(int colour)
{
	switch(colour)
	{
		//RED
		case 1 : 
		{

			//if can be chosen
			if(red > 0)
			{

				//decrement
				red--;
				//set current
				current_colour = colour;
				return 0;
			}
			else
			{
				return check_colour(2);
			}
		}
		//BLUE
		case 3 :		
		{

			//if can be chosen
			if(blue > 0)
			{

				//decrement
				blue--;
				//set current
				current_colour = colour;
				return 2;
			}
			else
			{
				return check_colour(1);
			}
		}
		//GREEN
		case 2 :
		{

			//if can be chosen
			if(green > 0)
			{

				//decrement
				green--;
				//set current
				current_colour = colour;
				return 1;
			}
			else
			{
				return check_colour(3);
			}
		}
		default : return check_colour(1);
	}
}

int get_multiplier(int current_colour)
{
	switch(current_colour)
	{
		//RED
		case 1: 
		{
			return 1;
		}
		//GREEN
		case 2 :		
		{
			return 2;
		};		
		//BLUE
		default :
		{
			return 5;
		}
	}
}


int *getMatrixRandom()
{
	//init array to return
	static int bool_array[7];
	long long colour;
	long long matrix_4;
	//generate number
	int number = (A * seed + C) & RAND_MAX;
	
	//get remainder enough to fill 2 bits for cell position (2x2 matrix)
	//for 4x4 matrix once can imply do % 16
	int cell_bits = number % 4;
	//get number 0-2 to get colour from the generated number
	int colour_bits = number % 3;
	seed = number/C;
	
	//if same target change it
	if(cell_bits == current_target)
	{
		if(number<=1)
			cell_bits+=1;
		else
			cell_bits-=1;
	}
	
	//set current target
	current_target = cell_bits;
	
	//get colour bits
	colour_bits = check_colour(colour_bits+1);
	
	//get colour
	colour = getColour(colour_bits);
	//get matrix combo
	matrix_4 = convert(cell_bits);
	
	
	if(bitExtracted(colour, 1) == 0 && bitExtracted(colour, 2) == 0 && bitExtracted(colour, 3) == 0)
	{
			colour |= 1UL << 2;
	}

	//fill array
	//colours first
	bool_array[0] = bitExtracted(colour, 1);
	bool_array[1] = bitExtracted(colour, 2);
	bool_array[2] = bitExtracted(colour, 3);
	//matrix then
	//these are not being used but can be filled in when expanding to 4x4 matrix
	bool_array[3] = 0;
	bool_array[4] = 0;
	//these are the 2 bits used since we are going to demonstrate on 2x2 matrix
	bool_array[5] = bitExtracted(matrix_4, 1);
	bool_array[6] = bitExtracted(matrix_4, 2);

	return bool_array;
}

void set_target()
{
	int pins_array[8] = {0,1,2,3,4,5,6,7};
	int *pins = (pins_array);
	
	int *generated = getMatrixRandom();
	bool value_array[8] = {0,0,0,0,0,0,0,0};
	bool *values = value_array;
	
	reg_write_multiple_1sr(pins, values);
	//enable
	values[0] = 0;
	
	//matrix
	values[1] = generated[3];
	values[2] = generated[4];
	values[3] = generated[5];
	values[4] = generated[6];
	//colour
	values[5] = generated[0];
	values[6] = generated[1];
	values[7] = generated[2];

	reg_write_multiple_1sr(pins, values);
}

void training_mode_isr(uint32_t sources)
{
	//if interrupt is coming from reset btn
	if (sources & (1 << GET_PIN_INDEX(P_INT_BTN))) {
		score = 0;
		exit_flag = true;
	}
	
	//if interrupt is coming from ir receiver
	if(sources & (1 << GET_PIN_INDEX(P_IR_REC)))
	{
		//if interrupt can be read
		if(can_read)
		{
			//turn on piezo
			gpio_set(P_PIEZO, 1);
			is_hit = true;
		}
	}
	
}

void save_high_score(int to_write)
{
	uint32_t *write_buffer;
	int* value;
	*value = to_write;
	write_buffer = (uint32_t*)value;
	
	delay_ms(1000);
	EEPROM_Erase(0);
	EEPROM_Write(0,0, write_buffer, MODE_32_BIT, sizeof(write_buffer));
	
}

void players_2_mode()
{
	int i;
	int j;
	char display[16];
	score =0;
	exit_flag = false;
	can_read = false;
		
	//set interrupt callback
	gpio_set_callback(P_IR_REC, training_mode_isr);
	gpio_set_callback(P_INT_BTN, training_mode_isr);
	
	//2 players
	for(j=0; j < 2; j++)
	{
		//reset colours
		red = 5;
		green = 3;
		blue = 2;
		
		sprintf(display, "Player %d START", (j+1));
		print_second_line(display);
		
		delay_ms(1000);
		
		//coundown
		for(i=5;i>0;i--)
		{
			//print countdown
			sprintf(display, "STARTING IN: %d", i);
			print_second_line(display);
			//delay
			delay_ms(1000);
		}
		
		sprintf(display, "score: %d", score);
		print_second_line(display);
		
		//enable ir
		can_read = true;
	
		//10 rounds
		for(i=0; i < 10; i++)
		{
			//set is_hit and is_expired to false
			is_hit = false;
			is_expired = false;
			
			//disable timer for reinit
			timer_disable();
			
			//turn off piezo
			gpio_set(P_PIEZO, 0);
			
			//change matrix cell
			can_read = false;
			delay_ms(200);
			set_target();
			delay_ms(200);
			can_read = true;
			
			//start timer with 2 seconds
			timer_init((uint32_t)2*CYCLES_SEC);
			//enable timer
			timer_enable();
			
			
			//continuously check for the flag
			while(1)
			{
				//if reset is pressed
				if(exit_flag)
				{
					return;
				}
				//check if is hit
				if(is_hit)
				{
					//increment score for player
					if(j==0)
						score_p1+=get_multiplier(current_colour);
					else
						score_p2+=get_multiplier(current_colour);
					
					//show score
					if(j==0)
						sprintf(display, "score: %d", score_p1);
					else
						sprintf(display, "score: %d", score_p2);
					print_second_line(display);
					
					break;
						
				}
				//if time expired
				else if(is_expired)
				{
					break;
				}
			}
			
			switch_target_off();
			//delay between change target
			
			delay_ms(500);
		}
		
		//turn off piezo
		gpio_set(P_PIEZO, 0);
		//disable ir
		can_read = false;
		
		//if player 1
		if(j==0)
		{
			print_second_line("Player 2's turn");
			delay_ms(3000);
		}
		
	}

	
	//output game finished
	print_second_line("Game finished");
	
	delay_ms(1000);
	
	//player 1 winner
	if(score_p1 > score_p2)
	{
		//output game result
		print_second_line("Player 1 WINNER");
	}
	//if player 2 winner
	else if(score_p1 < score_p2)
	{
		//output game result
		print_second_line("Player 2 WINNER");
	}
	//if its a draw
	else
	{
		//output game result
		print_second_line("It's a DRAW!");
	}
	
	return;
}

void training_mode()
{
	//variable for loop
	int i;
	//variable for display
	char display[16];
	//buffer
	uint32_t *buffer = (uint32_t*) malloc(sizeof(uint32_t));
	//variable for score
	score =0;
	//exit flag
	exit_flag = false;
	
	//set colours quantities
	red =10;
	green = 6;
	blue = 4;
	
	//coundown
	for(i=5;i>0;i--)
	{
		//print countdown
		sprintf(display, "STARTING IN: %d", i);
		print_second_line(display);
		//delay
		delay_ms(1000);
	}
	
	//start and show score
	sprintf(display, "score: %d", score);
	print_second_line(display);
	
	//set interrupt callback
	gpio_set_callback(P_IR_REC, training_mode_isr);
	gpio_set_callback(P_INT_BTN, training_mode_isr);
	
	//20 rounds
	for(i=0; i < 20; i++)
	{
		//set is_hit and is_expired to false
		is_hit = false;
		is_expired = false;
		
		//disable timer for reinit
		timer_disable();
		
		//turn off piezo
		gpio_set(P_PIEZO, 0);
		
		//change matrix cell
		//disable interrupt for IR receiver while changing
		can_read = false;
		delay_ms(200);
		set_target();
		delay_ms(200);
		can_read = true;
		
		//start timer with 2 seconds
		timer_init((uint32_t)2*CYCLES_SEC);
		//enable timer
		timer_enable();
		
		//continuously check for the flag
		while(1)
		{
			//if reset is pressed
			if(exit_flag)
			{
				return;
			}
			//check if is hit
			if(is_hit)
			{
				//increment score
				score+=get_multiplier(current_colour);
				
				//show score
				sprintf(display, "score: %d", score);
				print_second_line(display);
				break;
					
			}
			//if time expired
			else if(is_expired)
			{
				break;
			}
		}	
		switch_target_off();
		//delay between change target
		delay_ms(500);
	}
	
	//turn off piezo
	gpio_set(P_PIEZO, 0);
	//disable IR reciever from reading
	can_read = false;
	
	//disable timer
	timer_disable();
		
	//get high score from memory
	EEPROM_Read(0,0, buffer, MODE_32_BIT, sizeof(buffer));

	//print out new score
	sprintf(display, "Final score: %d", score);
	print_second_line(display);
	
	//if score is higher than highscore
	if(score > *buffer)
	{
		//display new high score
		sprintf(display, "HIGH SCORE: %d", score);
		print_second_line(display);
			
		//set highscore
		save_high_score(score);
	}
	
	
	return;
}


void action_isr_TEST(uint32_t sources)
{
	if (sources & (1 << GET_PIN_INDEX(P_INT_BTN))) {
		print_second_line("BTN INTERRUPT");
	}
	if(sources & (1 << GET_PIN_INDEX(P_IR_REC)))
	{
		print_second_line("IR INTERRUPT");
	}
}
