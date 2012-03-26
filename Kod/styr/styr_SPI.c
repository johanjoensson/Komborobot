#include <avr/io.h>
#include <avr/interrupt.h> 
#include "styr_spi.h"

int transfer_count=0;



ISR(SPI_STC_vect) //sensor REQ
{
		

		switch(transfer_count){
		case 0:
				PORTD &= ~(1<<PD6); 	//REQ low
				header=SPDR;		//spara mottagen header
				SPDR=data;			//Välj data för nästa överföring
				PORTD = (1<<PD6); 	//REQ high
				transfer_count=1;
				PORTD &= ~(1<<PD6); 	//REQ low
				break;
		case 1:
				data=SPDR;			//spara mottagen data
				SPDR=0x00;
				transfer_count=0;
				break;
		}

}


int req_sending()
{
		SPDR= header;
		PORTD |= (1<<PD6); //skicka req
		return 0;
}
