/*
Skapad av: Tobias
Datum: 19/3 2012

Koden st�ller in de interna och externa muxarna f�r adomvandlaren.
Omvandlingskedjan startas av en intern timer ca var 35 ms. (16Mhz).
De digitala omvandlas till avst�nd i convert_to_distance (beh�ver skrivas)
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "sensor_ad.h"

volatile int a=0;

int main(void){
		
		ADMUX |= (1<<ADLAR);				//V�nsterjustering
		ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
		//Till�t ADC avbrott, dela frekvens med 128
		DDRC |= (1<<DDC0) | (1<<DDC1) | (1<<DDC6) | (1<<DDC7);
		PORTC |= (1<<PC0) | (1<<PC1) | (1<<PC6) | (1<<PC7);
		TCCR1B |= (1<<CS11);		//Prescaler delar med 8, startar r�knare
		TIMSK |= (1<<TOV1);			//Till�t interrupts p� timer overflow
		SREG |= (1<<7);				//Globala avbrott
		//test
		DDRD = 0xFF;
		//test end

				while (a<255) {
						a++;
						if (a>254) {
							a = 1;
						}
				}
}


ISR(ADC_vect)
{
		start_next_ad();
}


ISR(TIMER1_OVF_vect)
{
		ADCSRA |= (1<<ADSC);
		count=0;			//starta om
}

