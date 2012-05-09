/******************************************************************************
 * Datum 09/05/2012
 * 
 * 
 * Skapad av: Markus Falck
 *
 * Redigerad: 09/05/2012
 *****************************************************************************/


#include <avr/io.h>
#include <avr/interrupt.h>

/* Initiering av kommunikationsenheten. Diverse olika kontrollregister initieras. 
 *
 * Tar inga inparametrar
 * Returnerar alltid 0.
 *
 */

int komm_init()
{
		//Riktningsregister f�r port B.
		DDRB |= (1<<DDB7) | (1<<DDB5) | (1<<DDB4) | (1<<DDB3); 
	
		//Riktningsregister f�r port D.
		DDRD |= (1<<DDD6);
		
		//Kontrollregister f�r SPI-kommunikation.
		SPCR |= (1<<SPE) | (1<<MSTR);
		
		//Kontrollregister f�r bland annat avbrott.
		MCUCR |=  (1<<ISC11) | (1<<ISC10) | (1<<ISC00); 
	
		//Kontrollregister f�r bland annat avbrott.
		MCUCSR |= (1<<ISC2);						
		
		//Avbrottsregister
		GICR |= (1<<INT1) | (1<<INT0) | (1<<INT2);
	
		//B�da SS signalerna s�tts h�ga
		PORTB |= (1<<PB3) | (1<<PB4);
	
		//Globala avbrott till�ts
		SREG = (SREG | 0x80);
		return 0;
}
