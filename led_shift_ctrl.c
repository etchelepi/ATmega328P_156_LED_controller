/******************************************************************************
 * Program Name: LED_SHIFT_CTRL
 * Author: Evan Tchelepi
 * Create Date:
 * Last Modified: 10/15/2014
 * Notes: 	This is the code to drive the arrays of shift registers to control
 *			156 LEDs or 52 RGB LEDs in 5 bit color at 60hz.
 *****************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#include <avr/iom328.h>
#include "led_shift_ctrl.h"

#define TIMSK0 _SFR_MEM8(0x6E)
#define TIMSK1 _SFR_MEM8(0x6F)
#define TIMSK2 _SFR_MEM8(0x70)

#define RED 0
#define GREEN 1
#define BLUE 2

void update_led_bank (char bank , char index, char red, char green, char blue);

/*Global Variables*/
//TRI_COLOR_LED led_array[52] = {0}; /*This is an array of all our LEDs*/
/*Giant Global memory block that eats up 1.2K*/
//char led_array[156 * 8] = {0};

volatile char LED_UPDATE_EVENT = 0;

unsigned char led_master_array [10][3] = {{0}};

unsigned char led_array_bank_0 [64][4];
unsigned char led_array_bank_1 [64][4];
unsigned char led_array_bank_2 [64][4];
unsigned char led_array_bank_3 [64][4];
unsigned char led_array_bank_4 [64][4];

/******************************************************************************
 * Function Name: Shift LED
 * Author: Evan Tchelepi
 * Create Date: 10/1/2013
 * Last Modified:
 * Function: This function takes the data from all the banks, and then shifts
 *           them onto the LEDs. When it finishes they are output.
 * Input: Unsigned chars for each Bank
 * Return Value: None
 * Notes:
 *****************************************************************************/
void shift_led_8bit(unsigned char data_bank_0,
					unsigned char data_bank_1,
					unsigned char data_bank_2,
					unsigned char data_bank_3)
{

	/*Before starting to write out, we must disable the output registers
	 * by setting the register clk pin low before shifting in the data*/
	PORTB &= ~(1 << RCLK_PIN);

	/*We now need to loop through the length of the data to convert it
	 * to a serial output for the shift registers*/
	for (int i = 0; i < 8; i++){	// Now we are entering the loop to shift out 8+ bits

		/*We set the serial clock pin low*/
		PORTB &= ~(1 << SRCLK_PIN);
		/*We are going to now set the output of the data to the pins*/
		PORTB |= (((data_bank_0&(0x01<<i))>>i) << SER_PIN_BANK_1 );
		/*We now need to set the serial clock pin high*/
		PORTB |= (1 << SRCLK_PIN);
		/*We now need to set the Data pin low again*/
		PORTB &= ~(((data_bank_0&(0x01<<i))>>i) << SER_PIN_BANK_1 );
	}
	for (int i = 0; i < 8; i++){	// Now we are entering the loop to shift out 8+ bits

		/*We set the serial clock pin low*/
		PORTB &= ~(1 << SRCLK_PIN);
		/*We are going to now set the output of the data to the pins*/
		PORTB |= (((data_bank_1&(0x01<<i))>>i) << SER_PIN_BANK_1 );
		/*We now need to set the serial clock pin high*/
		PORTB |= (1 << SRCLK_PIN);
		/*We now need to set the Data pin low again*/
		PORTB &= ~(((data_bank_1&(0x01<<i))>>i) << SER_PIN_BANK_1 );
	}
	for (int i = 0; i < 8; i++){	// Now we are entering the loop to shift out 8+ bits

		/*We set the serial clock pin low*/
		PORTB &= ~(1 << SRCLK_PIN);
		/*We are going to now set the output of the data to the pins*/
		PORTB |= (((data_bank_2&(0x01<<i))>>i) << SER_PIN_BANK_1 );
		/*We now need to set the serial clock pin high*/
		PORTB |= (1 << SRCLK_PIN);
		/*We now need to set the Data pin low again*/
		PORTB &= ~(((data_bank_2&(0x01<<i))>>i) << SER_PIN_BANK_1 );
	}
	for (int i = 0; i < 8; i++){	// Now we are entering the loop to shift out 8+ bits

		/*We set the serial clock pin low*/
		PORTB &= ~(1 << SRCLK_PIN);
		/*We are going to now set the output of the data to the pins*/
		PORTB |= (((data_bank_3&(0x01<<i))>>i) << SER_PIN_BANK_1 );
		/*We now need to set the serial clock pin high*/
		PORTB |= (1 << SRCLK_PIN);
		/*We now need to set the Data pin low again*/
		PORTB &= ~(((data_bank_3&(0x01<<i))>>i) << SER_PIN_BANK_1 );
	}

	/*The register clock is set high again to update the output giving the illusion
	 * that they are all being updated at once*/
	PORTB |= (1 << RCLK_PIN);
}

