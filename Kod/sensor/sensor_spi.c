#include <avr/io.h>
#include <avr/interrupt.h> 
#include "sensor_spi.h"

int transfer_count=0;

int req_sending()
{
		SPDR= header;
		PORTB |= (1<<PB3); //skicka req
		return 0;
}

ISR(SPI_STC_vect) //sensor REQ
{
		


		switch(transfer_count){
		case 0:
				PORTB &= ~(1<<PB3); 		//REQ low
				header=SPDR;			//spara mottagen header
				SPDR=data;				//Välj data för nästa överföring
				PORTB |= (1<<PB3); 		//REQ high
				transfer_count=1;
				PORTB &= ~(1<<PB3); 		//REQ low
				break;
				
		case 1:
				data=SPDR;			//spara mottagen data
				SPDR=0x00;
				transfer_count=0;
				break;
		}

}
