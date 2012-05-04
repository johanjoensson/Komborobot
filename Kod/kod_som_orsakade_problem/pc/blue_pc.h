#ifndef BLUE_PC_H
#define BLUE_PC_H

#include <sys/socket.h>
#include <stdint.h>

struct instruction_t {
	unsigned char header;
	unsigned char data;
} instruction; 

int init();

struct sockaddr_rc connect_to_firefly(int s);

void send_msg(int s, uint8_t msg);

void close_socket(int s);

void start_listening(int s);

void receive_data(int s, struct sockaddr_rc ff, void* buffer);



#endif