/******************************************************************************
 * Function Name: Shift LED
 * Author: Evan Tchelepi
 * Create Date: 10/1/2013
 * Last Modified:
 * Function: This function takes the data from all the banks, and then shifts
 *           them onto the LEDs. When it finishes they are output.
 * Input: 
 * Return Value: N/A
 * Notes:
 *****************************************************************************/
void shift_led_8bit2(unsigned char data_bank_A0,
					 unsigned char data_bank_A1,
					 unsigned char data_bank_A2,
					 unsigned char data_bank_A3,

					 unsigned char data_bank_B0,
					 unsigned char data_bank_B1,
					 unsigned char data_bank_B2,
					 unsigned char data_bank_B3,

					 unsigned char data_bank_C0,
					 unsigned char data_bank_C1,
					 unsigned char data_bank_C2,
					 unsigned char data_bank_C3,

					 unsigned char data_bank_D0,
					 unsigned char data_bank_D1,
					 unsigned char data_bank_D2,
					 unsigned char data_bank_D3,

					 unsigned char data_bank_E0,
					 unsigned char data_bank_E1,
					 unsigned char data_bank_E2,
					 unsigned char data_bank_E3)
{

	char i;
	/*Before starting to write out, we must disable the output registers
	 * by setting the register clk pin low before shifting in the data*/
	PORTB &= ~(1 << RCLK_PIN);

	/*We now need to loop through the length of the data to convert it
	 * to a serial output for the shift registers*/
	for (i =0; i < 8; i++){	// Now we are entering the loop to shift out 8+ bits

		/*We set the serial clock pin low*/
		PORTB &= ~(1 << SRCLK_PIN);
		/*We are going to now set the output of the data to the pins*/
		PORTB |= (((data_bank_A0&(0x01<<i))>>i) << SER_PIN_BANK_1 );
		PORTB |= (((data_bank_B0&(0x01<<i))>>i) << SER_PIN_BANK_2 );
		PORTB |= (((data_bank_C0&(0x01<<i))>>i) << SER_PIN_BANK_3 );
		PORTB |= (((data_bank_D0&(0x01<<i))>>i) << SER_PIN_BANK_4 );
		PORTB |= (((data_bank_E0&(0x01<<i))>>i) << SER_PIN_BANK_5 );
		/*We now need to set the serial clock pin high*/
		//PORTB |= (1 << SRCLK_PIN);
		PORTB = (1 << SRCLK_PIN);
		/*We now need to set the Data pin low again*/
		/*PORTB &= ~(((data_bank_A0&(0x01<<i))>>i) << SER_PIN_BANK_1 );
		PORTB &= ~(((data_bank_B0&(0x01<<i))>>i) << SER_PIN_BANK_2 );
		PORTB &= ~(((data_bank_C0&(0x01<<i))>>i) << SER_PIN_BANK_3 );
		PORTB &= ~(((data_bank_D0&(0x01<<i))>>i) << SER_PIN_BANK_4 );
		PORTB &= ~(((data_bank_E0&(0x01<<i))>>i) << SER_PIN_BANK_5 );*/
	}
	for (i =0; i < 8; i++){	// Now we are entering the loop to shift out 8+ bits

		/*We set the serial clock pin low*/
		PORTB &= ~(1 << SRCLK_PIN);
		/*We are going to now set the output of the data to the pins*/
		PORTB |= (((data_bank_A1&(0x01<<i))>>i) << SER_PIN_BANK_1 );
		PORTB |= (((data_bank_B1&(0x01<<i))>>i) << SER_PIN_BANK_2 );
		PORTB |= (((data_bank_C1&(0x01<<i))>>i) << SER_PIN_BANK_3 );
		PORTB |= (((data_bank_D1&(0x01<<i))>>i) << SER_PIN_BANK_4 );
		PORTB |= (((data_bank_E1&(0x01<<i))>>i) << SER_PIN_BANK_5 );

		/*We now need to set the serial clock pin high*/
		PORTB = (1 << SRCLK_PIN);
		/*We now need to set the Data pin low again*/
		/*PORTB &= ~(((data_bank_A1&(0x01<<i))>>i) << SER_PIN_BANK_1 );
		PORTB &= ~(((data_bank_B1&(0x01<<i))>>i) << SER_PIN_BANK_2 );
		PORTB &= ~(((data_bank_C1&(0x01<<i))>>i) << SER_PIN_BANK_3 );
		PORTB &= ~(((data_bank_D1&(0x01<<i))>>i) << SER_PIN_BANK_4 );
		PORTB &= ~(((data_bank_E1&(0x01<<i))>>i) << SER_PIN_BANK_5 );*/
	}
	for (i =0; i < 8; i++){	// Now we are entering the loop to shift out 8+ bits

		/*We set the serial clock pin low*/
		PORTB &= ~(1 << SRCLK_PIN);
		/*We are going to now set the output of the data to the pins*/
		PORTB |= (((data_bank_A2&(0x01<<i))>>i) << SER_PIN_BANK_1 );
		PORTB |= (((data_bank_B2&(0x01<<i))>>i) << SER_PIN_BANK_2 );
		PORTB |= (((data_bank_C2&(0x01<<i))>>i) << SER_PIN_BANK_3 );
		PORTB |= (((data_bank_D2&(0x01<<i))>>i) << SER_PIN_BANK_4 );
		PORTB |= (((data_bank_E2&(0x01<<i))>>i) << SER_PIN_BANK_5 );
		/*We now need to set the serial clock pin high*/
		PORTB = (1 << SRCLK_PIN);
		/*We now need to set the Data pin low again*/
		/*PORTB &= ~(((data_bank_A2&(0x01<<i))>>i) << SER_PIN_BANK_1 );
		PORTB &= ~(((data_bank_B2&(0x01<<i))>>i) << SER_PIN_BANK_2 );
		PORTB &= ~(((data_bank_C2&(0x01<<i))>>i) << SER_PIN_BANK_3 );
		PORTB &= ~(((data_bank_D2&(0x01<<i))>>i) << SER_PIN_BANK_4 );
		PORTB &= ~(((data_bank_E2&(0x01<<i))>>i) << SER_PIN_BANK_5 );*/
	}
	for (i =0; i < 8; i++){	// Now we are entering the loop to shift out 8+ bits

		/*We set the serial clock pin low*/
		PORTB &= ~(1 << SRCLK_PIN);
		/*We are going to now set the output of the data to the pins*/
		PORTB |= (((data_bank_A3&(0x01<<i))>>i) << SER_PIN_BANK_1 );
		PORTB |= (((data_bank_B3&(0x01<<i))>>i) << SER_PIN_BANK_2 );
		PORTB |= (((data_bank_C3&(0x01<<i))>>i) << SER_PIN_BANK_3 );
		PORTB |= (((data_bank_D3&(0x01<<i))>>i) << SER_PIN_BANK_4 );
		PORTB |= (((data_bank_E3&(0x01<<i))>>i) << SER_PIN_BANK_5 );
		/*We now need to set the serial clock pin high*/
		PORTB = (1 << SRCLK_PIN);
		/*We now need to set the Data pin low again*/
		/*PORTB &= ~(((data_bank_A3&(0x01<<i))>>i) << SER_PIN_BANK_1 );
		PORTB &= ~(((data_bank_B3&(0x01<<i))>>i) << SER_PIN_BANK_1 );
		PORTB &= ~(((data_bank_C3&(0x01<<i))>>i) << SER_PIN_BANK_1 );
		PORTB &= ~(((data_bank_D3&(0x01<<i))>>i) << SER_PIN_BANK_1 );
		PORTB &= ~(((data_bank_E3&(0x01<<i))>>i) << SER_PIN_BANK_1 );*/
	}

	/*The register clock is set high again to update the output giving the illusion
	 * that they are all being updated at once*/
	PORTB |= (1 << RCLK_PIN);
}

