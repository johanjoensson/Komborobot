/*
 * =====================================================================================
 *
 *       Filename:  regulator.c
 *
 *    Description:  Ny PD-reglering sköts här minsann
 *
 *        Version:  1.0
 *        Created:  2012-03-27 19:29:43
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
#include <avr/io.h>
#include <avr/interrupt.h>
#include "styr_spi.h"
#include "regulator.h"

unsigned char speed = 105;
signed char old_distance_right = 0;
signed char old_distance_left = 0;
int old_line = 0;
int angle = 0;

signed char cut(signed char value, signed char max);


/*-----------------------------------------------------------------------------
 *  distance_regulator   
 *      input:  
 *              left_fron: avstånd till vänster (20 --- 120)
 *              right_front: avstånd till höger (20 --- 120)
 *              left_back: avstånd bak till vänster bak (10 --- 80)
 *              right_back: avstånd bak till vänster bak (10 --- 80)
 *      output: 
 *              värde som ska styra motorer (-70 --- 70)
 *-----------------------------------------------------------------------------*/
signed char distance_regulator(unsigned char left_front, unsigned char left_back,
                unsigned char right_front, unsigned char right_back)
{
		speed = 107;
        int Kp = 3;
		int Ka = 1;
        int Kd = 8;

        signed char outvalue=0;

        signed char difference_left = (left_front - left_back);		//+-1 justering beh�vs ej
		signed char difference_right = (right_front) - right_back;	//de visar nog lika �nd�
		signed char difference_left_right_f = right_front - left_back;
		signed char difference_left_right_b = right_back - left_back;
		
		if((left_front == 20) && (left_back == 20)){
				wall = 0;
		}
		else if((right_front == 20) && (right_back == 20)){
				wall = 1;
		}

		if(wall==0){														//h�ger v�gg
				outvalue = -Kp*(difference_right);							// P-delen
//				outvalue=cut(outvalue,25);
				outvalue += -Kd*(difference_right - old_distance_right);    // D-delen
		}
		else if(wall==1){													//v�nster v�gg
				outvalue = Kp*(difference_left);               				// P-delen
//				outvalue=cut(outvalue,25);
				outvalue += Kd*(difference_left - old_distance_left);		// D-delen
		}
				
		old_distance_right = difference_right;
		old_distance_left = difference_left;		

		int temp =-Ka*(difference_left_right_f + difference_left_right_b)/(2);
		outvalue += cut(temp,6);

        // sätter max- och minvärden på utvärdet
       	return cut(outvalue, 22);
}


/*-----------------------------------------------------------------------------
 *  line_regulator
 *  	input: 
 *	   	new_value värde att reglera på
 *  	output: 
 *              värde som ska styra motorer (-70 --- 70)
 *-----------------------------------------------------------------------------*/
signed char line_regulator(signed char new_value)
{
        int Kp = 1;
		speed = 104;
        signed char outvalue;

        //Kollar om roboten rör sig åt höger eller vänster
        if(new_value > old_line){
                angle = -1; // roboten går åt höger
        } else if(new_value < old_line){
                angle = 1; // roboten går åt vänster
        }
		outvalue = (Kp*new_value) >> 2;
        switch(angle){
                case 1:                      
					    if(new_value < 0){   // roboten går åt vänster och är på 
                                             // vänstra sidan om tejpen
                                outvalue += (Kp*new_value) >> 2;
						} else if (new_value >= 90){
								outvalue = 0;
						} else {              // roboten går åt höger, men är på
                                             // högra sidan om tejpen
                                outvalue -= (Kp*new_value) >> 1;
                        }
                        break;
                case -1: 
						if(new_value <= -90){
								outvalue = 0;
                        } else if(new_value <= 0){   // roboten går åt höger och är på 
                                             // vänstra sidan om tejpen
                                outvalue -= (Kp*new_value) >> 1;
                        } else{              // roboten går åt höger och är på
                                             // högra sidan om tejpen
                                outvalue += (Kp*new_value) >> 2;
                        }
                        break;
                default:
                        outvalue = Kp*new_value;
                        break;
        }

        old_line = new_value;

        // sätter max- och minvärden på utvärdet
        if(outvalue > 70){
                outvalue = 70;
        } else if(outvalue < -70){
                outvalue = -70;
        }

        return outvalue;
}

/*-----------------------------------------------------------------------------
 *  drive_engines styr motorerna beroende på value
 *      input: 
 *              value, värde att reglera på
 *-----------------------------------------------------------------------------*/
void drive_engines(signed char value)
{
        if(value > 0){
                OCR2 = speed + 3 - value; // Vänstermotor
                OCR0 = speed; // + value; // Högermotor
        } else {
                value = -value;

                OCR2 = speed + 3; //+ value; // Vänstermotor
                OCR0 = speed - value; // Högermotor
        }
}

/****************************************************************************\
	function: 	cut
	descr:		ser till att v�rden ligger i intervallet [-max,max]
\****************************************************************************/

signed char cut(signed char value, signed char max)
{
		if(value > max){
                value = max;
        } else if(value < -max){
                value = -max;
        }
		return value;
}
