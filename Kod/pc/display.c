#include <stdio.h>
#include <ncurses.h>
#include "display.h"
#include "blue_pc.h"

int width, height;

int trim = 0;

WINDOW *mode, *spec_komm, *lsensor, *rsensor, *speed, *ltrim, *rtrim, *speed_back, *ltrim_back, *rtrim_back;

WINDOW *create_win(int win_height, int win_width, int starty, int startx, char corner, char hline, char vline)
{
	WINDOW *local_win;

	local_win = newwin(win_height, win_width, startx, starty);
	wborder(local_win, vline, vline, hline, hline, corner, corner, corner, corner);

	wrefresh(local_win);

	return local_win;
}

void remove_win(int win)
{
	WINDOW *local_win;
	
	switch(win){
		case 0:
			local_win = mode;
			break;
		case 1:
			local_win = spec_komm;
			break;
		case 2:
			local_win = lsensor;
			break;
		case 3:
			local_win = rsensor;
			break;
		case 4:
			local_win = speed;
			break;
		case 5:
			local_win = ltrim;
			break;
		case 6:
			local_win = rtrim;
			break;
		default:
			local_win = NULL;
	}

	if(local_win != NULL){
		wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
		wrefresh(local_win);
	}
	return;
}

void init_curses()
{
	initscr();
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);

	noecho();
	getmaxyx(stdscr, height, width);

	mode = create_win(height/6, width, 0, 0, '#', '=', '/');
	spec_komm = create_win(height/6, width/3, width/3, height/6, '#', '=', '/');
	lsensor = create_win(height >> 1, width/3, 0, height/6, '#', '=', '/');
	rsensor = create_win(height >> 1, width/3, width/3 << 1, height/6, '#', '=', '/');
	speed = create_win((height >> 1) - height/6, width/3, width/3, height/6 << 1, '#', '=', '/');
	ltrim = create_win(height/3 , width/6, width/3, height/3 << 1, '#', '=', '/');
	rtrim = create_win(height/3 , width/6, width >> 1, height/3 << 1, '#', '=', '/');
	
	speed_back = speed;
	ltrim_back = ltrim;
	rtrim_back = rtrim;
	return;

}

void exit_curses()
{
	remove_win(0);
	remove_win(1);
	remove_win(2);
	remove_win(3);
	remove_win(4);
	remove_win(5);
	remove_win(6);

	delwin(mode);
	delwin(spec_komm);
	delwin(lsensor);
	delwin(rsensor);
	delwin(speed);
	delwin(ltrim);
	delwin(rtrim);
	endwin();
}

void display_error(struct instruction_t *inst)
{
	fprintf(stderr, "Something is seriously wrong with you!\n%X;%X is supposed to be what?\n", inst->header, inst->data);
	return;
}
enum mode run_mode(struct instruction_t *inst)
{
	switch((inst->header >> 4) % 2){
		case 0:
			return LABYRINT;
		case 1:
			return LINJE;
		default:
			return ERROR;
	}
	return ERROR;
}

void display_mode(struct instruction_t *inst)
{
	int y, x;
	getmaxyx(mode, y, x);
	switch(run_mode(inst)){
		case LABYRINT:
			mvwaddstr(mode, y >> 1, (x >> 1) - x/12 ,"Kör i labyrint");
			break;
		case LINJE:
			mvwaddstr(mode, y >> 1, (x >> 1) - x/12 ,"Följer linjen");
			break;
		default:
			display_error(inst);
	}

	wrefresh(mode);
	return;
}

enum data_type_t type_of_inst(struct instruction_t *inst)
{

	switch( (inst->header >> 2) % 4 ){ // maska fram B och C flaggorna
		/* C = 0
		 * Alltså sensorinfo
		 */
		case 0:
		case 2:
			return SENSORDATA;
		/* C = 1, B = 0
		 * Alltså vanlig reglering
		 */
		case 1:
			return REGLERING;

		/* C = 1, B = 1
		 * Alltså specilkommando
		 */
		case 3:
			return SPECKOMMANDO;
		
		/* Något är väldigt fel
		 */ 
		default:
			return UNKNOWN_COMMAND;
	}
	return UNKNOWN_COMMAND;
}

enum spec_komm_t type_of_command(struct instruction_t *inst)
{
	switch((inst->data & 0xE0) >> 5){
		case 0:
			return REGLER;

		case 1:
			return FRAM;

		case 2:
			return VANSTER;

		case 3:
			return HOGER;

		default:
			return UNKNOWN_COMMAND;
	}

