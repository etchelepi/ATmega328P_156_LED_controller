/*
 * Shift.h
 *
 *  Created on: Oct 1, 2013
 *      Author: Evan Tchelepi
 */

#ifndef SHIFT_H_
#define SHIFT_H_

/*defined variables and descriptions*/
#define SRCLK_PIN PB2 /*This is pin 10 on the board*/
#define RCLK_PIN PB1

//How many of the shift registers are there daisey chained?
#define number_of_74hc595s 1

/*We have multiple banks of LED output*/
#define SER_PIN_BANK_1 PB0 /**/
#define SER_PIN_BANK_2 PB3 /**/
#define SER_PIN_BANK_3 PB4 /**/
#define SER_PIN_BANK_4 PB5 /**/
#define SER_PIN_BANK_5 PB6 /**/
#define SER_PIN_BANK_6 PB7 /**/


#define PIN_8 PB0 //PD4
#define PIN_9 PB1 //PD4
#define PIN_10 PB2 //PD4
#define LED PB5 //PD4

#define SIZE_OF_BANK 16 // The size of each LED bank
#define NUM_TRI_LEDS 52
#define NUM_COLORS_PER_LED 3

/*The size of a LED color pattern. This is how many times we need to flash
 * a color value inorder to create the illusion of a color*/
#define COLOR_DEPTH 64

#define QUEUE_SIZE 10


/*Define types we need for our design*/
typedef struct tri_color_led
	{
		char RED;
		char GREEN;
		char BLUe;

	}TRI_COLOR_LED;


#endif /* SHIFT_H_ */
