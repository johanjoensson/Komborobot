#include <avr/io.h>
#include <avr/interrupt.h> 
#include "styr_spi.h"
#include "styr_tolka_data.h"



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
			
			
				tolka_data();

}


void req_sending()
{
		SPDR= header;
		PORTD |= (1<<PD6); //skicka req
}

ISR(INT0_vect)
{
		if(auto_mode==1){
				auto_mode=0;
		}
		else if(auto_mode==0){
				auto_mode=1;
		}
}