	return UNKNOWN_COMMAND;
}


void display_sensor(struct instruction_t *inst)
{
	int y, x;
	getmaxyx(mode, y, x);

	fprintf(stdout, "Avstånd till vägg:\t%d", inst->data);
	return;
}

void display_reg(struct instruction_t *inst)
{
	int y, x;
	getmaxyx(mode, y, x);

	return;
}



void display_spec(struct instruction_t *inst)
{
	int y, x;
	getmaxyx(spec_komm, y, x);

	switch(type_of_command(inst)){
		case REGLER:
			mvwprintw(spec_komm,  y >> 1, x >> 4,  "Tillbaka till vanlig reglering!");
			break;
		case FRAM:
			mvwprintw(spec_komm, y >> 1, x >> 2,  "Kör framåt!");
			break;
		case VANSTER:
			mvwprintw(spec_komm, y >> 1, x >> 2,  "Sväng vänster!");
			break;
		case HOGER:
			mvwprintw(spec_komm, y >> 1, x >> 2,  "Sväng höger!");
			break;

		default:
			display_error(inst);
	}

	wrefresh(spec_komm);

	return;
}

int is_trim(struct instruction_t *inst)
{
	switch((inst->data & 0xF0) >> 4){
		case 9:
		case 0xA:
		case 0xB:
			return 1;

		default:
			return 0;
	}
	return 0;
}

int has_speed(struct instruction_t *inst)
{
	switch((inst->data & 0xF0) >> 4){
		case 0x7:
		case 0x8:
		case 0x9:
		case 0xA:
		case 0xB:
			return 0;

		default:
			return 1;
	}

	return 1;
}

void display_speed(struct instruction_t *inst)
{
	int x,y;
	int c = 1;
	int cur_speed = inst->data & 0x0F;

	getmaxyx(speed, y, x);
		
	wattron(speed, COLOR_PAIR(c));

	for(int i = 1; i < x - 1 ; i++){
		mvwaddch(speed, y >> 1, i, ' ');
	}
	wrefresh(speed);

	for(int j = 0; j < cur_speed; j++){
	
		if(j < 7){
			c = 1;
		}else if(j < 11){
			c = 2;
		} else{
			c = 3;
		}

		wattron(speed,COLOR_PAIR(c));
		mvwaddch(speed, y >> 1, (x >> 4) + j, '|');
	}	

	wrefresh(speed);
	wattroff(speed, COLOR_PAIR(c));
	return;
}

void print_trim(WINDOW *win, int spd)
{
	int x,y, i;
	int c = 1;
	getmaxyx(win, y, x);

	for(i = 2; i < y; i++){
		mvwaddch(win, y-i, x >> 1, ' ');
	}
	wrefresh(win);
	for(i = 0; i < ((8 + spd) >> 1); i++){
		if(i < 4){
			c = 1;
		}else if(i < 6){
			c = 2;
		}else{
			c = 3;
		}

		wattron(win, COLOR_PAIR(c));
		mvwaddch(win, (y - 2) - i, x >> 1, '=');
	}
	if((8 + spd) % 2){
		mvwaddch(win, (y - 3) - i, x >> 1, '_');
	}

	wrefresh(win);

	wattroff(win, COLOR_PAIR(c));
	return;
}
void display_trim(struct instruction_t *inst)
{

	switch((inst->data & 0xF0) >> 4){
	case 0x9:
		if(trim < 8){
			trim++;
		}
		print_trim(ltrim, trim);
		print_trim(rtrim, -trim);
		break;

	case 0xA:
		if(trim > -8){
			trim--;
		}
		print_trim(ltrim, trim);
		print_trim(rtrim, -trim);
		break;


	case 0xB:
		trim = 0;
		print_trim(ltrim, trim);
		print_trim(rtrim, -trim);
		break;


	}

	return;
}

void update_speed(struct instruction_t *inst)
{
	if(has_speed(inst)){
		display_speed(inst);
	}else if(is_trim(inst)){
		display_trim(inst);
	}

	return;
}
void display_inst(struct instruction_t *inst)
{

	display_mode(inst);
	switch(type_of_inst(inst)){
			case SENSORDATA:
				display_sensor(inst);
				break;

			case REGLERING:
				display_reg(inst);
				break;

			case SPECKOMMANDO:
				display_spec(inst);
				break;

			case UNKNOWN_DATA:
				display_error(inst);
				break;
			}
	return;
}
