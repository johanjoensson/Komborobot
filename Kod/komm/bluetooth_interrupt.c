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
	while(!(UCSRA & (1<<RXC))){ //While recieve not complete
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
* - Baude rate:  
* ---------------------------------------------*/
void USARTInit(uint16_t ubrr_value)
{
	unsigned int ubrr = ubrr_value;
	/* Set baud rate */
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;
	/* Enable receiver, avbrott OK*/
	UCSRB = (1<<RXCIE)|(1<<RXEN);
	/* Set frame format: 8data 1 stoppbit*/
	UCSRC = (1<<URSEL)|(3<<UCSZ0);

	SREG |= 0x80; // Global interrupt enable

	DDRD |= 0x32; //Output på RTS,CTS och RXD
    PORTD |= (1<<PIND5); //CTS

}

char USARTReadChar()
{	
	unsigned char data = 'a';
	
	PORTD |= (1<<PIND5);

   while(!(UCSRA & (1<<RXC)))
   {
      //Do nothing
   }

	data = UDR;

	USARTReadChar();

	return data;
}


void USART_write_char(unsigned char data)
{
	UCSRB = (1<<TXEN);

   while(!(UCSRA & (1<<UDRE)))
   {
      ;//Do nothing
   }

//	UCSRA = (1<<TXC);
	PORTD &= 0xDF; //CTS = 0
  	PORTD |= (1<<PIND4); //RTS = 1
    UDR = data;
}
