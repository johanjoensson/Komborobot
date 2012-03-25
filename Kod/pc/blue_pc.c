/*
 * Datum 22/03/2012
 * Gränssnitt för att skicka information över en blåtandsanslutning till en
 * given blåtandsenhet (firefly)
 * 
 * Skapad av: Johan Jönsson
 *
 * Redigerad:
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
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
//	char dest[18] = "38:E7:D8:C0:8F:E2";	// MAC-addess till min HTC
	char dest[18] = "00:06:66:03:A9:D1";	// MAC-address till
//	FireFlyenheten
//	char dest[18] = "00:15:83:2A:48:18";
	
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
	printf("Sending message: %X to Firfly unit\n", msg);
	if(chk_msg == -1){
		fprintf(stderr, "Error, message not sent!\n");
	}else{
		printf("Sent %d byte to Firefly unit\n", chk_msg);
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
		printf("Start listening to the Firefly unit\n");
		listen(s,1);
}

/* Tag emot en databyte skickad till socket s från blåtandsenhete ff
 *
 * Inparametrar:	int s		socket data skickas till
 *			sockaddr_rc ff	blåtandsenhet som skickar datan
 *			char *buf	buffer för mottagande av data
 *
 * Returvärde:		uint8_t		databyten som mottogs
 */
uint8_t receive_data(int s, struct sockaddr_rc ff, char* buffer)
{
	ba2str(&ff.rc_bdaddr, buffer);

	memset(buffer, 0, sizeof(buffer));

	printf("Receiving data from the Firefly unit\n");

	int msg = recv(s, buffer, 1, 0);
	if(msg == -1){
		fprintf(stderr, "Error, no message received!\n");
	}
	return msg;
}

/* Huvudprogrammet.
 * Sköter anrop till övriga funktioner och initierar variabler.
 * Skapar anslutning till en firefly-enhet, skickar en byte och tar emot en byte
 *
 * Inparametrar:	int argc	antalet argument programmet anropas med
 *			char** argv	lista över argumenten
 */
int main(int argc, char** argv)
{

	/*
	int dev_id, sock;

	printf("Finding local Bluetooth adapter\n");
	dev_id = hci_get_route(NULL);

	printf("Opening BLuetooth socket\n");
	sock = hci_open_dev( dev_id );
	if (dev_id < 0 || sock < 0) {
		perror("opening socket");
		exit(1);
	}

	printf("Socket open, local adapter OK!\n");
	*/
	uint8_t msg; 
	if(argc > 1){
		msg = atoi(argv[1]);
	}else{
	       msg = 0xA2;
	}

	char *buf = malloc(sizeof(char));
	uint8_t data;
	
	int s = init();

	struct sockaddr_rc ffunit = connect_to_firefly(s);

	send_msg(s,msg);

	start_listening(s);

	data = receive_data(s, ffunit, buf);
		
	printf("Data received %X\n", data);

	close_socket(s);
	return 0;
}
