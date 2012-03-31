#ifndef BLUE_PC_H
#define BLUE_PC_H

#include <sys/socket.h>
int init();

struct sockaddr_rc connect_to_firefly(int s);

void send_msg(int s, uint8_t msg);

void close_socket(int s);

void start_listening(int s);

uint8_t receive_data(int s, struct sockaddr_rc ff, char* buffer);



#endif
