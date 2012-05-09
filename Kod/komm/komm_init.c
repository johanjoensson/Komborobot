
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
		//Riktningsregister för port B.
		DDRB |= (1<<DDB7) | (1<<DDB5) | (1<<DDB4) | (1<<DDB3); 
	
		//Riktningsregister för port D.
		DDRD |= (1<<DDD6);
		
		//Kontrollregister för SPI-kommunikation.
		SPCR |= (1<<SPE) | (1<<MSTR);
		
		//Kontrollregister för bland annat avbrott.
		MCUCR |=  (1<<ISC11) | (1<<ISC10) | (1<<ISC00); 
	
		//Kontrollregister för bland annat avbrott.
		MCUCSR |= (1<<ISC2);						
		
		//Avbrottsregister
		GICR |= (1<<INT1) | (1<<INT0) | (1<<INT2);
	
		//Båda SS signalerna sätts höga
		PORTB |= (1<<PB3) | (1<<PB4);
	
		//Globala avbrott tillåts
		SREG = (SREG | 0x80);
		return 0;
}

