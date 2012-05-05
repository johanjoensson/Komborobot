#ifndef DISPLAYENHET_H
#define DISPLAYENHET_H

void display_setup(); 
void waiting(int n);
void data_to_display(int cm_value,unsigned char sensor_source);
void char_to_display(unsigned char data,unsigned char place);
unsigned char int_to_ascii(int digit);

#endif
