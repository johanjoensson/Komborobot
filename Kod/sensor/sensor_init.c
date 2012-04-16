#include <avr/io.h>
#include <avr/interrupt.h> 
#include "sensor_ad.h"
#include "sensor_spi.h"
#include "upptack_tejp.h"
		
void spi_init()
{		
		DDRB |= (1<<DDB6) | (1<<DDB3); //1=ut bit7=SCLK,1 bit6=MISO,0 bit5=MOSI bit4=SS1
		SPCR |= (1<<SPIE) | (1<<SPE) | (1<<SPR0);  
		//bit7 SPI interrupt enable bit6 SPI enable bit5 LSB first bit4 slave bit3:2 SPI-mode 3, bit1:0 g�r inget	
		GICR |= (1<<INT0);		//Till�t avbrott fr�n spak
		MCUCR |= (1<<ISC00);	//Spakavbrott triggar p� change
		decide_mode();
}


void ad_init()
{
		ADMUX |= (1<<ADLAR);				//V�nsterjustering
		ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
		//Till�t ADC avbrott, dela frekvens med 128

		DDRC |= (1<<DDC0) | (1<<DDC1) | (1<<DDC6) | (1<<DDC7);
		PORTC |= (1<<PC0) | (1<<PC1) | (1<<PC6) | (1<<PC7);
		TIMSK |= (1<<TOV1);			//Till�t interrupts p� timer overflow
		TCCR1B |= (1<<CS11);		//Prescaler delar med 8, startar r�knare
		level = 0xA8;				//Tr�skelv�rde
}
void upptack_tejp_init()
{
		TCCR1B |= (1<<CS12) | (1<<CS10); //L�t timer g� med fosc/1024					 

}

