#include <avr/io.h>
#include <avr/interrupt.h> 
int komm_init();
//SPI för Kruse (master)

volatile int a=0;

int main(){

		//volatile char SPIF;		//SPI interrupt flag
		

		
		
		komm_init();


				while (a<255) {
						a++;
						if (a>254) {
							a = 1;
						}
				}
				
				
				return 0;

	

return 0;
}
