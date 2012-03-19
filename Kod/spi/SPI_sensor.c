#include <avr/io.h>
#include <avr/interrupt.h> 



//SPI för sensor

unsigned char header=0x20;
unsigned char data;
int transfer_count=0;
int req_sending();
volatile int a=0;

int main(void){



		
		DDRB |= (1<<DDB6) | (1<<DDB3); //1=ut bit7=SCLK,1 bit6=MISO,0 bit5=MOSI bit4=SS1
		SPCR |= (1<<SPIE) | (1<<SPE) | (1<<DORD) | (1<<CPOL) | (1<<CPHA);  //bit7 SPI interrupt enable bit6 SPI enable bit5 LSB first bit4 slave bit3:2 SPI-mode 3, bit1:0 gör inget
		DDRD |= (1<<DDD2);		//Spak på INT0
		SFIOR |= (1<<PUD);
		
		SREG = (SREG | 0x80);  //Tillåt globala interrupts



				while (a<255) {
						a++;
						if (a>254) {
							a = 1;
						}
						else if (a==100)
						{
						req_sending();
						}
				}
		return 0;

}


ISR(SPI_STC_vect) //sensor REQ
{
		

		switch(transfer_count){
		case 0:
				header=SPDR;		//spara mottagen header
				SPDR=data;			//Välj data för nästa överföring
				PORTB &= ~(1<<PB3); //REQ low
				transfer_count=1;
				break;
		case 1:
				transfer_count=0;
				data=SPDR;			//spara mottagen data
				SPDR=0x00;
				break;
		}

}

int req_sending()
{
		SPDR= header;
		PORTB |= (1<<PB3); //skicka req
		return 0;
}