/******************************************************************************
 * Function Name: Shift LED
 * Author: Evan Tchelepi
 * Create Date: 10/1/2013
 * Last Modified:
 * Function: This function takes the data from all the banks, and then shifts
 *           them onto the LEDs. When it finishes they are output.
 * Input:
 * Return Value:
 * Notes:
 *****************************************************************************/
void shift_led_32bit(unsigned long data_bank_0)
{

	/*Before starting to write out, we must disable the output registers
	 * by setting the register clk pin low before shifting in the data*/
	PORTB &= ~(1 << RCLK_PIN);

	/*We now need to loop through the length of the data to convert it
	 * to a serial output for the shift registers*/
	for (int i = 0; i < 32; i++){	// Now we are entering the loop to shift out 8+ bits

		/*We set the serial clock pin low*/
		PORTB &= ~(1 << SRCLK_PIN);

		/*We are going to now set the output of the data to the pins*/
		PORTB |= (((data_bank_0&(0x01<<i))>>i) << SER_PIN_BANK_1 );
		//PORTB |= (((*data_bank_1&(0x01<<i))>>i) << SER_PIN_BANK_2 );
		//PORTB |= (((*data_bank_2&(0x01<<i))>>i) << SER_PIN_BANK_3 );
		//PORTB |= (((*data_bank_3&(0x01<<i))>>i) << SER_PIN_BANK_4 );
		//PORTB |= (((*data_bank_4&(0x01<<i))>>i) << SER_PIN_BANK_5 );

		/*We now need to set the serial clock pin high*/
		PORTB |= (1 << SRCLK_PIN);

		/*We now need to set the Data pin low again*/
		/*NOTE: this might not be necessary*/
		PORTB &= ~(((data_bank_0&(0x01<<i))>>i) << SER_PIN_BANK_1 );
		//PORTB &= ~(((*data_bank_1&(0x01<<i))>>i) << SER_PIN_BANK_2 );
		//PORTB &= ~(((*data_bank_2&(0x01<<i))>>i) << SER_PIN_BANK_3 );
		//PORTB &= ~(((*data_bank_3&(0x01<<i))>>i) << SER_PIN_BANK_4 );
		//PORTB &= ~(((*data_bank_4&(0x01<<i))>>i) << SER_PIN_BANK_5 );
	}

	/*The register clock is set high again to update the output giving the illusion
	 * that they are all being updated at once*/
	PORTB |= (1 << RCLK_PIN);
}


