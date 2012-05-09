#ifndef DISPLAY_H
#define DISPLAY_H

#include "blue_pc.h"

/******************************************************************************
 * Lägen som Komborobot kan köra i
 *****************************************************************************/
enum mode{
	LABYRINT,
	LINJE,
	FJARR,
	ERROR
};

/*****************************************************************************
 * Olika typer av data som Komborobot kan skicka
 *****************************************************************************/
enum data_type_t{
	SENSORDATA,
	REGLERING,
	SPECKOMMANDO,
	UNKNOWN_DATA
} data_type;

/******************************************************************************
 * Specialkommandon som Komborobot kan skicka
 *****************************************************************************/
enum spec_komm_t{
	REGLER,
	FRAM,
	VANSTER,
	HOGER,
        FEL,
	UNKNOWN_COMMAND
};

/******************************************************************************
 * Initiera det pseudogrfiska gränssnittet
 * Inparametrar - 
 * Returvärde   - 
 *****************************************************************************/
void init_curses();

/******************************************************************************
 * Avsluta det pseudografiska gränssnittet och frigör allt minne som allokerats
 * under initieringen
 * Inprametrar  - 
 * Returvärde   -
 *****************************************************************************/
void exit_curses();

/******************************************************************************
 * Uppdatera gränssnittet med aktuell information om önskad hastiget på roboten
 * Inparametrar - struct instruction_t *inst Instruktionen som skickas till
 *              Komborobot
 * Returvärde   - 
 *****************************************************************************/
void update_speed(struct instruction_t *inst);

/******************************************************************************
 * Uppdatera gränssnittet med aktuell information om vad roboten skickar till
 * datorn
 * Inparametrar - struct instruction_t *inst Informationen roboten skickat till
 * datorn
 * Returvärde   - 
 *****************************************************************************/
void display_inst(struct instruction_t *inst); 


#endif // DISPLAY_H
