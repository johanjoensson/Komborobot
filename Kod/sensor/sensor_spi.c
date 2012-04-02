#include <avr/io.h>
#include <avr/interrupt.h> 
#include "sensor_spi.h"

int req_sending()
{
		
		SPDR= header;
		PORTB |= (1<<PB3); //skicka req
		return 0;
}

ISR(SPI_STC_vect) //sensor REQ
{
		



				PORTB &= ~(1<<PB3); 		//REQ low
				header=SPDR;			//spara mottagen header
				SPDR=data;				//V�lj data f�r n�sta �verf�ring
				PORTB |= (1<<PB3); 		//REQ high
				while(!(SPSR & (1<<SPIF)))
				{
				;
				}	
				PORTB &= ~(1<<PB3); 		//REQ low
				data=SPDR;			//spara mottagen data
				SPDR=0x00;

}

//*auto_mode beskriver huruvida roboten �r i autonomt l�ge eller ej.
//INT0 �r programmerat f�r att ge avbrott p� f�r�ndring p� spaken f�r att byta l�ge*

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
