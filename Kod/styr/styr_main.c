#include <avr/io.h>
#include <avr/interrupt.h> 
#include "styr_spi.h"

volatile int a=0;


int main(void){


		DDRB |= (1<<DDB6) | (1<<DDB3); //1=ut bit7=SCLK,1 bit6=MISO,0 bit5=MOSI bit4=SS
		SPCR |= (1<<SPIE) | (1<<SPE) | (1<<SPR1) | (1<<SPR0);  //bit7 SPI interrupt enable bit6 SPI enable bit5 LSB first bit4 slave bit3:2 SPI-mode 3, bit1:0 gör inget
		DDRD |= (1<<DDD7) | (1<<DDD6) | (1<<DDD5) | (1<<DDD4);
		//PWM vänster, REQ, b5:4 riktning, Spak på INT0
		SPDR=0x20;
		SREG = (SREG | 0x80);  //Tillåt globala interrupts



		while (a<255) {
				a++;
				if (a>254) {
						a = 1;
				}
		}
		return 0;

}



