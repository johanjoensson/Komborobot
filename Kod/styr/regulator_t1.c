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
int old_angle=0;
int old_angle_count=0;


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
		speed = 120;
        int Kp = 3;
		int Ka = 1;
        int Kd = 8;

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
		speed = 110;
        signed char outvalue;

        //Kollar om roboten rör sig åt höger eller vänster
        if(new_value > old_line){
                angle = -1; // roboten går åt höger
				old_angle_count=0;
        } else if(new_value < old_line){
                angle = 1; // roboten går åt vänster
				old_angle_count=0;
		}
		  else if(new_value=old_line && old_angle_count<20){
				angle=old_angle;
				old_angle_count++;
			}
		  else if(new_value=old_line && old_angle_count>=20){
			  output=0;
			  angle=0;
		  }
			  
							
	
	if(angle==1){
		switch(new_value){
			
			
			case -127:
				outvalue = 20;
				break;
			case -90:
				outvalue = 15;
				break;
				
			case -75:
				outvalue = 10;
				break;
			case -50: 
				outvalue = 6;
				break;	
				
			case -25:
				outvalue = 3;
				break;
			case 0:
				outvalue = 1;
				break;
			case 25:
				outvalue = 2;
				break;
				
			case 50:
				outvalue = 3;
				break;
			case 75: 
				outvalue = 5;
				break;	
				
			case 90:
				outvalue = 3;
				break;
			case 127:
				outvalue = 1;
				break;
			default: 
				outvalue = 0;
						break
				}
	}
		else if(angle==-1){
					switch(new_value){
						case -127:
							outvalue = 1;
							break;
						case -90:
							outvalue = 3;
							break;
						
						case -75:
							outvalue = 5;
							break;
						case -50: 
							outvalue = 3;
							break;	
					
						case -25:
							outvalue = 2;
							break;
						case 0:
                        	outvalue = 1;
							break;
						case 25:
							outvalue = 3;
							break;
							
						case 50:
							outvalue = 6;
							break;
						case 75: 
							outvalue = 10;
							break;	
							
						case 90:
							outvalue = 15;
							break;
						case 127:
							outvalue = 20;
							break;
						default: 
							outvalue = 0;
							break
					}
	
			}
	
	old_line = new_value;
	old_angle=angle;

        // sätter max- och minvärden på utvärdet

        return outvalue;
}

/*-----------------------------------------------------------------------------
 *  drive_engines styr motorerna beroende på value
 *      input: 
 *              value, värde att reglera på
 *-----------------------------------------------------------------------------*/
void drive_engines(signed char value)
{
        if(angle < 0){//V�nstersv�ng

				if(value > 20){
					OCR2 = speed - (value >> 1) + 3;  // Vänstermotor
              		OCR0 = speed + (value >> 1); // Högermotor
					}
				else{
					OCR2 = speed - (value+3);  // Vänstermotor
              		OCR0 = speed;			// Högermotor
					}
                
        } else {// H�gersv�ng


				if(value > 20){
					OCR2 = speed + (value >> 1)+3;  // Vänstermotor
              		OCR0 = speed - (value >> 1); // Högermotor
					}
				else{
					OCR2 = speed+3;  // Vänstermotor
              		OCR0 = speed - value;// Högermotor
					}
                
        }
}

