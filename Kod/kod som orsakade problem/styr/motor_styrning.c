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
#include "motor_styrning.h" 
int trim;
int speed_right;
int speed_left;
void init()
 	{
			TCCR0 |= (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00);
			TCCR2 |= (1<<WGM20) | (1<<WGM21) | (1<<COM21) | (1<<CS20);
			DDRD |= (1<<DDD7) | (1<<DDD5) | (1<<DDD4);
			DDRB |= (1<<DDB3);
			PORTD &= 0xCF;
	}	


void stop(unsigned char speed)
 	{
		OCR0 = 0x00;
		OCR2 = 0x00;
	}

void forward(unsigned char speed)
	{
		set_speed(speed);
		PORTD &= 0xCF;	//st�ller riktning fram�t
	/*	OCR0 = 0x89;	//h�ger
		OCR2 = 0x8F;	//v�nster */
		if (trim < 0)
		{
			OCR0 = (speed_right);		//h�ger
			OCR2 = (speed_left);		//v�nster
		}
		else
		{
			OCR0 = (speed_right);		//h�ger
			OCR2 = (speed_left);		//v�nster
		}
	}

void forward_left(unsigned char speed)
	{
		set_speed(speed);
		PORTD &= 0xCF;
		OCR0 = (speed_right);		//h�ger
		OCR2 = (speed_left)-6;		//v�nster
	}

void forward_right(unsigned char speed)
	{	set_speed(speed);
		PORTD &= 0xCF;
		OCR0 = (speed_right)-6;		//h�ger
		OCR2 = (speed_left);		//v�nster
	}

void rotate_left(unsigned char speed)
	{
		set_speed(speed);
		PORTD &= 0xEF;
		PORTD |= (1<<PD5);
		OCR0 = (speed_right);		//h�ger 
		OCR2 = (speed_left)+6;		//v�nster �kar hastigheten n�got p� det hjulet som k�r bak�t
	}

void rotate_right(unsigned char speed)
	{	
		set_speed(speed);
		PORTD &= 0xDF;
		PORTD |= (1<<PD4);
		OCR0 = (speed_right)+6;		//h�ger �kar hastigheten n�got p� det hjulet som k�r bak�t
		OCR2 = (speed_left);		//v�nster
	}

void back(unsigned char speed)
	{	
		set_speed(speed);
		PORTD |= (1<<PD4) | (1<<PD5);
		OCR0 = (speed_right)+6;		//h�ger (med �kad hastighet d� back inte �r lika stark)
		OCR2 = (speed_left)+6;		//v�nster
	}
void set_speed_left(unsigned char speed)
	{
		set_speed(speed);
		PORTD &= 0xCF;	//st�ller riktning fram�t
		OCR2 = (speed_left);		//v�nster
	}
void set_speed_right(unsigned char speed)
	{
		set_speed(speed);
		PORTD &= 0xCF;	//st�ller riktning fram�t
		OCR0 = (speed_right);		//h�ger
	}
void trim_left()		// �kar h�ger speed och minskar v�nster speed med 1
	{
		if(trim>-16){
			trim=trim-1;
		}
	}
void trim_right()		//�kar h�ger speed och minskar v�nster speed med 1
	{
		if(trim<16){
			trim=trim+1;
		}
	}
void trim_zero()		//nollst�ller trimning
	{
		trim=0;
	}
void set_speed(unsigned char speed)
	{
		speed = (speed << 1);		// speed som kommer in ligger 0-F, dubblar det til 0-1E
		speed = 0x6A + speed;		// s�tter l�gsta hastigheten till 6A och h�gsta till 88
		if (trim > 0)
		{
			speed_right = speed - 1;			// s�tter hastigheten f�r rak k�rning h�ger hjul
			speed_left = speed + (2+trim);		// s�tter hastigheten f�r rak k�rning v�nser hjul
		}
		else
		{
			speed_right = speed - (1+trim);		// s�tter hastigheten f�r rak k�rning h�ger hjul
			speed_left = speed + 2;				// s�tter hastigheten f�r rak k�rning v�nser hjul
		}
	}
		
/*
void decide_speed(unsigned char speed)
{
		
}
*/


