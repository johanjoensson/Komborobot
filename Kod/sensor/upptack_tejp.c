#include <avr/io.h>
#include <avr/interrupt.h>
#include "upptack_tejp.h"
#include "sensor_ad.h"
#include "sensor_spi.h"
#include "avstandsskillnad.h"
#include "linjeskillnad.h"

#define DELTA 200

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Markning
 *  Description:  Tar emot nuvarande v�rde p� mittensensorn
 *	Input:			Nuvarande v�rde p� n�gon av de mittre sensorerna, 1 f�r tejp 0 annars
 *	Output:			1 om markering f�r h�gersv�ng uppt�ckts, 2 om v�nstersv�ng uppt�ckts, 3 om rakt fram uppt�ckts, 0 annars
 * =====================================================================================
 */
int count_2;
int last_value;
unsigned int time1;
unsigned int time2;

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
	
	
	
	
	//Om g�r fr�n ingen tejpmarkering till Tejp, s�tt p� timer
	if(last_value==0 && now_value==1 && count_2==0){
		TCNT1=0x0000;
		count_2++;
	}
	
	//Om g�r fr�n Tejp till ingen Tejp, sl� av timer och spara v�rde i time1
	if(last_value==1 && now_value==0 && count_2==1){
		time1 = (int)TCNT1;
		count_2++;
		
	}
	
	//N�r g�r fr�n ingen tejp till tejp, sl� p� timer
	if(last_value==0 && now_value==1 && count_2==2){
		//reset timer
		TCNT1=0x0000;
		count_2++;
	}
	//N�r g�r fr�n tejp till ingen tejp, sl� av timer och spara v�rde i time2
	if(last_value==1 && now_value==0 && count_2==3){
		time2 = (int)TCNT1;
	
		
		
		int diff1 = time1 - time2;
		int diff2 = time2 - time1;
		count_2=0;
		if(diff1>DELTA){
			//Sv�ng h�ger
			return 1;
			
		}
		else if(diff2>DELTA){
			//Sv�ng v�nster
			return 2;
			
		}
		else{
			//K�r rakt fram
			return 3;
		}
	
	}
	
	last_value=now_value;
	
	

	return 0;
}

