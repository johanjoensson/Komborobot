#include <avr/io.h>
#include <avr/interrupt.h>
#include "upptack_tejp.h"
#include "sensor_ad.h"
#include "sensor_spi.h"
#include "avstandsskillnad.h"
#include "linjeskillnad.h"

#define DELTA 2

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Markning
 *  Description:  Tar emot nuvarande v�rde p� mittensensorn
 *	Input:			Nuvarande v�rde p� n�gon av de mittre sensorerna, 1 f�r tejp 0 annars
 *	Output:			1 om markering f�r h�gersv�ng uppt�ckts, 2 om v�nstersv�ng uppt�ckts, 3 om rakt fram uppt�ckts, 0 annars
 * =====================================================================================
 */
int count_2;
unsigned int time1;
unsigned int time2;
int iterations;

//anropa med markning(find_ones(line_array_1));

int find_ones(unsigned char array)
{
		if((array & 0x70) == 0x70){
			return 1;
		}
		else {
			return 0;
		}

}

int markning(int now_value){
	
		
	//Om �r p� Tejp, �ka antalet iterationer
	if(now_value==1){
		iterations++;
	}	//Spara antalet iterationer n�r g�r av f�rsta markeringen	
	else if(iterations > 0 && now_value==0 && count_2==0){
		time1 = iterations;
		iterations=0;
		count_2++;

		
	}	//N�r g�r fr�n tejp till ingen tejp, sl� av timer och spara v�rde i time2
	else if(iterations > 0 && now_value==0 && count_2==1){
		time2 = iterations;
		iterations=0;
		count_2=0;

		if(time1-time2>DELTA){
			//Sv�ng h�ger
			EEDR=0x07;
			return 1;
			
		}
		else if(time2-time1>DELTA){
			//Sv�ng v�nster
			EEDR=0x07;
			return 2;
			
		}
		else{
			//K�r rakt fram
			EEDR=0x07;
			return 3;
		}
	
	}
	else {
			return 0;
	}

	return 0;
}

/*
S�tter maze_mode=1 om roboten befinner sig i en labyrint, annars s�tts
maze_mode=0
*/

void decide_maze_mode(int no_tape)
{
		if(maze_mode==0){
				if((no_tape==1) && (dist_left < 60) && (dist_right < 60)) {
						maze_mode=1;
				}
		}
		else {
				if((no_tape==0) && (dist_left > 60) && (dist_right > 60)) {
						maze_mode=0;
				}
		}
}
		

