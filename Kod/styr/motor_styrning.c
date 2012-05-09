/*
 * ===============================================================================
 *
 *       Filename:  motor_styrning.c
 *
 *    Description:  styr motorerna i manuellt läge
 *        Created:  2012-03-31
 *
 *         Author:  Simon Wallin & Simon Larsson
 *   Organization:  Grupp 17 TSEA27
 *
 * ===============================================================================
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "motor_styrning.h" 
int trim;
int speed_right;
int speed_left;

/*******************************************************************************
*void init() är en initsiering som anropas för sätta alla register rätt
*så att körning ska möjliggöras genom att ställa in vilken typ av PWM signal
*som ska användas och vilka register som ska räkna
*******************************************************************************/
 
void init(){
			TCCR0 |= (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00);
			TCCR2 |= (1<<WGM20) | (1<<WGM21) | (1<<COM21) | (1<<CS20);
			DDRD |= (1<<DDD7) | (1<<DDD5) | (1<<DDD4);
			DDRB |= (1<<DDB3);
			PORTD &= 0xCF;
	}	

/*******************************************************************************
*Funktionerna nedan anropar en subrutin set_speed som sätter olka hastighet 
*på hjulen för att jämna ut felet och få roboten att gå rakare.
*Funktionerna sätter registrena som styr motorerna.
*OCR0 = Hastighet höger hjul
*OCR2 = Hastighet vänster hjul
*PORTD = Sätter riktningen på hjulen
*Hjulen kör långasammare bakåt och därför adderas de/det hjul som går bakåt
*med 6
*******************************************************************************/

void stop(unsigned char speed)
 	{
		OCR0 = 0x00;
		OCR2 = 0x00;
	}

void forward(unsigned char speed)
	{
		set_speed(speed);
		PORTD &= 0xCF;	//ställer riktning framåt
		if (trim < 0)
		{
			OCR0 = (speed_right);		//höger
			OCR2 = (speed_left);		//vänster
		}
		else
		{
			OCR0 = (speed_right);		
			OCR2 = (speed_left);		
		}
	}

void forward_left(unsigned char speed)
	{
		set_speed(speed);
		PORTD &= 0xCF;
		OCR0 = (speed_right);		
		OCR2 = (speed_left)-6;		
	}

void forward_right(unsigned char speed)
	{	set_speed(speed);
		PORTD &= 0xCF;
		OCR0 = (speed_right)-6;		
		OCR2 = (speed_left);		
	}

void rotate_left(unsigned char speed)
	{
		set_speed(speed);
		PORTD &= 0xEF;
		PORTD |= (1<<PD5);
		OCR0 = (speed_right);		 
		OCR2 = (speed_left)+6;		 
	}

void rotate_right(unsigned char speed)
	{	
		set_speed(speed);
		PORTD &= 0xDF;
		PORTD |= (1<<PD4);
		OCR0 = (speed_right)+6;		
		OCR2 = (speed_left);		
	}

void back(unsigned char speed)
	{	
		set_speed(speed);
		PORTD |= (1<<PD4) | (1<<PD5);
		OCR0 = (speed_right)+6;		
		OCR2 = (speed_left)+6;		
	}
void set_speed_left(unsigned char speed)
	{
		set_speed(speed);
		PORTD &= 0xCF;	
		OCR2 = (speed_left);		
	}
void set_speed_right(unsigned char speed){
		set_speed(speed);
		PORTD &= 0xCF;	
		OCR0 = (speed_right);		
	}

/*******************************************************************************
*Trim funktionerna ökar 
*
*
*
*
*******************************************************************************/
void trim_left(){
		if(trim>-16){
			trim=trim-1;
		}
	}
void trim_right()		// ökar höger speed speed med 1
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
			speed_right = speed - 2;			// sätter hastigheten för rak körning höger hjul
			speed_left = speed + (2+trim);		// sätter hastigheten för rak körning vänser hjul
		}
		else
		{
			speed_right = speed - (2+trim);		// sätter hastigheten för rak körning höger hjul
			speed_left = speed + 2;				// sätter hastigheten för rak körning vänser hjul
		}
	}
		
/*
void decide_speed(unsigned char speed)
{
		
}
*/


