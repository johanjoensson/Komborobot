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

unsigned char speed = 105;
//int old_distance_right = 0;
int old_distance_left = 0;
int old_line = 0;
int angle = 0;


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
		speed = 110;
        int Kp = 3;
		int Ka = 1;
        int Kd = 8;

		header = 0x80;
		data = left_front;
		req_sending();

        signed char outvalue;

        signed char difference_left = (left_front - 1) - left_back;
		//signed char difference_right = (right_front - 1) - right_back;
		signed char difference_left_right = left_front - right_front;

		outvalue = Kp*(difference_left);               // P-delen
		outvalue += Kd*(difference_left - old_distance_left);      // D-delen
		old_distance_left = difference_left;

		outvalue += Ka*(difference_left_right);

        // sätter max- och minvärden på utvärdet
        if(outvalue > 60){
                outvalue = 60;
        } else if(outvalue < -60){
                outvalue = -60;
        }
        return outvalue;
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
						} else {              // roboten går åt höger, men är på
                                             // högra sidan om tejpen
                                outvalue -= (Kp*new_value) >> 1;
                        }
                        break;
                case -1: 
                        if(new_value <= 0){   // roboten går åt höger och är på 
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
                OCR2 = speed - value; // Vänstermotor
                OCR0 = speed; // + value; // Högermotor
        } else {
                value = -value;

                OCR2 = speed; //+ value; // Vänstermotor
                OCR0 = speed - value; // Högermotor
        }
}
