#include <avr/io.h>
#include <avr/interrupt.h> 
#include "styr_spi.h"
#include "styr_tolka_data.h"
#include "regulator.h"
#include "motor_test.h"

volatile int a=0;


int main(void){


		DDRB |= (1<<DDB6); //1=ut bit7=SCLK,1 bit6=MISO,0 bit5=MOSI bit4=SS
		SPCR |= (1<<SPIE) | (1<<SPE) | (1<<SPR0);  //bit7 SPI interrupt enable bit6 SPI enable bit5 LSB first bit4 slave bit3:2 SPI-mode 3, bit1:0 gör inget
		DDRD |= (1<<DDD6); //REQ
		init();

		SREG = (SREG | 0x80);  //Tillåt globala interrupts
		
		

		while (a<255) {
				a++;
				if (a>254) {
						a = 1;
						//forward(0x0F);
						
				}
		}
		

}



