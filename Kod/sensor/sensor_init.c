#include <avr/io.h>
#include <avr/interrupt.h> 
#include "sensor_ad.h"
#include "sensor_spi.h"
#include "upptack_tejp.h"
		
void spi_init()
{		
		DDRB |= (1<<DDB6) | (1<<DDB3); //1=ut bit7=SCLK,1 bit6=MISO,0 bit5=MOSI bit4=SS1
		SPCR |= (1<<SPIE) | (1<<SPE);  
		//bit7 SPI interrupt enable bit6 SPI enable bit5 LSB first bit4 slave bit3:2 SPI-mode 3, bit1:0 gör inget	
		GICR |= (1<<INT0);		//TillŒt avbrott frŒn spak
		MCUCR |= (1<<ISC00);	//Spakavbrott triggar pŒ change
		decide_mode();
}

void ad_init()
{
		ADMUX |= (1<<ADLAR);				//Vänsterjustering
		ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
		//Tillåt ADC avbrott, dela frekvens med 128
		
		OCR1A=0x1745;
		OCR1B=0x2E8A;
		DDRC |= (1<<DDC0) | (1<<DDC1) | (1<<DDC6) | (1<<DDC7);
		PORTC |= (1<<PC0) | (1<<PC1) | (1<<PC6) | (1<<PC7);
		TIMSK |= (1<<TOV1)| (1<<OCIE1A) | (1<<OCIE1B);			//Tillåt interrupts på timer overflow
		TCCR1B |= (1<<CS11);		//Prescaler delar med 8, startar räknare
		level = 0xB0;				//Tröskelvärde
}

