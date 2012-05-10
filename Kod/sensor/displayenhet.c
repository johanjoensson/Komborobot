/* ---------------------------------------------
 * Datum 2012-05-01
 * Skapad av Mattias Jansson
 *
 * Hanterar displayenheten
 * ---------------------------------------------*/

#include <avr/io.h>
#include<stdio.h>
#include<stdlib.h>
#include <inttypes.h>
#include "displayenhet.h"

volatile int display_tester_loop=0;
int waiting_time = 20;


/* ---------------------------------------------
 * void DISPLAY_SETUP()
 *
 *	Sätter pinnar för output på processorn, 
 *	Datalängd = 8, En rad på displayen
 * 	Display på, cursor på, cursor blinkar
 * 	Rensar displayen
 * 	Flyttriktning framåt
 * ---------------------------------------------*/
void display_setup(){

	//Pinnar output
	DDRD = 0xFB; 
	DDRB |= (1<<DDB0)|(1<<DDB1)|(1<<DDB2);

	//Datalength, number of lines
	PORTD = (1<<PORTD5)|(1<<PORTD4)|(1<<PORTD3); 

	PORTB |= (1<<PORTB1); 
	waiting(waiting_time);
	PORTB &= 0xFD; 
	waiting(waiting_time); 

	//Display, cursor på
	PORTD = (1<<PORTD3)|(1<<PORTD1)|(1<<PORTD0); 
	PORTB |= (1<<PORTB2);

	PORTB |= (1<<PORTB1);
	waiting(waiting_time);
	PORTB &= 0xFD; 
	waiting(waiting_time);

	//Clear display
	PORTD = (1<<PORTD0);
	PORTB &= 0xFB;	

	PORTB |= (1<<PORTB1); 
	waiting(waiting_time);
	PORTB &= 0xFD; 
	waiting(waiting_time); 

	//Cursor moving direction
	PORTB |= (1<<PORTB2);
	PORTD = (1<<PORTD1);
	  
	PORTB |= (1<<PORTB1); 
	waiting(waiting_time);
	PORTB &= 0xFD; 
	waiting(waiting_time+1000); 
    
    //Skriv 'rubriker'
	char_to_display('V',0x00);
	char_to_display('H',0x0C);
	char_to_display('V',0x00);
	char_to_display('F',0x06);
	char_to_display('H',0x4C);
	char_to_display('V',0x40);
	char_to_display('K',0x46);
}


/* ---------------------------------------------
 *	void WAITING(int n)
 *
 *	Gör ingenting i n klockcykler
 * ---------------------------------------------*/
void waiting(int n){
	
	volatile int a = 0;
	while(a<n){a++;}
}

/* ---------------------------------------------
 *	void DATA_TO_DISPLAY(unsigned char data)
 *
 *	Skickar en databyte till displayen, och säger åt 
 *  displayen att visa den
 *
 *	sensor_source: 0x00: höger fram
 *					0x01: vänster fram
 *					0x02: höger bak
 *					0x03: vänster bak
 *					0x04: framsensorn
 *					0x05: specialkommando
 * ---------------------------------------------*/
void data_to_display(int cm_value,unsigned char sensor_source){

	PORTB |= (1<<PORTB0); 
	PORTB &= 0xFD; 

	//Isolera sifforna i int
	int first_digit;
	int second_digit;
	int third_digit;

	for(int i=0;i<3;i++){
		first_digit = cm_value/100;
		second_digit = (cm_value-(first_digit*100))/10;
	}
	third_digit = cm_value-(second_digit*10)-(first_digit*100);

	//Konvertera till ASCII
	unsigned char data[3] = {'0','0','0'};

	if(first_digit!=0) data[0] = int_to_ascii(first_digit);
	if(first_digit!=0 || second_digit!=0) data[1] = int_to_ascii(second_digit);
	data[2] = int_to_ascii(third_digit);
	
	//Sätt rätt skrivplats
	unsigned char place;
	if(sensor_source == 0x00) place = 0x0D;
	else if(sensor_source == 0x01) place = 0x01;
	else if(sensor_source == 0x02) place = 0x4D;
	else if(sensor_source == 0x03) place = 0x41;
	else if(sensor_source == 0x04) place = 0x07;
	else if(sensor_source == 0x05) place = 0x47;
	else  place = 0x00;

    //Redo att skriva
	PORTB &= 0xFE; 
	PORTB &= 0xFD; 
	PORTD &= 0x04; 
	PORTB &= 0xFB;

	//Sätt position i minnet
	PORTD |= (1<<PORTD7);
	PORTD |= (place & 0xFB);
	PORTB |= (place & 0x04);
	  
	PORTB |= (1<<PORTB1); 
	waiting(waiting_time);
	PORTB &= 0xFD; 
	waiting(waiting_time);

	//Sänd till display
	PORTB |= (1<<PORTB0); 
	for (int i = 0;i<3; i++){
		PORTD &= 0x04; 
		PORTB &= 0xFB;
		PORTD |= (data[i] & 0xFB);
		PORTB |= (data[i] & 0x04);

		PORTB |= (1<<PORTB1); 
		waiting(waiting_time);
		PORTB &= 0xFD; 
		waiting(waiting_time);
	}
}


/* ---------------------------------------------
 *	void CHAR_TO_DISPLAY(unsigned char data, 
 *                       unsigned char place)
 *
 *	Skriver ut ett tecken på en given plats
 *
 *	data: tecken i ascii-kod
 *	place: plats i minnet i hex
 * ---------------------------------------------*/
void char_to_display(unsigned char data,unsigned char place){

	PORTB &= 0xFE; 
	PORTB &= 0xFD; 

	//Nollställ register
	PORTD &= 0x04; 
	PORTB &= 0xFB;

	//Sätt position i minnet
	PORTD |= (1<<PORTD7);
	PORTD |= (place & 0xFB);
	PORTB |= (place & 0x04);
	  
	PORTB |= (1<<PORTB1); 
	waiting(waiting_time+80); 
	PORTB &= 0xFD; 
	waiting(waiting_time+80); 


    //Redo att skriva
	PORTB = (1<<PORTB0); 
	PORTD &= 0x04; 
	PORTB &= 0xFB;

	//Sätt databitar
	PORTD |= (data & 0xFB);
	PORTB |= (data & 0x04);

	PORTB |= (1<<PORTB1);
	waiting(waiting_time);
	PORTB &= 0xFD; 
	waiting(waiting_time); 

}


/* ---------------------------------------------
 * 	unsigned char INT_TO_CHAR(int cm_value)
 *
 * 	Omvandlar int till char
 * ---------------------------------------------*/
unsigned char int_to_ascii(int digit){
	
	int digit_ascii = 48+digit;
	unsigned char data = (unsigned char)digit_ascii;
	return data;
}

