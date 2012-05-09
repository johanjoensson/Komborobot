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
 * Anv�nds d� data ska f�rmedlas till andra enheter p� roboten. S�tter in header
 * i SPDR och s�tter requestsignalen till kommunikationsenheten h�g. 
 *
 *Input: tom
 *Output: 0
 *
 */
 

int req_sending()
{
		
		SPDR= header;
			
		//H�g requestsignal till kommunikationsenhet
		PORTB |= (1<<PB3);
		return 0;
}

/* ISR(SPI_STC_vect)
 *
 * Avbrottsvektor som aktiveras d� en SPI-�verf�ring �r f�rdig. 
 * Skickar ocks� den andra halvan i �verf�ringen, databyten.
 *
 *
 *
 */


ISR(SPI_STC_vect) //sensor REQ
{
		//REQ signal s�tts l�g
		PORTB &= ~(1<<PB3); 	
	
		//Den mottagna headern sparas
		header=SPDR;
	
		//Den data som ska skickas skrivs till SPI dataregistret
		SPDR=data;			
	
		//REQ-signal s�tts h�g
		PORTB |= (1<<PB3); 	
	
		//O�ndlig loops som v�ntar p� att �verf�ring ska bli f�rdig
		while(!(SPSR & (1<<SPIF))){
				;
		}
	
		//Request signal s�tt l�g
		PORTB &= ~(1<<PB3); 		
		
		//Den mottagna datan sparas
		data=SPDR;
	
		//0 s�tts in i SPI dataregistret
		SPDR=0x00;
	
		//Header -och databyten tolkas
		tolka_data();

}

/*****************************************************************************\
	Skicka header 0x2F till sensorenheten f�r att st�lla in linjesensorernas
	tr�sklingsniv� (niv�n skickas som data).
\*****************************************************************************/

void tolka_data()
{
		if(header==0x2F){		//st�ll in tr�skelniv� direkt
				level=data;
		}
}


//*auto_mode beskriver huruvida roboten �r i autonomt l�ge eller ej.
//INT0 �r programmerat f�r att ge avbrott p� f�r�ndring p� spaken f�r att byta l�ge*

ISR(INT0_vect)
{
		decide_mode();
}
	
/*void decide_mode()
 *
 *�ndrar l�get fr�n fj�rrstyrt till autonomt och viceverca
 *Input: Inget
 *Output: Void
 */
 
void decide_mode()
{
		if(0x04==(PIND & 0x04)){
				auto_mode=1;
		}
		else if(0x00==(PIND & 0x04)){
				auto_mode=0;
		}
}
