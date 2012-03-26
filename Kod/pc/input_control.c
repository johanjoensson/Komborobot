/*
 * =====================================================================================
 *
 *       Filename:  input_control.c
 *
 *    Description:  Hanterar input från tangenbord och genererar header och databytes 
 *                  som ska skickas till roboten.
 *
 *        Version:  1.0
 *        Created:  2012-03-25 15:26:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Gustav (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include "header.h"
#include "keypress.h"

#define WIDTH 80
#define HEIGTH 80
#define BPP 4
#define DEPTH 32

void event_loop();
void on_key_up();
void on_e_down();
void on_q_down();
void on_d_down();
void on_s_down();
void on_a_down();
void on_w_down();

unsigned char header;
unsigned char data;


void event_loop()
{
	int speed = 7;

        int error = SDL_Init(SDL_INIT_EVERYTHING);
        if(error == -1){
                printf("SDL error");
                exit(1);
        } 
        SDL_Surface *screen;
        SDL_Event event;
        SDL_WM_SetCaption("El centro de control", "El centro de control");

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
						on_w_down(speed);
                                                printf("Framåt\n");
						//printf("Header = %x\n", header);
						//printf("Data = %x\n", data);
						break;
                                        case SDLK_a:
						on_a_down(speed);
                                                printf("Rotera vänster\n");
						//printf("Header = %x\n", header);
						//printf("Data = %x\n", data);
						break;
                                        case SDLK_s:
						on_s_down(speed);
                                                printf("Back\n");
						//printf("Header = %x\n", header);
						//printf("Data = %x\n", data);
						break;
                                        case SDLK_d:
						on_d_down(speed);
                                                printf("Rotera höger\n");
						//printf("Header = %x\n", header);
						//printf("Data = %x\n", data);
						break;
                                        case SDLK_q:
						on_q_down(speed);
                                                printf("Fram vänster\n");
						//printf("Header = %x\n", header);
						//printf("Data = %x\n", data);
						break;
                                        case SDLK_e:
						on_e_down(speed);
                                                printf("Fram höger\n");
						//printf("Header = %x\n", header);
						//printf("Data = %x\n", data);
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
						printf("Header = %x\n", header);
						printf("Data = %x\n", data);
                                        default:
                                                break;
                                }
                                break;
                        case SDL_KEYUP:
                                switch(event.key.keysym.sym){
					case SDLK_w:
						on_key_up();
                                                printf("Stopp\n");
						//printf("Header = %x\n", header);
						//printf("Data = %x\n", data);
						break;
                                        case SDLK_a:
						on_key_up();
                                                printf("Stopp\n");
						//printf("Header = %x\n", header);
						//printf("Data = %x\n", data);
						break;
                                        case SDLK_s:
						on_key_up();
                                                printf("Stopp\n");
						//printf("Header = %x\n", header);
						//printf("Data = %x\n", data);
						break;
                                        case SDLK_d:
						on_key_up();
                                                printf("Stopp\n");
						//printf("Header = %x\n", header);
						//printf("Data = %x\n", data);
						break;
                                        case SDLK_q:
						on_key_up();
                                                printf("Stopp\n");
						//printf("Header = %x\n", header);
						//printf("Data = %x\n", data);
						break;
                                        case SDLK_e:
						on_key_up();
                                                printf("Stopp\n");
						//printf("Header = %x\n", header);
						//printf("Data = %x\n", data);
						break;
                                        default:
                                                break;
				}
                        default:
                                break;
                }
        }
}


void on_w_down(int speed)
{
       header = generate_header(2,2);
       data = forward(speed);
}
void on_a_down(int speed)
{
       header = generate_header(2,2);
       data = rotate_left(speed);
}
void on_s_down(int speed)
{
       header = generate_header(2,2);
       data = reverse(speed);
}
void on_d_down(int speed)
{
       header = generate_header(2,2);
       data = rotate_right(speed);
}
void on_q_down(int speed)
{
       header = generate_header(2,2);
       data = forward_left(speed);
}
void on_e_down(int speed)
{
       header = generate_header(2,2);
       data = forward_right(speed);
}

void on_key_up()
{
       header = generate_header(2,2);
       data = stop();
}

int main()
{
        event_loop();
        printf("Header is 0x%x\n", header);
        printf("Data is 0x%x\n", data);
        return 0;
}
