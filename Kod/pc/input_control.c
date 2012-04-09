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

void event_loop(struct instruction_t *inst, FILE *db);
void on_key_up(struct instruction_t *instr);
void on_e_down(int speed, struct instruction_t *instr);
void on_q_down(int speed, struct instruction_t *instr);
void on_d_down(int speed, struct instruction_t *instr);
void on_s_down(int speed, struct instruction_t *instr);
void on_a_down(int speed, struct instruction_t *instr);
void on_w_down(int speed, struct instruction_t *instr);


void event_loop(struct instruction_t *inst, FILE *db)
{
	int speed = 7;

        int error = SDL_Init(SDL_INIT_EVERYTHING);
        if(error == -1){
                printf("SDL error");
                exit(1);
        } 
        SDL_Surface *screen;
        SDL_Event event;
        SDL_WM_SetCaption("Hauptquartier", "Hauptquartier");

        screen = SDL_SetVideoMode(WIDTH, HEIGTH, DEPTH, 0);

        int quit = 0;
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
                                                printf("Framåt\n");
						//printf("Header = %x\n", inst->header);
						//printf("Data = %x\n", inst->data);
						break;
                                        case SDLK_a:
						on_a_down(speed, inst);
                                                printf("Rotera vänster\n");
						//printf("Header = %x\n", inst->header);
						//printf("Data = %x\n", inst->data);
						break;
                                        case SDLK_s:
						on_s_down(speed, inst);
                                                printf("Back\n");
						//printf("Header = %x\n", inst->header);
						//printf("Data = %x\n", inst->data);
						break;
                                        case SDLK_d:
						on_d_down(speed, inst);
                                                printf("Rotera höger\n");
						//printf("Header = %x\n", inst->header);
						//printf("Data = %x\n", inst->data);
						break;
                                        case SDLK_q:
						on_q_down(speed, inst);
                                                printf("Fram vänster\n");
						//printf("Header = %x\n", inst->header);
						//printf("Data = %x\n", inst->data);
						break;
                                        case SDLK_e:
						on_e_down(speed, inst);
                                                printf("Fram höger\n");
						//printf("Header = %x\n", inst->header);
						//printf("Data = %x\n", inst->data);
						break;
					case SDLK_UP:
						if(speed<15){
							speed++;
							//printf("Speed is %d\n", speed);
						} else {
							printf("Max speed\n");
						}
						break;
					case SDLK_DOWN:
						if(speed>2){
							speed--;
							//printf("Speed is %d\n", speed);
						} else {
							printf("Min speed\n");
						}
						break;
					case SDLK_SPACE:
						printf("Header = %x\n", inst->header);
						printf("Data = %x\n", inst->data);
                                        default:
                                                break;
                                }
                                break;
                        case SDL_KEYUP:
                                switch(event.key.keysym.sym){
					case SDLK_w:
					case SDLK_a:
					case SDLK_s:
					case SDLK_d:
					case SDLK_q:
					case SDLK_e:
						on_key_up(inst);
                                                printf("Stopp\n");
						//printf("Header = %x\n", inst->header);
						//printf("Data = %x\n", inst->data);
						break;
                                        default:
                                                break;
				}
                        default:
                                break;
                }
		add_to_db(db,inst , 2);
        }
}


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

void on_key_up(struct instruction_t *instr)
{
       instr->header = generate_header(2,2);
       instr->data = stop();
}

int main()
{
	FILE *f = init_write("instr_db");
	struct instruction_t *inst = malloc(sizeof(struct instruction_t));
        event_loop(inst, f);
        printf("Header is 0x%x\n", inst->header);
        printf("Data is 0x%x\n", inst->data);

	fclose(f);
        return 0;
}
