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
#include "sensor_spi.h"
#include "sensor_ad.h"

void tolka_data();

/* int req_sending()
 *
 * Används då data ska förmedlas till andra enheter på roboten. Sätter in header
 * i SPDR och sätter requestsignalen till kommunikationsenheten hög. 
 *
 *Input: tom
 *Output: 0
 *
 */
 

int req_sending()
{
		
		SPDR= header;
			
		//Hög requestsignal till kommunikationsenhet
		PORTB |= (1<<PB3);
		return 0;
}

/* ISR(SPI_STC_vect)
 *
 * Avbrottsvektor som aktiveras då en SPI-överföring är färdig. 
 * Skickar också den andra halvan i överföringen, databyten.
 */


ISR(SPI_STC_vect) //sensor REQ
{
		//REQ signal sätts låg
		PORTB &= ~(1<<PB3); 	
	
		//Den mottagna headern sparas
		header=SPDR;
	
		//Den data som ska skickas skrivs till SPI dataregistret
		SPDR=data;			
	
		//REQ-signal sätts hög
		PORTB |= (1<<PB3); 	
	
		//Oändlig loops som väntar på att överföring ska bli färdig
		while(!(SPSR & (1<<SPIF))){
				;
		}
	
		//Request signal sätt låg
		PORTB &= ~(1<<PB3); 		
		
		//Den mottagna datan sparas
		data=SPDR;
	
		//0 sätts in i SPI dataregistret
		SPDR=0x00;
	
		//Header -och databyten tolkas
		tolka_data();

}

/*****************************************************************************\
	Skicka header 0x2F till sensorenheten för att ställa in linjesensorernas
	trösklingsnivå (nivån skickas som data).
\*****************************************************************************/

void tolka_data()
{
		if(header==0x2F){		//ställ in tröskelnivå direkt
				level=data;
		}
}


//auto_mode beskriver huruvida roboten är i autonomt läge eller ej.
//INT0 är programmerat för att ge avbrott på förändring på spaken då byts läge

ISR(INT0_vect)
{
		decide_mode();
}
	
void decide_mode()
{
		if(0x04==(PIND & 0x04)){
				auto_mode=1;
		}
		else if(0x00==(PIND & 0x04)){
				auto_mode=0;
		}
}
