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
 *  Description:  Tar emot nuvarande värde på mittensensorn
 *	Input:			Nuvarande värde på någon av de mittre sensorerna, 1 för tejp 0 annars
 *	Output:			1 om markering för högersväng upptäckts, 2 om vänstersväng upptäckts, 3 om rakt fram upptäckts, 0 annars
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
	
	
	
	
	//Om går från ingen tejpmarkering till Tejp, sätt på timer
	if(last_value==0 && now_value==1 && count_2==0){
		TCNT1=0x0000;
		count_2++;
	}
	
	//Om går från Tejp till ingen Tejp, slå av timer och spara värde i time1
	if(last_value==1 && now_value==0 && count_2==1){
		time1 = (int)TCNT1;
		count_2++;
		
	}
	
	//När går från ingen tejp till tejp, slå på timer
	if(last_value==0 && now_value==1 && count_2==2){
		//reset timer
		TCNT1=0x0000;
		count_2++;
	}
	//När går från tejp till ingen tejp, slå av timer och spara värde i time2
	if(last_value==1 && now_value==0 && count_2==3){
		time2 = (int)TCNT1;
	
		
		
		int diff1 = time1 - time2;
		int diff2 = time2 - time1;
		count_2=0;
		if(diff1>DELTA){
			//Sväng höger
			return 1;
			
		}
		else if(diff2>DELTA){
			//Sväng vänster
			return 2;
			
		}
		else{
			//Kör rakt fram
			return 3;
		}
	
	}
	
	last_value=now_value;
	
	

	return 0;
}

