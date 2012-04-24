#ifndef SPECIAL_H
#define SPECIAL_H

void set_next_special_command(unsigned char command);
unsigned char get_next_special_command();

void generate_special_command(unsigned char command_code);
void send_special_command(unsigned char command);
int search_for_crossroad();

#endif
