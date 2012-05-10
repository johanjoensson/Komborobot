/*
 * =====================================================================================
 *
 *       Filename:  input_control.c
 *
 *    Description:  Hanterar input från tangenbord och genererar instruktion 
 *                  som ska skickas till roboten.
 *
 *        Version:  1.0
 *        Created:  2012-03-25 15:26:34
 *       Revision:  2012-04-05 (by Johan Jönsson) 16:58
 *       Compiler:  gcc
 *
 *         Author:  Gustav (), Johan Jönsson 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include "header.h"
#include "keypress.h"
#include "blue_pc.h"
#include "db.h"

#define WIDTH 80
#define HEIGTH 80
#define BPP 4
#define DEPTH 32

void event_loop(FILE *db);
void on_key_up(struct instruction_t *instr);
void on_e_down(int speed, struct instruction_t *instr);
void on_q_down(int speed, struct instruction_t *instr);
void on_d_down(int speed, struct instruction_t *instr);
void on_s_down(int speed, struct instruction_t *instr);
void on_a_down(int speed, struct instruction_t *instr);
void on_w_down(int speed, struct instruction_t *instr);

void set_left(int speed, struct instruction_t *instr);
void set_right(int speed, struct instruction_t *instr);

void trim_left(int trim, struct instruction_t *instr);
void trim_right(int trim, struct instruction_t *instr);
void no_trim(struct instruction_t *instr);

void calibrate_sensors(int sensor_thresh, struct instruction_t *inst);

void event_loop(FILE *db)
{
	int speed = 7;
        int trim = 0;
        unsigned int sensor_thresh = 0xA8;

	struct instruction_t old_instruction;
	struct instruction_t instruction;

        struct instruction_t *inst = &instruction;
        struct instruction_t *old_inst = &old_instruction;

	int error = SDL_Init(SDL_INIT_VIDEO);
	if(error == -1){
		printf("SDL error");
		exit(1);
	} 
	SDL_Surface *screen;
	SDL_Event event;

	SDL_WM_SetCaption("Hauptquartier", "Hauptquartier");

	screen = SDL_SetVideoMode(WIDTH, HEIGTH, DEPTH, 0);
        SDL_Rect screen_rect;
        screen_rect.x = screen_rect.y = 0;
        screen_rect.w = screen->w;
        screen_rect.h = screen->h; 
/******************************************************************************
 * Färgkoder som används för att visa linjesensorkalibreringskonstantens
 * (sensor_thresh) värde
 *****************************************************************************/
        Uint32 white = SDL_MapRGB(screen->format, 255, 255, 255);
        Uint32 black = SDL_MapRGB(screen->format, 0, 0, 0);
        Uint32 c1 = SDL_MapRGB(screen->format, 0x1F, 0x1F, 0x1F);
        Uint32 c2 = SDL_MapRGB(screen->format, 0x2F, 0x2F, 0x2F);
        Uint32 c3 = SDL_MapRGB(screen->format, 0x3F, 0x3F, 0x3F);
        Uint32 c4 = SDL_MapRGB(screen->format, 0x4F, 0x4F, 0x4F);
        Uint32 c5 = SDL_MapRGB(screen->format, 0x5F, 0x5F, 0x5F);
        Uint32 c6 = SDL_MapRGB(screen->format, 0x6F, 0x6F, 0x6F);
        Uint32 c7 = SDL_MapRGB(screen->format, 0x7F, 0x7F, 0x7F);
        Uint32 c8 = SDL_MapRGB(screen->format, 0x8F, 0x8F, 0x8F);
        Uint32 c9 = SDL_MapRGB(screen->format, 0x9F, 0x9F, 0x9F);
        Uint32 cA = SDL_MapRGB(screen->format, 0xAF, 0xAF, 0xAF);
        Uint32 cB = SDL_MapRGB(screen->format, 0xBF, 0xBF, 0xBF);
        Uint32 cC = SDL_MapRGB(screen->format, 0xCF, 0xCF, 0xCF);
        Uint32 cD = SDL_MapRGB(screen->format, 0xDF, 0xDF, 0xDF);
        Uint32 cE = SDL_MapRGB(screen->format, 0xEF, 0xEF, 0xEF);
        
        Uint32 colors[16];
        colors[0] = black;
        colors[1] = c1;
        colors[2] = c2;
        colors[3] = c3;
        colors[4] = c4;
        colors[5] = c5;
        colors[6] = c6;
        colors[7] = c7;
        colors[8] = c8;
        colors[9] = c9;
        colors[0xA] = cA;
        colors[0xB] = cB;
        colors[0xC] = cC;
        colors[0xC] = cC;
        colors[0xD] = cD;
        colors[0xE] = cE;
        colors[0xF] = white;
        

        SDL_FillRect(screen, &screen_rect, colors[(sensor_thresh & 0xF0) >> 4]);
        SDL_Flip(screen);

	int quit = 0;
        /**********************************************************************
         * Börja hantera användarinstruktioner
         *********************************************************************/
	while((quit == 0) && (SDL_WaitEvent(&event))){
		switch(event.type){
			case SDL_QUIT:
				quit = 1;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_ESCAPE:
						quit = 1;
						break;
					case SDLK_w:
						on_w_down(speed, inst);
						add_to_db(db,inst , 2);
						break;
					case SDLK_a:
						on_a_down(speed, inst);
						add_to_db(db,inst , 2);
						break;
					case SDLK_s:
						on_s_down(speed, inst);
						add_to_db(db,inst , 2);
						break;
					case SDLK_d:
						on_d_down(speed, inst);
						add_to_db(db,inst , 2);
						break;
					case SDLK_q:
						on_q_down(speed, inst);
						add_to_db(db,inst , 2);
						break;
					case SDLK_e:
						on_e_down(speed, inst);
						add_to_db(db,inst , 2);
						break;
					case SDLK_k:
						set_left(speed, inst);
						add_to_db(db,inst , 2);
						break;
					case SDLK_l:
						set_right(speed, inst);
						add_to_db(db,inst , 2);
						break;
					case SDLK_o:
						no_trim(inst);
						add_to_db(db,inst , 2);
						break;
					case SDLK_UP:
						if(speed<15){
							speed++;
						}
						break;
					case SDLK_DOWN:
						if(speed>2){
							speed--;
						}
						break;
					case SDLK_RIGHT:
						old_inst = inst;
						trim_right(trim, inst);
						add_to_db(db, inst, 2);
						inst = old_inst;
						add_to_db(db, inst, 2);
                                                trim = 1 - trim;
						break;
					case SDLK_LEFT:
						old_inst = inst;
						trim_left(trim, inst);
						add_to_db(db, inst, 2);
						inst = old_inst;
						add_to_db(db, inst, 2);
                                                trim = 1 - trim;
						break;

					case SDLK_SPACE:
						on_key_up(inst);
						add_to_db(db, inst, 2);
						break;
                                        case SDLK_c:
                                                calibrate_sensors(sensor_thresh,//
                                                                inst);
                                                add_to_db(db, inst, 2);
                                                break;
                                        case SDLK_u:
                                                if(((sensor_thresh & 0xF0) >> 4)//
                                                                < 0xF){
                                                        sensor_thresh += 0x10;
                                                }
                                                break;
                                        case SDLK_i:
                                                if(((sensor_thresh & 0xF0) >> 4)//
                                                                > 0x0){
                                                        sensor_thresh -=  0x10;
                                                }
                                                break;
					default:
						break;
				}
                                SDL_FillRect(screen, &screen_rect, colors[//
                                                (sensor_thresh & 0xF0) >> 4]);
                                SDL_Flip(screen);
				break;
			default:
				break;
		}

	}
        SDL_Quit();
}


