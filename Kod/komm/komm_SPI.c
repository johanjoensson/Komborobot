/******************************************************************************
 * Datum 09/05/2012
 * 
 * Alla avbrottsvektorer som anv�nds av kommunikationsenheten. Inneh�ller ocks�
 * de funktioner som kr�vs f�r att styra SPI-kommunikationen.
 *
 * Skapad av: Markus Falck
 *
 * Redigerad: 09/05/2012
 *****************************************************************************/
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

//Avbrottsvektor f�r sensorenhetens request-signal.
ISR(INT2_vect)
{
		send_to_sensor(0x00,0x00);
}
//Avbrottsvektor f�r styrenhetens request-signal.
ISR(INT1_vect)
{
		send_to_styr(0x00,0x00);	
}

/* int tolka_och_skicka(unsigned char header,unsigned char data)
 *
 * Tolkar den mottagna headerbyten som �terkommer vid varje �verf�ring. Ska databyten 
 * vidare s� skickas header och data till den eller de enheter som datat �r adresserad till.
 *
 * Input: Headerbyte och databyte
 * Output: 0
 */ 

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

/* send_to_styr(unsigned char header,unsigned char data)
 * 
 * Skickar header- och databyte till styrenheten. 
 * 
 * Input: Headerbyte och databyte
 * Output: 0
 */ 


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


/* send_to_sensor(unsigned char header,unsigned char data)
 * 
 * Skickar header- och databyte till sensorenheten. 
 * 
 * Input: Headerbyte och databyte
 * Output: 0
 */ 

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


/* --------------------------------------------------
* SEND_TO_PC(unsigned char header,unsigned char data)
*
* Skickar i tur och ordning orden 'header' och 'data' 
* via bl�tand till PC.
*
* Efter s�ndning �terst�lls enheten att vara redo att
* ta emot data fr�n PC.
* ---------------------------------------------------*/
int send_to_PC(unsigned char header,unsigned char data)
{
		
/* Send data */
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

/*
	volatile int ctr = 0;
	while(ctr<100){
		ctr++;
	}
*/

/* Reset flags */
//	UCSRA = (1<<TXC);
//	UCSRB = (1<<TXEN);
	UCSRB = (1<<RXCIE)|(1<<RXEN);
	PORTD &= 0xEF; //RTS = 0
	PORTD |= (1<<PIND5); //CTS = 1

	return 0;
}

/* int transmit_data(unsigned char data_send)
 * 
 * Skriver till SPI dataregistret SPDR, vilket p�b�rjar �verf�ringen
 * av 1 byte data.
 *
 */ 
		

int transmit_data(unsigned char data_send)
{
		SPDR=data_send;				//v�lj data som skickas och starta �verf�ring
		while(!(SPSR & (1<<SPIF))) 	//v�nta p� att alla data skiftats
		{
		;
		}
		return SPDR;  //clears SPIF och returnerar data som skiftats in
}

