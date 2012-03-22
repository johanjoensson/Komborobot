//F�r att skicka en header+data byte, stoppa in v�rdena i variabel header respektive data och tillkallafkn req_sending

#include <avr/io.h>
#include <avr/interrupt.h> 


unsigned char header;
unsigned char data;
int transfer_count=0;
volatile int a=0;
int req_sending(); //funktion

int main(void){


		DDRB |= (1<<DDB6) | (1<<DDB3); //1=ut bit7=SCLK,1 bit6=MISO,0 bit5=MOSI bit4=SS
		SPCR |= (1<<SPIE) | (1<<SPE) | (1<<SPR1) | (1<<SPR0);  //bit7: SPI interrupt enable bit6 SPI enable bit5 LSB first bit4 slave bit3:2 SPI-mode 3, bit1:0 g�r inget
		DDRD |= (1<<DDD7) | (1<<DDD6) | (1<<DDD5) | (1<<DDD4);		//PWM v�nster, REQ, b5:4 riktning, Spak p� INT0
		SPDR=0x00;
		SREG = (SREG | 0x80);  //Till�t globala interrupts

		//O�ndlig loop som inte �r lika aktuell i drivande l�ge
		while (a<255) {
				a++;
				if (a>254) {
						a = 1;
				}
				else if(a==10){
					header=0x20;
					data=0xCD;
					req_sending();
				}
				else if(a=20){
					header=0x40;
					data=0xBB;
					req_sending();
				}
			
			PORTA=header;
			PORTA=data;
				
		}
		return 0;

}


ISR(SPI_STC_vect) //�verf�ring f�rdig
{
		

		switch(transfer_count){
		case 0:
				PORTD &= ~(1<<PD6); 	//REQ low
				header=SPDR;			//spara mottagen header
				PORTA=header;
				SPDR=data;			//V�lj data f�r n�sta �verf�ring
				PORTA=data;
				PORTD = (1<<PD6); 	//REQ high
				transfer_count=1;
				PORTD &= ~(1<<PD6); 	//REQ low
				break;
		case 1:
				transfer_count=0;
				data=SPDR;			//spara mottagen data
				PORTA=data;
				SPDR=0x00;
				break;
		}

}


int req_sending()
{
		SPDR=header;
		PORTA=header;
		PORTD |= (1<<PD6); //skicka req
		return 0;
}


