/*
Funktionen läser
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "styr_SPI.h"
#include "motor_test.h"
#include "regulator.h"


unsigned char kommando;
unsigned char dist_left;

void tolka_data()
{
		
		if(auto_mode==1)						//autonom
		{
				if(0x02==(header & 0x02))		//D-flagga=1?
				{
						//kör specialkommando
				}
				else if(0x11==(header & 0x11)){		//linjeläge
						drive_engines(line_regulator(data));
				}
				else if(0x01==(header & 0x11)){		//avståndsläge
						if(0x80==(data & 0x80)){
								dist_left=data;
						}
						else {
								drive_engines(distance_regulator(dist_left, data));
						}
				}
		}
		else if(auto_mode==0)				//fjärrstyrd
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
