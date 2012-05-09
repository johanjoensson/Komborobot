
/******************************************************************************
 * Datum 09/05/2012
 * 
 * Mainfunktion för kommunikationsenhet. 
 *
 * Skapad av: Markus Falck
 *
 * Redigerad: 09/05/2012
 *****************************************************************************/


#include <avr/io.h>
#include <avr/interrupt.h> 


int komm_init();

void USARTInit(uint16_t ubrr_value);

//Variabel för att motverka att kompilatorn tar bort en oändlig loop.
volatile int a=0;

int main(){
		
		//Initiering av kommunikationsenhet exklusive USART
		komm_init();
		
		//Initiering av USART
		USARTInit(8);


		while (a<255) {
				a++;
				if (a>254) {
					a = 1;
				
				}
		}
	
		return 0;
}