/******************************************************************************
 * Function Name: Shift LED
 * Author: Evan Tchelepi
 * Create Date: 10/1/2013
 * Last Modified:
 * Function: This function takes the data from all the banks, and then shifts
 *           them onto the LEDs. When it finishes they are output.
 * Input:
 * Return Value:
 * Notes:
 *****************************************************************************/
void shift_led(	unsigned long data_bank_0,
				unsigned long* data_bank_1X,
				unsigned long* data_bank_2X,
				unsigned long* data_bank_3X,
				unsigned long* data_bank_4X)
{

	/*Before starting to write out, we must disable the output registers
	 * by setting the register clk pin low before shifting in the data*/
	PORTB &= ~(1 << RCLK_PIN);

	/*We now need to loop through the length of the data to convert it
	 * to a serial output for the shift registers*/
	for (int i = 0; i < 32; i++){	// Now we are entering the loop to shift out 8+ bits

		/*We set the serial clock pin low*/
		PORTB &= ~(1 << SRCLK_PIN);

		/*We are going to now set the output of the data to the pins*/
		PORTB |= (((data_bank_0&(0x01<<i))>>i) << SER_PIN_BANK_1 );
		//PORTB |= (((*data_bank_1&(0x01<<i))>>i) << SER_PIN_BANK_2 );
		//PORTB |= (((*data_bank_2&(0x01<<i))>>i) << SER_PIN_BANK_3 );
		//PORTB |= (((*data_bank_3&(0x01<<i))>>i) << SER_PIN_BANK_4 );
		//PORTB |= (((*data_bank_4&(0x01<<i))>>i) << SER_PIN_BANK_5 );

		/*We now need to set the serial clock pin high*/
		PORTB |= (1 << SRCLK_PIN);

		/*We now need to set the Data pin low again*/
		/*NOTE: this might not be necessary*/
		PORTB &= ~(((data_bank_0&(0x01<<i))>>i) << SER_PIN_BANK_1 );
		//PORTB &= ~(((*data_bank_1&(0x01<<i))>>i) << SER_PIN_BANK_2 );
		//PORTB &= ~(((*data_bank_2&(0x01<<i))>>i) << SER_PIN_BANK_3 );
		//PORTB &= ~(((*data_bank_3&(0x01<<i))>>i) << SER_PIN_BANK_4 );
		//PORTB &= ~(((*data_bank_4&(0x01<<i))>>i) << SER_PIN_BANK_5 );
	}

	/*The register clock is set high again to update the output giving the illusion
	 * that they are all being updated at once*/
	PORTB |= (1 << RCLK_PIN);
}

