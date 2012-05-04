#include <avr/io.h>
#include <avr/interrupt.h> 
#include "styr_spi.h"
#include "styr_tolka_data.h"
#include "motor_styrning.h"
#include "styr_specialkommando.h"

volatile int a=0;


int main(void){


		DDRB |= (1<<DDB6); //1=ut bit7=SCLK,1 bit6=MISO,0 bit5=MOSI bit4=SS
		SPCR |= (1<<SPIE) | (1<<SPE);  
		//bit7 SPI interrupt enable bit6 SPI enable bit5 LSB first bit4 slave 
		//bit3:2 SPI-mode 3, bit1:0 gör inget
		DDRD |= (1<<DDD6); 	//REQ
		MCUCR |= (1<<ISC00) | (1<<ISC11);	//interrupt när man drar i spaken
										//eller när man släpper knappen
		GICR |= (1<<INT0) | (1<<INT1);	//tillåt spaken att fungera
		decide_mode();
		init();				//init motorer

		SREG = (SREG | 0x80);  //Tillåt globala interrupts
		
		

		while (a<255) {
				a++;
				if (a>254) {
						a = 1;
						//forward(0x0F);
						//specialkommando();
						//stop(0);
						
				}
		}
		

}



