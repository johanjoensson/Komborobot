#include <avr/io.h>
#include <avr/interrupt.h> 
int komm_init();
//SPI f�r Kruse (master)

volatile int a=0;

int main(){

		
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
