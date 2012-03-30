#include <avr/io.h>
#include <avr/interrupt.h> 
#include "styr_spi.h"

int transfer_count=0;



ISR(SPI_STC_vect) //sensor REQ
{
		

	
				PORTD &= ~(1<<PD6); 	//REQ low
				header=SPDR;		//spara mottagen header
				SPDR=data;			//Välj data för nästa överföring
				PORTD = (1<<PD6); 	//REQ high
				
				while(!(SPSR & (1<<SPIF))){
				;
				}
				PORTD &= ~(1<<PD6); 	//REQ low
				
				data=SPDR;			//spara mottagen data
				SPDR=0x00;
			
			
	

}


int req_sending()
{
		SPDR= header;
		PORTD |= (1<<PD6); //skicka req
		return 0;
}
