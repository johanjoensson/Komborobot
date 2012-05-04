#include <avr/io.h>
#include <avr/interrupt.h> 


int tolka_och_skicka(unsigned char header,unsigned char data);
int send_to_sensor(unsigned char header,unsigned char data);
int send_to_styr(unsigned char header,unsigned char data);
int send_to_PC(unsigned char header,unsigned char data);
int transmit_data(unsigned char data_send);

void USARTInit(uint16_t ubrr_value);
char USARTReadChar();
void USART_write_char(unsigned char data);


ISR(INT2_vect) //sensor REQ,
{
		send_to_sensor(0x00,0x00);
}

ISR(INT1_vect) //styr REQ
{
		send_to_styr(0x00,0x00);	
}


int tolka_och_skicka(unsigned char header,unsigned char data)
{		
		unsigned char mal_enhet;
		unsigned char temp_header;
		unsigned char temp_data;
		
		mal_enhet = (header & 0xE0);


		if(mal_enhet==0x20){
				send_to_sensor(header,data);
				}
		else if(mal_enhet==0x40){
				send_to_styr(header,data);
				}
		else if(mal_enhet==0x80){
				send_to_PC(header,data);
				}
		else if(mal_enhet==0xC0){
				temp_header=header;
				temp_data=data;
				send_to_styr(header,data);
				send_to_PC(temp_header,temp_data);
		}
		return 0;
}





int send_to_styr(unsigned char header,unsigned char data)
{
		PORTB &= ~(1<<PB4);		//L�g SS till styrenhet
		header=transmit_data(header); //Byt header
		while(!(GIFR & (1<<INTF1))) 	//v�nta p� att andra enheten l�st data
		{
		;
		}
		GIFR = GIFR & 0x80;
		data=transmit_data(data);			//Byt data
		PORTB |= (1<<PB4);
		tolka_och_skicka(header,data);
		return 0;
		}

int send_to_sensor(unsigned char header,unsigned char data)
{
		PORTB &= ~(1<<PB3);		//L�g SS till sensorenhet
		header=transmit_data(header); 	//Byt header
		while(!(GIFR & (1<<INTF2))) 	//v�nta p� att andra enheten l�st data
		{
		;
		}
		GIFR = GIFR & 0x20;						
		data=transmit_data(data);		//Byt data
		PORTB |= (1<<PB3);		//H�g SS till sensorenhet
		
		tolka_och_skicka(header,data);	//Skicka data vidare
		return 0;
}

int send_to_PC(unsigned char header,unsigned char data)
{
		
/* Recieve data */
	USART_write_char(header);
	while(!(UCSRA & (1<<TXC))) //While transmission not complete
   	{
      ;
   	}
	USART_write_char(data);
	while(!(UCSRA & (1<<TXC))) //While transmission not complete
   	{
      ;
   	}

/* Reset flags */
	
	UCSRA = (1<<TXC);
	UCSRB = (1<<TXEN);
	UCSRB = (1<<RXCIE)|(1<<RXEN);
	PORTD = (1<<PIND5);

	return 0;
}
		

int transmit_data(unsigned char data_send)
{
		SPDR=data_send;				//v�lj data som skickas och starta �verf�ring
		while(!(SPSR & (1<<SPIF))) 	//v�nta p� att alla data skiftats
		{
		;
		}
		return SPDR;  //clears SPIF och returnerar data som skiftats in
}

