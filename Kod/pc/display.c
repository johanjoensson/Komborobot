#include <stdio.h>
#include <ncurses.h>
#include "display.h"
#include "blue_pc.h"

#define DEBUG
#undef DEBUG
/******************************************************************************
 * Fönstrets storlek
 *****************************************************************************/
int width, height;

/******************************************************************************
 * Variabel för att hålla koll på motorernas trim
 *****************************************************************************/
int trim = 0;

/******************************************************************************
 * Typ för att hålla koll på robotens olika avståndssensorer
 *****************************************************************************/
enum sensors{
	LEFT_FRONT = 0,
	LEFT_BACK = 1,
	RIGHT_FRONT = 2,
	RIGHT_BACK = 3,
	NONE = 4
};


/******************************************************************************
 * De olika fönster vi visar information i
 *****************************************************************************/
WINDOW *mode, *spec_komm, *lusensor, *llsensor, *rusensor, *rlsensor, *speed, *ltrim, *rtrim, *speed_back, *ltrim_back, *rtrim_back, *err;

/******************************************************************************
 * Initiera ett fönster
 *****************************************************************************/
WINDOW *create_win(int win_height, int win_width, int starty, int startx, char corner, char hline, char vline)
{
	WINDOW *local_win;

	local_win = newwin(win_height, win_width, startx, starty);
	wborder(local_win, vline, vline, hline, hline, corner, corner, corner, corner);

	wrefresh(local_win);

	return local_win;
}

/******************************************************************************
 * Radera givet fönster
 *****************************************************************************/
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
			local_win = lusensor;
			break;
		case 3:
			local_win = rusensor;
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
		case 7:
			local_win = llsensor;
			break;
		case 8:
			local_win = rlsensor;
			break;
                case 9:
                        local_win = err;

		default:
			local_win = NULL;
	}

	if(local_win != NULL){
		wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
		wrefresh(local_win);
	}
	return;
}

/******************************************************************************
 * Initiera grässnittet
 *****************************************************************************/
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
	lusensor = create_win(height >> 2, width/3, 0, height/6, '#', '=', '/');
	llsensor = create_win(height >> 2, width/3, 0, (height/6) << 2, '#', '=', '/');
	rusensor = create_win(height >> 2, width/3, width/3 << 1, height/6, '#', '=', '/');
	rlsensor = create_win(height >> 2, width/3, width/3 << 1, (height/6) << 2, '#', '=', '/');
	speed = create_win((height >> 1) - height/6, width/3, width/3, height/6 << 1, '#', '=', '/');
	ltrim = create_win(height/3 , width/6, width/3, height/3 << 1, '#', '=', '/');
	rtrim = create_win(height/3 , width/6, width >> 1, height/3 << 1, '#', '=', '/');
        err = create_win(height/5, width/3, 0, 5*height/12, '#', '=', '/');
	
	speed_back = speed;
	ltrim_back = ltrim;
	rtrim_back = rtrim;
	return;

}

/******************************************************************************
 * Avsluta gränssnittet
 *****************************************************************************/
void exit_curses()
{
	remove_win(0);
	remove_win(1);
	remove_win(2);
	remove_win(3);
	remove_win(4);
	remove_win(5);
	remove_win(6);
	remove_win(7);
	remove_win(8);
	remove_win(9);


	delwin(mode);
	delwin(spec_komm);
	delwin(lusensor);
	delwin(llsensor);
	delwin(rusensor);
	delwin(rlsensor);
	delwin(speed);
	delwin(ltrim);
	delwin(rtrim);
	delwin(err);
	endwin();
}

/******************************************************************************
 * Töm felmeddelandefönstret
 *****************************************************************************/
void clear_error()
{
        int y,x;
        getmaxyx(err, y, x);

        for(int i = 1; i < x - 1; i++){
                mvwaddch(err, y >> 1, i, ' ');
        }

        wrefresh(err);
        return;
}

/******************************************************************************
 * Visa felmeddelande
 *****************************************************************************/