/******************************************************************************
 * Function Name: update_led
 * Author: Evan Tchelepi
 * Create Date: 10/1/2013
 * Last Modified:
 * Function: This takes a index and finds the right bank. Then updates that bank
 * Input:
 * Return Value:
 * Notes:
 *****************************************************************************/
void update_led (char index, char red, char green, char blue)
{
	char bank_number;

	if(index >= 40) //We are in the 5th bank
	{
		bank_number = 4;
	}
	else if (index >= 30)
	{
		bank_number = 3;
	}
	else if (index >= 20)
	{
		bank_number = 2;
	}
	else if (index >= 10)
	{
		bank_number = 1;
	}
	else //the number is smaller then ten so it must be in the final bank
	{
		bank_number = 0;
	}

	update_led_bank(bank_number, index%10,red,green,blue);

}

/******************************************************************************
 * Function Name: update_led_array
 * Author: Evan Tchelepi
 * Create Date: 10/1/2013
 * Last Modified:
 * Function: We intend to update the memory array with a new RGB value for the
             led provided.
 * Input:
 * Return Value:
 * Notes:
 *****************************************************************************/
void update_led_bank (char bank , char index, char red, char green, char blue)
{
	unsigned char i;
	unsigned char bank_index = index%10;
	int byte_set;// = (i * index * 3) /8; /*our byte location*/
	int bit_set;// = (i * index * 3) % 8; /*our bit location within the byte*/
	unsigned char* location; //= (led_array + byte_set);
	unsigned char* location_overflow;// = led_array + (byte_set + 1);

	/*Byte set gives us 0,1,2,3 which we use to index into the array*/
	byte_set = ((bank_index*3)/8);
	/*this tells us where to */
	bit_set = 7 - (bank_index * 3) % 8;
	/*We need to know which byte we care about */
	//printf("\nBITSET: %d BYTESET: %d",bit_set,byte_set);

	/*We need to go through and update all the values*/
	for (i = 0; i < 64; i++)
	{
		/*We need to set the location */

		//*FIX HERE THE LOCATION ONLY DOES THE FIRST BANK*//
		/*Should be rewritten to just pass the right pointer to begin with and
		 * not use a case statment. for now we just want it to work*/
		switch(bank)
		{
		    case 0:
		    	location = &led_array_bank_0[i][byte_set];
		    	location_overflow = &led_array_bank_0[i][byte_set+1];
		        break;
		    case 1:
		    	location = &led_array_bank_1[i][byte_set];
		    	location_overflow = &led_array_bank_1[i][byte_set+1];
		        break;
		    case 2:
		    	location = &led_array_bank_2[i][byte_set];
		    	location_overflow = &led_array_bank_2[i][byte_set+1];
		        break;
		    case 3:
		    	location = &led_array_bank_3[i][byte_set];
		    	location_overflow = &led_array_bank_3[i][byte_set+1];
		    	break;
		    case 4:
		    	location = &led_array_bank_4[i][byte_set];
		    	location_overflow = &led_array_bank_4[i][byte_set+1];
		    	break;
		}

		if(i < red) /*If the red should be positive*/
		{
		    /*We shift over the bit by the amount it takes to move it's
		    position to the correct bit location*/
		    *location |= (0x01 << bit_set);
		}
		else
		{
			/*We need to erase any value if it happens to be a zero at
			the location for the value of red. We do this by subtracking
			the bit shifted valye from 11111111 such that a zero is in that
			location. The & result will then be missing that location*/
			*location &= (0xFF - (0x01 << bit_set));
		 }
		/*GREEN*/
		if (bit_set == 0) //Green is in the next location over
		{
			if(i < green)
			{
				*location_overflow |= (0x01 << 7); /*0000000R|G0000000*/
			}
			else
			{
				*location_overflow &= (0xFF - (0x01 << 7));/*0000000R|G0000000*/
			}
		}
		else /*Only blue overflowed*/
		{
			if(i < green)
			{
				*location|= (0x01 << (bit_set -1));
			}
			else
			{
				*location&= (0xFF - (0x01 << (bit_set - 1)));
			}
		}
		/*BLUE*/
		if (bit_set == 0) //blue is two bits into the overflow
		{
			if(i < blue)
		    {
		                *location_overflow |= (0x01 << 6); /*0000000R|GB000000*/
		    }
		    else
		    {
		                *location_overflow &= (0xFF - (0x01 << 6));/*1111111R|GB111111*/
		    }
		}
		else if (bit_set == 1) /*blue is one bit into the overflow*/
		{
		    if(i < blue)
		    {
		    	*location_overflow |= (0x01 << 7); /*0000000R|G0000000*/
		    }
		    else
		    {
		    	*location_overflow &= (0xFF - (0x01 << 7));/*0000000R|G0000000*/
		    }
		}
		else /*No one overflowed*/
		{
		    if(i < blue)
		    {
		        *location|= (0x01 << (bit_set -2));
		    }
		    else
		    {
		    	*location&= (0xFF - (0x01 << (bit_set - 2)));
		    }
		}



	}
}

