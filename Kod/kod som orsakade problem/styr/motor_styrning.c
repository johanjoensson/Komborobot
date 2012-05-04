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
	/*	OCR0 = 0x89;	//hšger
		OCR2 = 0x8F;	//vŠnster */
		if (trim < 0)
		{
			OCR0 = (speed_right);		//hšger
			OCR2 = (speed_left);		//vŠnster
		}
		else
		{
			OCR0 = (speed_right);		//hšger
			OCR2 = (speed_left);		//vŠnster
		}
	}

void forward_left(unsigned char speed)
	{
		set_speed(speed);
		PORTD &= 0xCF;
		OCR0 = (speed_right);		//hšger
		OCR2 = (speed_left)-6;		//vŠnster
	}

void forward_right(unsigned char speed)
	{	set_speed(speed);
		PORTD &= 0xCF;
		OCR0 = (speed_right)-6;		//hšger
		OCR2 = (speed_left);		//vŠnster
	}

void rotate_left(unsigned char speed)
	{
		set_speed(speed);
		PORTD &= 0xEF;
		PORTD |= (1<<PD5);
		OCR0 = (speed_right);		//hšger 
		OCR2 = (speed_left)+6;		//vŠnster škar hastigheten nŒgot pŒ det hjulet som kšr bakŒt
	}

void rotate_right(unsigned char speed)
	{	
		set_speed(speed);
		PORTD &= 0xDF;
		PORTD |= (1<<PD4);
		OCR0 = (speed_right)+6;		//hšger škar hastigheten nŒgot pŒ det hjulet som kšr bakŒt
		OCR2 = (speed_left);		//vŠnster
	}

void back(unsigned char speed)
	{	
		set_speed(speed);
		PORTD |= (1<<PD4) | (1<<PD5);
		OCR0 = (speed_right)+6;		//hšger (med škad hastighet dŒ back inte Šr lika stark)
		OCR2 = (speed_left)+6;		//vŠnster
	}
void set_speed_left(unsigned char speed)
	{
		set_speed(speed);
		PORTD &= 0xCF;	//ställer riktning framåt
		OCR2 = (speed_left);		//vŠnster
	}
void set_speed_right(unsigned char speed)
	{
		set_speed(speed);
		PORTD &= 0xCF;	//ställer riktning framåt
		OCR0 = (speed_right);		//hšger
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
		speed = 0x6A + speed;		// sŠtter lŠgsta hastigheten till 6A och hšgsta till 88
		if (trim > 0)
		{
			speed_right = speed - 1;			// sŠtter hastigheten fšr rak kšrning hšger hjul
			speed_left = speed + (2+trim);		// sŠtter hastigheten fšr rak kšrning vŠnser hjul
		}
		else
		{
			speed_right = speed - (1+trim);		// sŠtter hastigheten fšr rak kšrning hšger hjul
			speed_left = speed + 2;				// sŠtter hastigheten fšr rak kšrning vŠnser hjul
		}
	}
		
/*
void decide_speed(unsigned char speed)
{
		
}
*/


