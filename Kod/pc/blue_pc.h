/******************************************************************************
 * Skapad: 22/03/2012
 * Gränssnitt för att skicka information över en bkåtandsanslutning till en
 * given blåtandsenhet
 *
 * Skapad av: Johan Jönsson
 *
 * Redigerad: 08/05/2012
 *****************************************************************************/

#ifndef BLUE_PC_H
#define BLUE_PC_H

#include <sys/socket.h>
#include <stdint.h>

/******************************************************************************
 * Enkel beskrivning av hur en instruktion ser ut, med header-byte och data-byte
 ******************************************************************************/
struct instruction_t {
	unsigned char header;
	unsigned char data;
} instruction; 

/******************************************************************************
 * Initierar blåtandsenheten, kopplar en socket (int) till blåtandsenheten.
 * Inparametrar - 
 * Returvärde - socket allokerad till blåtandsenheten (int)
 *****************************************************************************/
int init();

/******************************************************************************
 * Skapar anslutning till en fireflyenhet via socket s
 * Inparametrar - socket s
 * Returvärde - sockaddr_rc blåtandsenhet vi anslutit till
 *****************************************************************************/
struct sockaddr_rc connect_to_firefly(int s);

/******************************************************************************
 * Skickar en byte (msg) till socket (s) för vidare kommunikation
 * Inparametrar - int s Socket för blåtandskommunikation
 *              - uint8_t msg Byten som skickas via blåtand
 * Returvärde - 
 *****************************************************************************/
void send_msg(int s, uint8_t msg);

/******************************************************************************
 * Stänger socket s så att vidare kommunikation är omöjlig
 * Inparametrar - int s socket som skall slängas
 * Returvärde - 
 *****************************************************************************/
void close_socket(int s);

/******************************************************************************
 * Börja accepterar anslutningar till socket s
 * Inparametrar - socket s socket att börja lyssna på
 * Returvärde -
 *****************************************************************************/
void start_listening(int s);

/******************************************************************************
 * Tag emot en byte skickad till socket s från blåtandsenhet ff
 * Inparametrar - int s Socket data skickas till
 *              - sockaddr_rc ff blåtandsenhet som skickar byten
 *              - void *buf buffer för mottagande av data
 * Returvärde    -
 *****************************************************************************/
void receive_data(int s, struct sockaddr_rc ff, void* buffer);



#endif
