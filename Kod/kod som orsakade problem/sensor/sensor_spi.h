#ifndef SENSOR_SPI_H
#define SENSOR_SPI_H

int req_sending();
unsigned char header;
unsigned char data;
int auto_mode;
void decide_mode();

#endif
