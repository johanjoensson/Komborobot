#ifndef SENSOR_SPI_H
#define SENSOR_SPI_H

/* int req_sending()
 *
 * Används då data ska förmedlas till andra enheter på roboten. Sätter in header
 * i SPDR och sätter requestsignalen till kommunikationsenheten hög. 
 *
 *Input: tom
 *Output: 0
 *
 */
int req_sending();

/* Globala variabler för att spara header och data som ska skickas
 * eller som nyligen tagits emot.
 */

unsigned char header;
unsigned char data;

//Global variabel som bestämmer vilket läge sensorenheten ska operera i, 
//fjärrstyrt eller autonomt läge.
int auto_mode;

/*void decide_mode()
 *
 *Ändrar läget från fjärrstyrt till autonomt och viceverca
 *Input: Inget
 *Output: Void
 */

void decide_mode();

#endif
