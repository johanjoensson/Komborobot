#include <avr/io.h>
#include <avr/interrupt.h> 
#include "sensor_spi.h"

int req_sending()
{
		
		if(auto_mode==1){
				header=0xC0;	//Om auto_mode är på, skicka till PC och styrenhet
			
		}
		else if(auto_mode==0){
				header=0x80;	//Om auto_mode är av, skicka bara till PC
				
		}
		
		SPDR= header;
		PORTB |= (1<<PB3); //skicka req
		return 0;
}

ISR(SPI_STC_vect) //sensor REQ
{
		



				PORTB &= ~(1<<PB3); 		//REQ low
				header=SPDR;			//spara mottagen header
				SPDR=data;				//V‰lj data fˆr n‰sta ˆverfˆring
				PORTB |= (1<<PB3); 		//REQ high
				while(!(SPSR & (1<<SPIF)))
				{
				;
				}	
				PORTB &= ~(1<<PB3); 		//REQ low
				data=SPDR;			//spara mottagen data
				SPDR=0x00;

}

//*auto_mode beskriver huruvida roboten är i autonomt läge eller ej.
//INT0 är programmerat för att ge avbrott på förändring på spaken för att byta läge*

ISR(INT0_vect)
{
		if(0x04==(PORTD & 0x04)){
				auto_mode=1;
		}
		else if(0x00==(PORTD & 0x04)){
				auto_mode=0;
		}
}
	

