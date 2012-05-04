#include <avr/io.h>
#include<stdio.h>
#include<stdlib.h>
#include <inttypes.h>
#include "displayenhet.h"

volatile int display_tester_loop=0;
int waiting_time = 20;

/*
PORTD 0-1: Databitar
PORTB 2: Databit
PORTD 3-7: Databitar
PORTB 0: Register select
PORTB 1: Enable
*/

/*
void display_setup(); 
void waiting(int n);
void data_to_display(int cm_value,unsigned char sensor_source);
void char_to_display(unsigned char data,unsigned char place);
unsigned char int_to_ascii(int digit);
*/

/*
*	void SETUP_DISPLAY()
*	Initierar displayen. Kallas vid uppstart av roboten
*
*	Sätter pinnar för output på processorn, 
*	Datalängd = 8, En rad på displayen
* 	Display på, cursor på, cursor blinkar
* 	Rensar displayen
* 	Flyttriktning framåt
*/
void display_setup(){

	//Pinnar output
	DDRD = 0xFB; 
	DDRB |= (1<<DDB0)|(1<<DDB1)|(1<<DDB2);

	//Datalength, number of lines
	PORTD = (1<<PORTD5)|(1<<PORTD4)|(1<<PORTD3); 

	PORTB |= (1<<PORTB1); //Enable
	waiting(waiting_time); //t_SU2
	PORTB &= 0xFD; //Disable
	waiting(waiting_time); //t_SU2

	//Display, cursor på
	PORTD = (1<<PORTD3)|(1<<PORTD1)|(1<<PORTD0); 
	PORTB |= (1<<PORTB2);

	PORTB |= (1<<PORTB1); //Enable
	waiting(waiting_time); //t_SU2
	PORTB &= 0xFD; //Disable
	waiting(waiting_time); //t_SU2

	//Clear display
	PORTD = (1<<PORTD0);
	PORTB &= 0xFB;	

	PORTB |= (1<<PORTB1); //Enable
	waiting(waiting_time); //t_SU2
	PORTB &= 0xFD; //Disable
	waiting(waiting_time); //t_SU2

	//Cursor moving direction
	PORTB |= (1<<PORTB2);
	PORTD = (1<<PORTD1);
	  
	PORTB |= (1<<PORTB1); //Enable
	waiting(waiting_time); //t_SU2
	PORTB &= 0xFD; //Disable
	waiting(waiting_time+1000); //t_SU2

	char_to_display('V',0x00);
	char_to_display(':',0x01);

	char_to_display('H',0x08);
	char_to_display(':',0x09);
}

/*
*	void WAITING(int n)
*	Gör ingenting i n klockcykler
*/
void waiting(int n){
	
	volatile int a = 0;
	while(a<n){a++;}
}

/*
*	void DATA_TO_DISPLAY(unsigned char data)
*	Skickar en databyte till displayen, och säger åt displayen att visa den
*	sensour_source: 0x00: höger
*					0x01: vänster
*					0x02: fram
*/
void data_to_display(int cm_value,unsigned char sensor_source){

	PORTB |= (1<<PORTB0); //Regsiter select = data
	PORTB &= 0xFD; //Disable 

	//Isolera sifforna i int
	int first_digit;
	int second_digit;
	int third_digit;

	for(int i=0;i<3;i++){
		first_digit = cm_value/100;
		second_digit = (cm_value-(first_digit*100))/10;
	}
	third_digit = cm_value-(second_digit*10)-(first_digit*100);

	//Convert digits to ASCII-code
	unsigned char data[3] = {'0','0','0'};

	if(first_digit!=0) data[0] = int_to_ascii(first_digit);
	if(first_digit!=0 || second_digit!=0) data[1] = int_to_ascii(second_digit);
	data[2] = int_to_ascii(third_digit);
	
	//Sätt rätt skrivplats
	unsigned char place;
	if(sensor_source == 0x00) place = 0x0A;
	else if(sensor_source == 0x01) place = 0x02;
	else  place = 0x00;

	PORTB &= 0xFE; //Register select = function
	PORTB &= 0xFD; //Disable 

	//Nollställ register
	PORTD &= 0x04; 
	PORTB &= 0xFB;

	//Sätt position i minnet
	PORTD |= (1<<PORTD7);
	PORTD |= (place & 0x0B);
	PORTB |= (place & 0x04);
	  
	PORTB |= (1<<PORTB1); //Enable
	waiting(waiting_time); //t_SU2
	PORTB &= 0xFD; //Disable
	waiting(waiting_time); //t_SU2

	//Sänd till display
	PORTB = (1<<PORTB0); //Regsiter select = data
	for (int i = 0;i<3; i++){
		//Nollställ databitar
		PORTD &= 0x04; 
		PORTB &= 0xFB;

		//Sätt databitar
		PORTD |= (data[i] & 0xFB);
		PORTB |= (data[i] & 0x04);

		PORTB |= (1<<PORTB1); //Enable
		waiting(waiting_time); //t_SU2
		PORTB &= 0xFD; //Disable
		waiting(waiting_time); //t_SU2
	}
}

/*
*	void CHAR_TO_DISPLAY(unsigned char data, unsigned char place)
*	Skriver ut ett tecken på en given plats
*
*	data: tecken i ascii-kod
*	place: plats i minnet i hex
*/
void char_to_display(unsigned char data,unsigned char place){

	PORTB &= 0xFE; //Register select = function
	PORTB &= 0xFD; //Disable 

	//Nollställ register
	PORTD &= 0x04; 
	PORTB &= 0xFB;

	//Sätt position i minnet
	PORTD |= (1<<PORTD7);
	PORTD |= (place & 0x0B);
	PORTB |= (place & 0x04);
	  
	PORTB |= (1<<PORTB1); //Enable
	waiting(waiting_time+80); //t_SU2
	PORTB &= 0xFD; //Disable
	waiting(waiting_time+80); //t_SU2


	PORTB = (1<<PORTB0); //Regsiter select = data

	//Nollställ databitar
	PORTD &= 0x04; 
	PORTB &= 0xFB;

	//Sätt databitar
	PORTD |= (data & 0xFB);
	PORTB |= (data & 0x04);

	PORTB |= (1<<PORTB1); //Enable
	waiting(waiting_time); //t_SU2
	PORTB &= 0xFD; //Disable
	waiting(waiting_time); //t_SU2

}



/*
* 	unsigned char INT_TO_CHAR(int cm_value)
* 	Omvandlar int till char
*/
unsigned char int_to_ascii(int digit){
	
	int digit_ascii = 48+digit;
	unsigned char data = (unsigned char)digit_ascii;
	return data;
}

/*
int main(void){

	display_setup();


	char_to_display('V',0x00);
	char_to_display(':',0x01);

	char_to_display('H',0x08);
	char_to_display(':',0x09);


	data_to_display(406,0x00);

	
	data_to_display(45,0x00);
	data_to_display(56,0x01);
	data_to_display(906,0x00);
	data_to_display(4,0x00);


	//Oändlig dummy-loop
	while (display_tester_loop<255) {
		display_tester_loop++;
		if (display_tester_loop>254) {
			display_tester_loop = 1;
		}
	}
	return 0;
}

*/