/******************************************************************************
 * Function Name: Test_Pattern_update
 * Author: Evan Tchelepi
 * Create Date: 10/1/2013
 * Last Modified:
 * Function:
 * Input:
 * Return Value:
 * Notes:
 *****************************************************************************/
void test_pattern_update (unsigned char LED_location)
{
	unsigned char color_red = led_master_array[LED_location][RED];
	unsigned char color_green = led_master_array[LED_location][RED];
	unsigned char color_blue = led_master_array[LED_location][RED];
	if(color_red > 63)
	{
		color_red = color_red -128;
	}
	if(color_green > 63)
	{
			color_green = color_green -128;
	}
	if(color_blue > 63)
	{
			color_blue = color_blue -128;
	}

	update_led ( LED_location,
			color_red,
			color_green,
			color_blue);
}
/******************************************************************************
 * Function Name: bounce color
 * Author: Evan Tchelepi
 * Create Date: 10/1/2013
 * Last Modified:
 * Function: This is a simple test to move colors back and fourth to demo
 * Input:
 * Return Value:
 * Notes:
 *****************************************************************************/
void bounce_color (unsigned char LED_location, unsigned char color)
{
	unsigned char working = led_master_array[LED_location][color];
	if(working & 128) //We check for direction
	{
		//We are downward direction
		if (working > (128 + 20))
		{
			/*If we don't roll over subtract 1*/
			working--;
		}
		else //We set working red back to zero
		{
			working = 20;
		}
	}
	else
	{
		if(working < 50)
		{
			/** if we have not crossed the limit*/
			working++;
		}
		else
		{
			/*We set it to go downawrds*/
			working = 128 + 50;
		}
	}
	led_master_array[LED_location][color] = working;
}

