/*
 * Av: Johan Jönsson
 * Datum: 27/3 2012
 * Klockan: 16:03
 */

#include "blue_pc.h"
#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>


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

int main(void)
{
	int socket = init();
	//printf("Socket #%d opened\n", socket);

	struct sockaddr_rc firefly = connect_to_firefly(socket);
	int quit = 0;
	FILE *f = init_read("instr_db");
	struct instruction_t *inst = malloc(sizeof(struct instruction_t));
	struct instruction_t *ex_inst = malloc(sizeof(struct instruction_t));
	inst->header = 'a';
	inst->data = 'b';
	int i = 0;
	while(!quit){
		if(new_data(f, inst)){
			//printf("%d: Ny data\n", i);
			send_inst(socket, inst);
		}else{
			// printf("%d: Gammal data\n", i);
			// receive_inst(socket, firefly, ex_inst);
		}
		i++;
	}

	fclose(f);
	return 0;
}
