/*
 * Johan Jönsson
 * Testa SDL funktioner
 * 20/02/2012
 */

#define WIDTH 640
#define HEIGTH 800
#define BPP 4
#define DEPTH 32


#include<stdio.h>
#include<SDL.h>

#include "base_functions.h"
#include "base_def.h"

void draw_window(SDL_Surface* screen)
{
	pixel colour;
	colour.r = 125;
	colour.g = 25;
	colour.b = 125;
	
	if(SDL_MUSTLOCK(screen)){
		SDL_UnlockSurface(screen);
	}
	for(int x = 0; x < screen->w; x++){
		for(int y = 0; y < screen->h; y++){
			paint_pixel(screen, x, y*screen->w, colour);
		}
	}
	SDL_UpdateRect(screen,0,0,0,0);
	return;
}

int main(int argc, char* argv[])
{
	SDL_Surface *screen;
	SDL_Event event;

	if (!SDL_Init(SDL_INIT_VIDEO) == 0){
		return 1;
	}

	SDL_WM_SetCaption("SDL Test", "SDL Test");
	
	screen = SDL_SetVideoMode(WIDTH, HEIGTH, DEPTH, 0);
	
	draw_window(screen);

	point p1, p2;
	
	p1.x = 125;
	p1.y = 5;

	p2.x = 135;
	p2.y = 125;
	
	pixel colour;
	colour.r = 255;
	colour.g = 255;
	colour.b = 255;
	colour.a = 255;

	draw_line(screen, p1, p2, colour);

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

	SDL_Quit();
	
	return 0;
}
