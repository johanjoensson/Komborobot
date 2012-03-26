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

void event_loop();

unsigned char header;
unsigned char data;

int main(int argc, char *argv[])
{
        if(argc != 3){
                printf ( "Användning: input_control destination datakod\n" );
                return 1;
        }
        int destination = atoi(argv[1]);
        int datakod = atoi(argv[2]);
        header = generate_header(destination, datakod);
        data = forward(8);
        event_loop();
        printf("Header is 0x%x\n", header);
        printf("Data is 0x%x\n", data);
        return 0;
}

void event_loop()
{
        int error = SDL_Init(SDL_INIT_EVERYTHING);
        if(error == -1){
                printf("SDL error");
                exit(1);
        } 
        SDL_Event event;
        int quit = 0;
        while((quit == 0) && (SDL_WaitEvent(&event))){
                switch(event.type){
                        case SDL_QUIT:
                                quit = 1;
                                break;
                        case SDL_KEYDOWN:
                                switch(event.key.keysym.sym){
                                        case SDLK_ESCAPE:
                                        case SDLK_q:
                                                quit = 1;
                                                break;
                                        default:
                                                break;
                                }
                                break;
                        default:
                                break;
                }
        }
}


void on_w_down()
{
       header = generate_header(2,2);
       data = forward(7);
}
void on_a_down()
{
       header = generate_header(2,2);
       data = rotate_left(7);
}
void on_s_down()
{
       header = generate_header(2,2);
       data = reverse(7);
}
void on_d_down()
{
       header = generate_header(2,2);
       data = rotate_right(7);
}
void on_q_down()
{
       header = generate_header(2,2);
       data = forward_left(7);
}
void on_e_down()
{
       header = generate_header(2,2);
       data = forward_right(7);
}

void on_key_up()
{
       header = generate_header(2,2);
       data = stop();
}
