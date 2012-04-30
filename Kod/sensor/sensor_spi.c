#include <avr/io.h>
#include <avr/interrupt.h> 
#include "sensor_spi.h"
#include "sensor_ad.h"

void tolka_data();

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
				SPDR=data;				//Välj data för nästa överföring
				PORTB |= (1<<PB3); 		//REQ high
				while(!(SPSR & (1<<SPIF)))
				{
				;
				}	
				PORTB &= ~(1<<PB3); 		//REQ low
				data=SPDR;			//spara mottagen data
				SPDR=0x00;

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


//*auto_mode beskriver huruvida roboten Šr i autonomt lŠge eller ej.
//INT0 Šr programmerat fšr att ge avbrott pŒ fšrŠndring pŒ spaken fšr att byta lŠge*

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
