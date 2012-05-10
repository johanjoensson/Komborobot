/* ---------------------------------------------
 * Datum 2012-05-01
 * Skapad av Mattias Jansson
 *
 * Hanterar displayenheten
 * ---------------------------------------------*/

#ifndef DISPLAYENHET_H
#define DISPLAYENHET_H

/* ---------------------------------------------
 *	Sätter pinnar för output på processorn, 
 *	Datalängd = 8, En rad på displayen
 * 	Display på, cursor på, cursor blinkar
 * 	Rensar displayen
 * 	Flyttriktning framåt
 *
 * INPAR: -
 * RET: -
 * ---------------------------------------------*/
void display_setup(); 



/* ---------------------------------------------
 * Gör ingenting i n klockcykler
 *
 * INPAR: -
 * RET: -
 * ---------------------------------------------*/
void waiting(int n);



/* ---------------------------------------------
 *	Skickar en databyte till displayen, och säger åt 
 *  displayen att visa den
 *
 * INPAR:   sensor_source: 0x00: höger fram
 *					0x01: vänster fram
 *					0x02: höger bak
 *					0x03: vänster bak
 *					0x04: framsensorn
 *					0x05: specialkommando
 * RET: -
 * ---------------------------------------------*/
void data_to_display(int cm_value,unsigned char sensor_source);



/* ---------------------------------------------
 *	Skriver ut ett tecken på en given plats
 *
 * INPAR:   data: tecken i ascii-kod
 *          place: plats i minnet i hex
 * RET: -
 * ---------------------------------------------*/
void char_to_display(unsigned char data,unsigned char place);


/* ---------------------------------------------
 * Omvandlar int till char kodad i ASCII
 *
 * INPAR:   digit: int som ska omvandlas
 * RET:     ASCII-koden för int
 * ---------------------------------------------*/
unsigned char int_to_ascii(int digit);

#endif
