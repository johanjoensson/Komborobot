#include <avr/io.h>
#include <avr/interrupt.h> 

int komm_init()
{
				
				DDRB |= (1<<DDB7) | (1<<DDB5) | (1<<DDB4) | (1<<DDB3); 
				//1=ut. bit7=SCLK,1 bit6=MISO,0 bit5=MOSI 
				//bit4=SS1(styr) bit3=SS2(sens) bit2=REQ(sens)
				DDRD |= (1<<DDD6) ; //bit3=REQ(styr) bit2=spak 
				//bit1=blue(trans) bit0=blue(read)
				SPCR |= (1<<SPE) | (1<<MSTR)| (1<<SPR1) | (1<<SPR0);
				MCUCR |=  (1<<ISC11) | (1<<ISC10) | (1<<ISC00); 
				//Interrupts på rising edge för INT1, INT0 triggar på change.
				MCUCSR |= (1<<ISC2);						//interrupts på rising för INT2
				GICR |= (1<<INT1) | (1<<INT0) | (1<<INT2); //Tillåt avbrott på 
				PORTB |= (1<<PB3) | (1<<PB4);	//Båda SS till hög
				SPDR=0x00;
				SREG = (SREG | 0x80);//Tillåt globala avbrott
				
				return 0;
}