/******************************************************************************
 * Funktioner som genererar instruktioner att skicka
 *****************************************************************************/
void on_w_down(int speed, struct instruction_t *instr)
{
       instr->header = generate_header(2,2);
       instr->data = forward(speed);
}
void on_a_down(int speed, struct instruction_t *instr)
{
       instr->header = generate_header(2,2);
       instr->data = rotate_left(speed);
}
void on_s_down(int speed, struct instruction_t *instr)
{
       instr->header = generate_header(2,2);
       instr->data = reverse(speed);
}
void on_d_down(int speed, struct instruction_t *instr)
{
       instr->header = generate_header(2,2);
       instr->data = rotate_right(speed);
}
void on_q_down(int speed, struct instruction_t *instr)
{
       instr->header = generate_header(2,2);
       instr->data = forward_left(speed);
}
void on_e_down(int speed, struct instruction_t *instr)
{
       instr->header = generate_header(2,2);
       instr->data = forward_right(speed);
}

void set_left(int speed, struct instruction_t *instr)
{
	instr->header = generate_header(2,2);
	instr->data = (unsigned char) 0x70 + speed;
}

void set_right(int speed, struct instruction_t *instr) 
{
	instr->header = generate_header(2,2);
	instr->data = (unsigned char) 0x80 + speed;
}

void trim_left(int trim, struct instruction_t *instr)
{
	instr->header = generate_header(2,2);
	instr->data = (unsigned char) 0x90 + trim;
}

void trim_right(int trim, struct instruction_t *instr)
{
	instr->header = generate_header(2,2);
	instr->data = (unsigned char) 0xA0 + trim;
}

void no_trim(struct instruction_t *instr)
{
	instr->header = generate_header(2,2);
	instr->data = (unsigned char) 0xB0;
}

void on_key_up(struct instruction_t *instr)
{
       instr->header = generate_header(2,2);
       instr->data = stop();
}

void calibrate_sensors(int sensor_thresh, struct instruction_t *inst)
{
	inst->header = generate_header(1,0xF);
	inst->data = sensor_thresh;
}

/******************************************************************************
 * Mainfunktion
 * Starta, öppna, kör och stäng
 *****************************************************************************/
int main()
{
	FILE *f = init_write("instr_db");
        event_loop(f);

	fclose(f);
        return 0;
}
