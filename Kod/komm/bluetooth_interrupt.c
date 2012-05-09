/* ---------------------------------------------
 * Datum 2012-03-25
 * Skapad av Mattias Jansson
 *
 * Blåtandskommunikation
 * ---------------------------------------------*/

#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/iom16.h>

int tolka_och_skicka(unsigned char header,unsigned char data);
void pass_from_PC(unsigned char header,unsigned char data);

/* ---------------------------------------------
* ISR(USART_RXC_vect)
*
* Avbrott som inträffar då data tagits emot från PC.
* Första ordet sparas undan, och väntar på att ta
* emot nästa.
* ---------------------------------------------*/
ISR(USART_RXC_vect){

	unsigned char header;
	unsigned char data;

	header = UDR;
	int j = 0;
	while(!(UCSRA & (1<<RXC))){ 
		;
	}
	data = UDR;
	tolka_och_skicka(header,data);
}

/* ---------------------------------------------
* USARTInit(uint16_t ubrr_value)
*
* Fukntionen initierar USART-kommunikation med firefly-
* enheten, med följande parametrar:
*
* - Recieve enable, samt recieve complete-avbrott.
* ---------------------------------------------*/
void USARTInit(uint16_t ubrr_value)
{
	unsigned int ubrr = ubrr_value;
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;
	UCSRB = (1<<RXCIE)|(1<<RXEN);
	UCSRC = (1<<URSEL)|(3<<UCSZ0);

	SREG |= 0x80; 

	DDRD |= 0x32; 
    PORTD |= (1<<PIND5);

}

/* ---------------------------------------------
 * char USARTReadChar()
 *
 * Läser data från USARTens dataregister. 
 * Returnerar: char data
 * ---------------------------------------------*/
char USARTReadChar()
{	
	unsigned char data = 'a';
	
	PORTD |= (1<<PIND5);

   while(!(UCSRA & (1<<RXC)))
   {    
   }

	data = UDR;

	USARTReadChar();

	return data;
}

/* ---------------------------------------------
 * void USART_write_char(unsigned char data)
 *
 * Skriver data till USARTens dataregister
 * Parametrar: unsigned char data: Data som ska
                                    skrivas
 * ---------------------------------------------*/
void USART_write_char(unsigned char data)
{
	UCSRB = (1<<TXEN);

   while(!(UCSRA & (1<<UDRE)))
   {
      ;
   }

	PORTD &= 0xDF; 
  	PORTD |= (1<<PIND4); 
    UDR = data;
}