/******************************************************************************
 * Function Name: main
 * Author: Evan Tchelepi
 * Create Date: 10/1/2013
 * Function: This is where we run from, and tests the code in this file.
 * Input:
 * Return Value: N/A this is am embeeded system. No one is watching for the return
 * Notes:
 *****************************************************************************/
int main()
{
	/*Define variables to be used in main*/
	char LED_CDC = 0;/*LED COLOR DEPTH COUNTER*/
	unsigned char queue[10]; /*queue points to a place in memory*/
	unsigned char queue_index = 0; /*Where we read values from*/
	unsigned char queue_end = 0; /*where we write values too*/
	char z = 0;

	/*Define Port and other embedded specific registers*/
	DDRB = 0xFF;
	PORTB = 0x00;
/******************TIMER INTERRUPT STUFF**************************************/
	/*We need to start the design off with our interrupt setup. This allows
	 * us to stop doing whatever we are doing and update the LEDs as needed
	 * as this is the most time critical part inorder to create the illusion
	 * of color*/
	/*initialize Timer1*/
	cli();          // disable global interrupts
	TCCR1A = 0;     // set entire TCCR1A register to 0
	TCCR1B = 0;     // same for TCCR1B
	/*set compare match register to desired timer count*/
	OCR1A = 250; /*159 ~ 1ms response time*/
	/*turn on CTC mode*/
	TCCR1B |= (1 << WGM12);
	/*Set CS10 and CS12 bits for 1024 prescaler*/
	TCCR1B |= (1 << CS10);
	TCCR1B |= (1 << CS12);
	/*enable timer compare interrupt*/
	TIMSK1 |= (1 << OCIE1A);
	/*enable global interrupts*/
	sei();
/*********************END TIMER INTERRUPT STUFF*******************************/


/******************RUN ANY ONE TIME FUNCTIONS*********************************/
				update_led ( 0, 63 , 0, 0);
				update_led ( 1, 63, 0, 0);
				update_led ( 2, 63, 0, 0);
				update_led ( 3, 63, 0, 0);
				update_led ( 4, 63, 0, 0);
				update_led ( 5, 63, 0, 0);
				update_led ( 6, 63, 0, 0);
				update_led ( 7, 63, 0, 0);
				update_led ( 8, 63, 0, 0);
				update_led ( 9, 63, 0, 0);/*

				update_led ( 0, 0 , 63, 0);
				update_led ( 1, 0 , 63, 0);
				update_led ( 2, 0 , 63, 0);
				update_led ( 3, 0 , 63, 0);
				update_led ( 4, 0 , 63, 0);
				update_led ( 5, 0 , 63, 0);
				update_led ( 6, 0 , 63, 0);
				update_led ( 7, 0 , 63, 0);
				update_led ( 8, 0 , 63, 0);
				update_led ( 9, 0 , 63, 0);

				update_led ( 0, 0 , 0, 63);
				update_led ( 1, 0 , 0, 63);
				update_led ( 2, 0 , 0, 63);
				update_led ( 3, 0 , 0, 63);
				update_led ( 4, 0 , 0, 63);
				update_led ( 5, 0 , 0, 63);
				update_led ( 6, 0 , 0, 63);
				update_led ( 7, 0 , 0, 63);
				update_led ( 8, 0 , 0, 63);
				update_led ( 9, 0 , 0, 63);*/
				update_led ( 10, 63, 0, 0);
				update_led ( 11, 63, 63, 0);
				update_led ( 12, 63, 0, 0);
				update_led ( 13, 63, 0, 0);
				update_led ( 14, 63, 0, 0);
				update_led ( 15, 63, 0, 0);
				update_led ( 16, 63, 0, 0);
				update_led ( 17, 63, 0, 0);
/******************END ONE TIME FUNCTIONS*************************************/
	while (1)
	{
		if(LED_UPDATE_EVENT)
				{
					for(LED_CDC = 0; LED_CDC < COLOR_DEPTH; LED_CDC++)
					{
						/*We shift out each y set of bits*/
						shift_led_8bit2(
								led_array_bank_0[LED_CDC][3],
								led_array_bank_0[LED_CDC][2],
								led_array_bank_0[LED_CDC][1],
								led_array_bank_0[LED_CDC][0],

								led_array_bank_1[LED_CDC][3],
								led_array_bank_1[LED_CDC][2],
								led_array_bank_1[LED_CDC][1],
								led_array_bank_1[LED_CDC][0],
								0,0,0,0,
								0,0,0,0,
								0,0,0,0);
					}
					LED_UPDATE_EVENT = 0;
				}
	}
	while (1)
	{
		/*Update the color pattern if a timer interrupt has happened*/
		if(LED_UPDATE_EVENT)
		{
			for(LED_CDC = 0; LED_CDC < COLOR_DEPTH; LED_CDC++)
			{
				/*We shift out each y set of bits*/
				shift_led_8bit2(
						led_array_bank_0[LED_CDC][3],
						led_array_bank_0[LED_CDC][2],
						led_array_bank_0[LED_CDC][1],
						led_array_bank_0[LED_CDC][0],
												0,0,0,0,
												0,0,0,0,
												0,0,0,0,
												0,0,0,0);
			}
			/*Reset the LED event*/
			LED_UPDATE_EVENT = 0;
		}
		/*update one item from the LED queue*/
		if(!LED_UPDATE_EVENT)
		{
		update_led (queue[queue_index],
				led_master_array[queue[queue_index]][RED],
				led_master_array[queue[queue_index]][GREEN],
				led_master_array[queue[queue_index]][BLUE]);
		/*Shift to the next space in the queue*/
		queue_index = (queue_index + 1)%QUEUE_SIZE;
		}
		/*update the master color array and the queue (if we even want to*/
		if(!LED_UPDATE_EVENT)
		{
			bounce_color (z, BLUE);
		}
		/*update green when red has had 32 moves*/
		if(led_master_array[z][BLUE] & 32)
		{
			bounce_color (z, GREEN);
		}
		/*update green when red has had 32 moves*/
		if(led_master_array[z][GREEN] & 32)
		{
			bounce_color (z, RED);
		}


		queue[queue_end] = z; /*We write to the last known empty value*/
		queue_end = (queue_end + 1) %QUEUE_SIZE;

		if(led_master_array[z][RED] & 32)
		{
			z = (z + 1) %10;
		}

			/*We update one LED in our LED queue*/
			//test_pattern_update(z%6);
			/*Bounce Colors*/
			//bounce_color (z%6, RED);
			/*update green when red has had 32 moves*/
			//if(led_master_array[z%6][RED] & 8)
			//{
				//bounce_color (z%6, GREEN);
			//}
			/*update green when red has had 32 moves*/
			//if(led_master_array[z%6][GREEN] & 8)
			//{
				//bounce_color (z%6, BLUE);
			//}
			/*This is the 64 bit shift*/

	}


	return 0;
}


/********************************************************************************
Interrupt Routines
********************************************************************************/
/*We update a LED event*/
ISR(TIMER1_COMPA_vect) {
	LED_UPDATE_EVENT = 1;
}
