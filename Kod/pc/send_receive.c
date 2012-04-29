/*
 * Av: Johan Jönsson
 * Datum: 27/3 2012
 * Klockan: 16:03
 */
#define DEBUG
#define NO_BLUE

//#undef NO_BLUE

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

FILE *f;	/* Härvid är jag af nöden tvungen!
		   Annars kan jag inte stänga filen när programmet avslutas
		*/

int new_data(FILE *db, struct instruction_t *inst)
{

	struct instruction_t *tmp = malloc(sizeof(struct instruction_t));
	read_from_db(db, tmp, 2);
	if((tmp->header == inst->header) && (tmp->data == inst->data)){ 
		return 0;
	} else {
		inst->header = tmp->header;
		inst->data = tmp->data;
		return 1;
	}
	free(tmp);
}

void send_inst(int s, struct instruction_t *inst)
{
	send_msg(s, inst->header);
	send_msg(s, inst->data);
	return;
}

void receive_inst(int s, struct sockaddr_rc ff, struct instruction_t *inst)
{

	receive_data(s, ff, &inst->header);
	receive_data(s, ff, &inst->data);

	return;
}
void exit_program()
{
	exit_curses();
	fclose(f);
	exit(0);

}
int main(void)
{
	int socket = init();
	signal(SIGINT, exit_program);
	

#ifndef NO_BLUE
	struct sockaddr_rc firefly = connect_to_firefly(socket);
#endif /* NO_BLUE */
	init_curses();

	int quit = 0;
	f = init_read("instr_db");
	struct instruction_t *inst = malloc(sizeof(struct instruction_t));
	struct instruction_t *ex_inst = malloc(sizeof(struct instruction_t));
	inst->header = 'a';
	inst->data = 'b';
#ifdef DEBUG
	ex_inst->header = (unsigned char) 0x00;
	ex_inst->data = (unsigned char) 0xC0;
#endif /* DEBUG */
	while(!quit){
		if(new_data(f, inst)){
			update_speed(inst);
#ifndef NO_BLUE
			send_inst(socket, inst);
#endif /* NO_BLUE */
		}else{
#ifndef DEBUG
#ifndef NO_BLUE
			receive_inst(socket, firefly, ex_inst);
#endif /* NO_BLUE */
#endif /* DEBUG */
			display_inst(ex_inst);
		}
	}

	return 0;
}