void display_error()
{
        int y,x;
        getmaxyx(err, y, x);

        for(int i = 1; i < x - 1; i++){
                mvwaddch(err, y >> 1, i, ' ');
        }

        mvwprintw(err, y >> 1, 1, "Okänt kommando skickat till styrenheten");

        wrefresh(err);

	return;
}

/******************************************************************************
 * Maska fram och returnera vilket läge Komborobot befinner sig i
 *****************************************************************************/
enum mode run_mode(struct instruction_t *inst)
{
	if((int)(inst->header & 0x01) == 0){
		return FJARR;
	}else{
		switch((int)(inst->header & 0x10) >> 4){
			case 0:
				return LABYRINT;
			case 1:
				return LINJE;
			default:
				return ERROR;
		}
	}

	return ERROR;
}

/******************************************************************************
 * Visa vilket läge roboten befinner sig i
 *****************************************************************************/
void display_mode(struct instruction_t *inst)
{
	int y, x;
	getmaxyx(mode, y, x);
#ifndef DEBUG
	for(int i = 2; i < x-1; i++){
		mvwaddch(mode, y >> 1, i, ' ');
	}
	switch(run_mode(inst)){
		case LABYRINT:
			mvwaddstr(mode, y >> 1, (x >> 1) - x/12 ,"Kör i labyrint");
			break;
		case LINJE:
			mvwaddstr(mode, y >> 1, (x >> 1) - x/12 ,"Följer linjen");
			break;
		case FJARR:
			mvwaddstr(mode, y >> 1, (x >> 1) - x/12 ,"Fjärrstyrt läge");
			break;
		default:
                        break;
	}

	wrefresh(mode);
#endif
	return;
}

/******************************************************************************
 * Maska fram och returnera vilken typ av data Komborobot har skickat
 *****************************************************************************/
enum data_type_t type_of_inst(struct instruction_t *inst)
{

	switch( (inst->header & 0x02) >> 1 ){ // maska fram D flaggan

		case 1:
			return SPECKOMMANDO;

		case 0:
			return SENSORDATA;

		default:
			return UNKNOWN_COMMAND;
	}
	return UNKNOWN_COMMAND;
}

/******************************************************************************
 * Maska fram och returnera vilket specialkommando Komborobot har skickat till
 * PC
 *****************************************************************************/
enum spec_komm_t type_of_command(struct instruction_t *inst)
{
	switch((inst->data & 0x70) >> 4){
		case 0:
			return REGLER;

		case 1:
			return FRAM;

		case 2:
			return HOGER;

		case 3:
			return VANSTER;

                case 7:
                        return FEL;
		default:
			return UNKNOWN_COMMAND;
	}

	return UNKNOWN_COMMAND;
}

/******************************************************************************
 * Maska fram och returnera vilken sensor som Komborobot har uppdaterat
 *****************************************************************************/
enum sensors which_sensor(struct instruction_t *inst)
{
	switch((int)(inst->header & 0x0C) >> 2){
		
                case 0:
                        return LEFT_FRONT;                
                case 1:
			return LEFT_BACK;
		case 2:
			return RIGHT_FRONT;
		case 3:
			return RIGHT_BACK;
		default:
			return	NONE;
	}
	return NONE;
}

/******************************************************************************
 * Returnera lämpligt fönster att uppdatera med sensorinfo
 *****************************************************************************/
WINDOW* get_sensor_window(enum sensors sensor)
{
#ifndef DEBUG
	switch(sensor){
		case LEFT_BACK:
			return llsensor;
		case LEFT_FRONT:
			return lusensor;
		case RIGHT_FRONT:
			return rusensor;
		case RIGHT_BACK:
			return rlsensor;
		default:
			return NULL;
	}
#endif

	return NULL;
}

/******************************************************************************
 * Visa sensordata på skärmen
 *****************************************************************************/
