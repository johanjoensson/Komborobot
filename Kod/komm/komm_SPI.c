#include <avr/io.h>
#include <avr/interrupt.h> 


int tolka_och_skicka(unsigned char header,unsigned char data);
int send_to_sensor(unsigned char header,unsigned char data);
int send_to_styr(unsigned char header,unsigned char data);
int send_to_PC(unsigned char header,unsigned char data);
int transmit_data(unsigned char data_send);


ISR(INT2_vect) //sensor REQ,
{
		//SREG = (SREG & 0x7F);	//stäng av globala avbrott
		GICR &= ~(1<<INT1) & ~(1<<INT0) & ~(1<<INT2);	//stäng av alla avbrott utom reset.
		send_to_sensor(0x00,0x00);
		GICR |= (1<<INT1) | (1<<INT0) | (1<<INT2);	//tillåt avbrott
}

ISR(INT1_vect) //styr REQ
{
		//SREG = (SREG & 0x7F);	//stäng av globala avbrott
		GICR &= ~(1<<INT1) & ~(1<<INT0) & ~(1<<INT2);	//stäng av alla avbrott utom reset.
		send_to_styr(0x00,0x00);
		GICR |= (1<<INT1) | (1<<INT0) | (1<<INT2);	//tillåt avbrott
		
}


int tolka_och_skicka(unsigned char header,unsigned char data)
{		
		unsigned char mal_enhet = (header & 0xE0);
		unsigned char temp_header;
		unsigned char temp_data;
		switch(mal_enhet){
		case 000:
				break;
		case 001:
				send_to_sensor(header,data);
				break;

		case 010:
				send_to_styr(header,data);
				break;
		case 100:
				send_to_PC(header,data);
				break;
		case 110:
				temp_header=header;
				temp_data=data;
				send_to_styr(header,data);
				send_to_PC(temp_header,temp_data);
				break;
		}
		return 0;
}





int send_to_styr(unsigned char header,unsigned char data)
{
		PORTB &= ~(1<<PB4);		//Låg SS till styrenhet
		header=transmit_data(header); //Byt header
		int i=0;
		while(i<2){
				i++;
		}
		data=transmit_data(data);			//Byt data
		PORTB |= (1<<PB4);
		tolka_och_skicka(header,data);
		return 0;
		}

int send_to_sensor(unsigned char header,unsigned char data)
{
		PORTB &= ~(1<<PB3);		//Låg SS till sensorenhet
		header=transmit_data(header); 	//Byt header
		int i=0;
		while(i<2){
				i++;
		}								//Ge andra enheten tid att spra undan data
		data=transmit_data(data);		//Byt data
		PORTB |= (1<<PB3);		//Hög SS till sensorenhet
		tolka_och_skicka(header,data);	//Skicka data vidare
		return 0;
}

int send_to_PC(unsigned char header,unsigned char data)
{
		//KOD

		return 0;
}
		

int transmit_data(unsigned char data_send)
{
		SPDR=data_send;				//välj data som skickas och starta överföring
		while(!(SPSR & (1<<SPIF))) 	//vänta på att alla data skiftats
		{
		;
		}

		return SPDR;  //clears SPIF och returnerar data som skiftats in
}

