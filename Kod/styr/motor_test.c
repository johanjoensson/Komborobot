/*
 * =====================================================================================
 *
 *       Filename:  motor.c
 *
 *    Description:  styr motorerna
 *        Created:  2012-03-31
 *
 *         Author:  S&S
 *   Organization:  
 *
 * =====================================================================================
 */

#include <avr/io.h>
#include <avr/interrupt.h> 

void init()
 	{
			TCCR0 |= (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00);
			TCCR2 |= (1<<WGM20) | (1<<WGM21) | (1<<COM21) | (1<<CS20);
			DDRD |= (1<<DDD7) | (1<<DDD5) | (1<<DDD4);
			DDRB |= (1<<DDB3);
	}	


void stop(unsigned char speed)
 	{
		OCR0 = 0x00;
		OCR2 = 0x00;
	}

void forward(unsigned char speed)
	{
		PORTD |= (1<<PD4) | (1<<PD5);
		OCR0 = (speed<<4);
		OCR2 = (speed<<4);
	}

void forward_left(unsigned char speed)
	{
		PORTD |= (1<<PD4) | (1<<PD5);
		OCR0 = (speed<<4);
		OCR2 = (speed<<3);
	}

void forward_right(unsigned char speed)
	{
		PORTD |= (1<<PD4) | (1<<PD5);
		OCR0 = (speed<<3);
		OCR2 = (speed<<4);
	}

void rotate_left(unsigned char speed)
	{
		PORTD &= 0xDF;
		PORTD |= (1<<PD4);
		OCR0 = (speed<<4);
		OCR2 = (speed<<4);
	}

void rotate_right(unsigned char speed)
	{
		PORTD &= 0xEF;
		PORTD |= (1<<PD5);
		OCR0 = (speed<<4);
		OCR2 = (speed<<4);
	}

void back(unsigned char speed)
	{
		PORTD &= 0xCF;
		OCR0 = (speed<<4);
		OCR2 = (speed<<4);
	}
