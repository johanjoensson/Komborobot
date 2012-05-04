#ifndef DISPLAY_H
#define DISPLAY_H

#include "blue_pc.h"

enum mode{
	LABYRINT,
	LINJE,
	ERROR
};

enum data_type_t{
	SENSORDATA,
	REGLERING,
	SPECKOMMANDO,
	UNKNOWN_DATA
} data_type;

enum spec_komm_t{
	REGLER,
	FRAM,
	VANSTER,
	HOGER,
	UNKNOWN_COMMAND
};

void init_curses();

void exit_curses();

void update_speed(struct instruction_t *inst);
void display_inst(struct instruction_t *inst); 


#endif // DISPLAY_H
