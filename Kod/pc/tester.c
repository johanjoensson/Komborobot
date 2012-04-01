/*
 * Fil för att testa blåtandskommunikation samt busskommunikation
 *
 *
 */


#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>

#include"blue_pc.h"



unsigned int get_input_mode()
{

	int choise = 0;
	while(!((choise == 1)|(choise == 2)|(choise == 3))){
		printf("Select input mode:\n\
				1: decimal\n\
				2: hexadecimal (stora bokstäver)\n\
				3: octal\n");
		scanf("%d", &choise);
	}

	printf("Thank you for choosing an input method\n");
	return choise;
}

uint8_t get_byte(int mode)
{
	unsigned int data;
	printf("Please input your byte\n");
	switch(mode){
		case 1:
			scanf("%u", &data);
			break;
		case 2:
			scanf("%X", &data);
			break;
		case 3:
			scanf("%o", &data);
			break;
		default:
			fprintf(stderr, "Unknown input format %d selected, aborting\n", mode);
			exit(1);
	}

	return (uint8_t) data;
}

uint8_t get_header()
{

	unsigned int mode = get_input_mode();
	printf("Enter header byte\n");

	return get_byte(mode);
}

uint8_t get_data()
{

	unsigned int mode = get_input_mode();
	printf("Enter data byte\n");
	
	return get_byte(mode);
}


int main(int argc, char** argv)
{
	int mode = 0;

	if(argc > 1){
		if(*argv[1] == 'l'){
			mode = 1;
		}else if(*argv[1] == 't'){
			mode ==2;
		}else{
			fprintf(stderr, "Uknown argument %s\n", argv[1]);
			return 1;
		}

	}
	struct instruction_t *inst = malloc(sizeof(instruction));
	int socket = init();
	
	struct sockaddr_rc ffunit = connect_to_firefly(socket);
	int quit = 0;
	char cont;

	switch(mode){
		case 0:
			while(!quit){
				printf("Prepare to enter header byte!\n");
				inst->header = get_header();
				printf("Prepare to enter data byte\n");
				inst->data = get_data();
				send_msg(socket, inst->header);
				send_msg(socket, inst->data);
				printf("Continue sending data? (y/n)\n");
				scanf("%c", &cont);
				if(cont == 'n'){
					quit =1;
				}
			}
			break;
		case 1:
			while(!quit){
				receive_data(socket, ffunit, &(inst->header));
				receive_data(socket, ffunit, &(inst->data));
				printf("Data receiveived:\n\
						header:\t%X\n\
						data:\t%X\n", (uint8_t) inst->header, (uint8_t) inst->data);
			}
			break;
		case 2:
			while(!quit){
				inst->header = 0x40;
				printf("Prepare to enter data byte\n");
				inst->data = get_byte(2);
				send_msg(socket, inst->header);
				send_msg(socket, inst->data);
				printf("Continue sending data? (y/n)\n");
				scanf("%c", &cont);
				if(cont == 'n'){
					quit =1;
				}
			}
			break;

		default:
			fprintf(stderr,"Uknown operating mode!\n");
			return 1;
	}

	return 0;

}