void display_sensor_data(struct instruction_t *inst, enum sensors sensor)
{
#ifndef DEBUG

        
	int y,x;
	WINDOW *cur_win = get_sensor_window(sensor);

	getmaxyx(cur_win,y,x);

	for(int i = x >> 4; i < x-1; i++){
		mvwaddch(cur_win, y >> 1, i, ' ');
	}
	mvwprintw(cur_win, y >> 1, x >> 4, "%dcm till väggen", (int) (inst->data & 0x7F));
	wrefresh(cur_win);

#endif
	return;
}

/******************************************************************************
 * Visa sensordate skickad fån Komborobot på skärmen
 *****************************************************************************/
void display_sensor(struct instruction_t *inst)
{

        enum sensors cur_sensor = which_sensor(inst);
	display_sensor_data(inst, cur_sensor);

	return;
}

/******************************************************************************
 * Visa specialkommando på skärmen
 *****************************************************************************/
void display_spec(struct instruction_t *inst)
{
#ifndef DEBUG
	int y, x;
	getmaxyx(spec_komm, y, x);


	switch(type_of_command(inst)){
		case REGLER:
                        break;
		case FRAM:
                        clear_error();
			for(int i = 2; i < x-1; i++){
				mvwaddch(spec_komm, y >> 1, i, ' ');
			}



			mvwprintw(spec_komm, y >> 1, x >> 2,  "Kör framåt!");
			break;
		case VANSTER:
                        clear_error();
			for(int i = 2; i < x-1; i++){
				mvwaddch(spec_komm, y >> 1, i, ' ');
			}

			mvwprintw(spec_komm, y >> 1, x >> 2,  "Sväng vänster!");
			break;
		case HOGER:
                        clear_error();
			for(int i = 2; i < x-1; i++){
				mvwaddch(spec_komm, y >> 1, i, ' ');
			}


			mvwprintw(spec_komm, y >> 1, x >> 2,  "Sväng höger!");
			break;
                case FEL:
                        display_error();
                        break;

		default:
                        break;
	}

	wrefresh(spec_komm);
#endif
	return;
}

/******************************************************************************
 * Returnera 1 ifall kommandot är ett trimkommando
 * returnera 0 ifall inte
 *****************************************************************************/
int is_trim(struct instruction_t *inst)
{
        if((inst->header & 0xE0) >> 5 == 0x1){
                return 0;
        }
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

/******************************************************************************
 * Returnera 1 ifall kommandot innehåller hastighet
 *****************************************************************************/
int has_speed(struct instruction_t *inst)
{

        if((inst->header & 0xE0) >> 5 == 0x1){
                return 0;
        }
	switch((inst->data & 0xF0) >> 4){
		case 0x7:
		case 0x8:
		case 0x9:
		case 0xA:
		case 0xB:
                case 0x0:
			return 0;

		default:
			return 1;
	}

	return 1;
}

/******************************************************************************
 * Visa önskad hastighet på roboten på skärmen
 *****************************************************************************/
void display_speed(struct instruction_t *inst)
{
#ifndef DEBUG
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
#endif
	return;
}

/******************************************************************************
 * Visa aktuella trimnivåer på skärmen
 *****************************************************************************/
void print_trim(WINDOW *win, int spd)
{
#ifndef DEBUG
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
#endif
	return;
}

/******************************************************************************
 * Ta fram aktuella trimnivåer och visa dessa
 *****************************************************************************/
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
        default:
                break;


	}

	return;
}

/******************************************************************************
 * Uppdatera hastighetsfönstret
 *****************************************************************************/
void update_speed(struct instruction_t *inst)
{
	if(has_speed(inst)){
		display_speed(inst);
	}else if(is_trim(inst)){
		display_trim(inst);
	}

	return;
}

/******************************************************************************
 * Visa information från Komborobot
 *****************************************************************************/
void display_inst(struct instruction_t *inst)
{
	display_mode(inst);
	switch(type_of_inst(inst)){
			case SENSORDATA:
				display_sensor(inst);
				break;

			case SPECKOMMANDO:
				display_spec(inst);
				break;

			case UNKNOWN_DATA:
				display_error();
				break;
                        default:
                                break;
			}
	return;
}
