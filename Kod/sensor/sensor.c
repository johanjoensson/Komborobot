#include <avr/io.h>
#include <avr/interrupt.h> 
#include "sensor_spi.h"
#include "sensor_init.h"
#include "sensor_ad.h"
#include "linjeskillnad.h"
#include "upptack_tejp.h"
#include "displayenhet.h"
volatile int a=0;

int main(void){
		
		spi_init();	
		ad_init();
		display_setup();
		SREG |= (1<<7);				//Globala avbrott	


				while (a<255) {
						a++;
						if (a>254) {
								a = 1;
						}
				}
		return 0;

}
