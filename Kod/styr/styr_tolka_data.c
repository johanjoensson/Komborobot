/*
Funktionen läser
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "styr_SPI.h"
#include "motor_styrning.h"
#include "regulator.h"
#include "styr_specialkommando.h"
#include "styr_tolka_data.h"

unsigned char kommando;
unsigned char dist_left_front, dist_left_back, dist_right_front;
unsigned char count;
int done_with_special_command;

void tolka_data()
{
		
		if(auto_mode==1 && start==1)						//autonom
		{
				if(0x02==(header & 0x02))		//D-flagga=1?
				{
						specialkommando(data & 0x70);
						done_with_special_command = 1;
				}
				else if(0x11==(header & 0x11)){		//linjeläge
						drive_engines_line(line_regulator(data));
				}
				else if(0x01==(header & 0x11)){		//avståndsläge
						if(0x00==(header & 0x0C)){
								dist_left_front= data;
								count=1;
						} else if (count==1 && 0x04==(header & 0x0C)){
								dist_left_back=data;
								count++;
						} else if(count==2 && 0x08==(header & 0x0C)){
								dist_right_front=data;
								count++;
						}
						else if(count==3 && 0x0C==(header & 0x0C)){
								drive_engines(distance_regulator(
								dist_left_front, dist_left_back, 
								dist_right_front, data));
						}
				}
		}
		else if(auto_mode==0 && (header & 0x01) == 0)				//fjärrstyrd
		{
				kommando=(data & 0xF0);

				if(0x00==kommando)
				{
						stop(data & 0x0F);
				}
				else if(0x10==kommando)
				{
						forward(data & 0x0F);
				}
				else if(0x20==kommando)
				{
						forward_left(data & 0x0F);
				}
				else if(0x30==kommando)
				{
						forward_right(data & 0x0F);
				}
				else if(0x40==kommando)
				{
						rotate_left(data & 0x0F);
				}
				else if(0x50==kommando)
				{
						rotate_right(data & 0x0F);
				}
				else if(0x60==kommando)
				{
						back(data & 0x0F);
				}
				else if(0x70==kommando)
				{
						set_speed_left(data & 0x0F);
				}
				else if(0x80==kommando)
				{
						set_speed_right(data & 0x0F);
				}
				else if(0x90==kommando)			//Trim-funktionerna ska köras en gång, sedan skall föregående kommando fortsätta.
				{
						trim_left();
				}
				else if(0xA0==kommando)
				{
						trim_right();
				}
				else if(0xB0==kommando)
				{
						trim_zero();
				}
		}

}

