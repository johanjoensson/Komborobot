/*
Funktionen läser
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "regulator.h"
#include "styr_SPI.h"
#include "motor_test.h"


unsigned char kommando;

void tolka_data()
{
		
		if(auto_mode==1)						//autonom
		{
				if(0x02==(header & 0x02))		//D-flagga=1?
				{
						//kör specialkommando
				}
				else
				{
						drive_engines(regulator(data));
				}
		}
		else if(auto_mode==0)				//fjärrstyd
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
				else if(0x50==kommando)
				{
						rotate_left(data & 0x0F);
				}
				else if(0x60==kommando)
				{
						rotate_right(data & 0x0F);
				}
				else if(0x70==kommando)
				{
						back(data & 0x0F);
				}
		}

}
