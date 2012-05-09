/*
 * Av: Johan Jönsson
 * Datum: 27/3 2012
 * Klockan: 16:03
 */
#define DEBUG
#define NO_BLUE

#undef NO_BLUE
#undef DEBUG

#include "blue_pc.h"
#include "db.h"
#include "display.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>

	
/******************************************************************************
 * Pekare till vår databas
 *****************************************************************************/
FILE *f;	

/******************************************************************************
 * Socket vi binder till blåtandsenheten
 *****************************************************************************/
int sock;

/******************************************************************************
 * Returnera 1 ifall ny data matats in i databasen
 * returnera 0 annars
 *****************************************************************************/
int new_data(FILE *db, struct instruction_t *inst)
{

	struct instruction_t tmp;
	read_from_db(db, &tmp, 2);
	if((tmp.header == inst->header) && (tmp.data == inst->data)){ 
		return 0;
	} else {
		inst->header = tmp.header;
		inst->data = tmp.data;
		return 1;
	}
}

/******************************************************************************
 * Skicka instruktion till Komborobot
 *****************************************************************************/
void send_inst(int s, struct instruction_t *inst)
{
	send_msg(s, inst->header);
	send_msg(s, inst->data);
	return;
}

/******************************************************************************
 * Ta emot instruktioner från Komborobot
 *****************************************************************************/
int receive_inst(int s, struct sockaddr_rc ff, struct instruction_t *inst)
{

	receive_data(s, ff, &inst->header);
	receive_data(s, ff, &inst->data);

        if(!( ( ( (int) inst->header & 0xC0) >> 5 == 4 )||( ( (int)inst->header & 0xC0) >> 5 == 6) ) ){
                return 1;
        }

	return 0;
}

/******************************************************************************
 * Avsluta programmet på korrekt sätt
 * Kör då ctr + c matas in
 *****************************************************************************/
void exit_program()
{
	exit_curses();
	fclose(f);
        close_socket(sock);
	exit(0);

}

/******************************************************************************
 * Initiera allt, och kommunicera med Komborobot
 *****************************************************************************/
int main(void)
{
        sock = init();
        /**********************************************************************
         * Bind ctr + c till exit_program
         *********************************************************************/
	signal(SIGINT, exit_program);

        struct instruction_t inst, ex_inst;

#ifndef NO_BLUE
	struct sockaddr_rc firefly = connect_to_firefly(sock);
#endif /* NO_BLUE */
	init_curses();
	int quit = 0;
	f = init_read("instr_db");



	inst.header = 'a';
	inst.data = 'b';
#ifdef DEBUG
	ex_inst.header = (unsigned char) 0x02;		// Speckomm 
	ex_inst.data = (unsigned char) 0x70;		// vänster fram, 15 cm
#endif /* DEBUG */
        /**********************************************************************
         * Huvudloopen, skicka, ta emot instruktioner samt uppdatera skärmen
         *********************************************************************/
	while(!quit){
		if(new_data(f, &inst)){
			update_speed(&inst);
#ifndef NO_BLUE
			send_inst(sock, &inst);
#endif /* NO_BLUE */
		}else{
#ifndef NO_BLUE
                        if( receive_inst(sock, firefly, &ex_inst) == 0){
#endif /* NO_BLUE */
                                display_inst(&ex_inst);
#ifndef NO_BLUE
                        }
#endif /* NO_BLUE */
		}
	}

	return 0;
}
