/******************************************************************************
 * Datum 22/03/2012
 * Gränssnitt för att skicka information över en blåtandsanslutning till en
 * given blåtandsenhet (firefly)
 * 
 * Skapad av: Johan Jönsson
 *
 * Redigerad: 08/05/2012
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>

#include"blue_pc.h"

/* Initiering av blåtandsenheten
 * Kopplar en socket (int) till blåtandsenheten
 *
 * Tar inga inparametrar
 * Returnerar värdet på den allokerade socketen
 */
int init()
{
	printf("Opening Bluetooth socket\n");
	return socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
}


/* Skapar anslutning till en fireflyenhet via socket s
 *
 * Inparametrar: int s	Socket som används för blåtand
 *
 * Returvärde:	sockaddr_rc Den avlägsna blåtandsenhet man anslutit till
 */
struct sockaddr_rc connect_to_firefly(int s)
{ 
	struct sockaddr_rc addr, loc_addr;
	int status;
	char dest[18] = "00:06:66:03:A9:D1";	// MAC-address fireflyenheten
	
	loc_addr.rc_family = AF_BLUETOOTH;
	loc_addr.rc_bdaddr = *BDADDR_ANY;
	loc_addr.rc_channel = (uint8_t) 1;

	bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

	addr.rc_family = AF_BLUETOOTH;
	addr.rc_channel = (uint8_t) 1;
	str2ba( dest, &addr.rc_bdaddr );
	
	printf("Connecting to Firefly unit: %s\n", dest);
	status = connect( s, (struct sockaddr *) &addr, sizeof(addr));

	if(status != 0){
		perror("Oh crap!");
		exit(1);
	}

	return addr;
}


/* Skickar en byte data (msg) till socket (s) för vidare kommunikation
 *
 * Inparametrar:	int s		socket för kommunikation
 *			uint8_t msg	Byten vi skickar
 *
 * Returvärde:		-
 */
void send_msg(int s, uint8_t msg)
{
	int chk_msg = send(s, &msg, 1, 0);
	if(chk_msg == -1){
		fprintf(stderr, "Error, message not sent!\n");
	}
}

/*Stänger socket s, så att vidare kommunikation är omöjlig
 *
 * Inparametrar:	int s	socket som skall stängas
 *
 * Returvärde:		-
 */
void close_socket(int s)
{
	close(s);
}

/* Börja acceptera anslutningar på socket s
 * För att möjliggöra tvåvägskommunikation, accepterar enbart en inkommande
 * anslutning
 *
 * Inarametrar:		int s	socket att lyssna på
 *
 * Returvärde:		-
 */
void start_listening(int s)
{
		listen(s,1);
}

/* Tag emot en databyte skickad till socket s från blåtandsenhete ff
 *
 * Inparametrar:	int s		socket data skickas till
 *			sockaddr_rc ff	blåtandsenhet som skickar datan
 *			void *buf	buffer för mottagande av data
 *
 * Returvärde:		-
 */
void receive_data(int s, struct sockaddr_rc ff, void* buffer)
{
	ba2str(&ff.rc_bdaddr, buffer);

	memset(buffer, 0, sizeof(buffer));


	int nr = recv(s, buffer, 1, 0);

	if(nr == -1){
		fprintf(stderr, "Error, no message received!\n");
	}
	return;
}
