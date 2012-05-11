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
		PORTD &= 0xCF;	//ställer riktning framåt
	/*	OCR0 = 0x89;	//höger
		OCR2 = 0x8F;	//vänster */
		if (trim < 0)
		{
			OCR0 = (speed_right);		//höger
			OCR2 = (speed_left);		//vänster
		}
		else
		{
			OCR0 = (speed_right);		//höger
			OCR2 = (speed_left);		//vänster
		}
	}

void forward_left(unsigned char speed)
	{
		set_speed(speed);
		PORTD &= 0xCF;
		OCR0 = (speed_right);		//höger
		OCR2 = (speed_left)-6;		//vänster
	}

void forward_right(unsigned char speed)
	{	set_speed(speed);
		PORTD &= 0xCF;
		OCR0 = (speed_right)-6;		//höger
		OCR2 = (speed_left);		//vänster
	}

void rotate_left(unsigned char speed)
	{
		set_speed(speed);
		PORTD &= 0xEF;
		PORTD |= (1<<PD5);
		OCR0 = (speed_right);		//höger 
		OCR2 = (speed_left)+6;		//vänster ökar hastigheten något på det hjulet som kör bakåt
	}

void rotate_right(unsigned char speed)
	{	
		set_speed(speed);
		PORTD &= 0xDF;
		PORTD |= (1<<PD4);
		OCR0 = (speed_right)+6;		//höger ökar hastigheten något på det hjulet som kör bakåt
		OCR2 = (speed_left);		//vänster
	}

void back(unsigned char speed)
	{	
		set_speed(speed);
		PORTD |= (1<<PD4) | (1<<PD5);
		OCR0 = (speed_right)+6;		//höger (med ökad hastighet då back inte är lika stark)
		OCR2 = (speed_left)+6;		//vänster
	}
void set_speed_left(unsigned char speed)
	{
		set_speed(speed);
		PORTD &= 0xCF;	//ställer riktning framåt
		OCR2 = (speed_left);		//vänster
	}
void set_speed_right(unsigned char speed)
	{
		set_speed(speed);
		PORTD &= 0xCF;	//ställer riktning framåt
		OCR0 = (speed_right);		//höger
	}
void trim_left()		// ökar höger speed och minskar vänster speed med 1
	{
		if(trim>-16){
			trim=trim-1;
		}
	}
void trim_right()		//ökar höger speed och minskar vänster speed med 1
	{
		if(trim<16){
			trim=trim+1;
		}
	}
void trim_zero()		//nollställer trimning
	{
		trim=0;
	}
void set_speed(unsigned char speed)
	{
		speed = (speed << 1);		// speed som kommer in ligger 0-F, dubblar det til 0-1E
		speed = 0x6A + speed;		// sätter lägsta hastigheten till 6A och högsta till 88
		if (trim > 0)
		{
			speed_right = speed - 1;			// sätter hastigheten för rak körning höger hjul
			speed_left = speed + (2+trim);		// sätter hastigheten för rak körning vänser hjul
		}
		else
		{
			speed_right = speed - (1+trim);		// sätter hastigheten för rak körning höger hjul
			speed_left = speed + 2;				// sätter hastigheten för rak körning vänser hjul
		}
	}
		
/*
void decide_speed(unsigned char speed)
{
		
}
*/


