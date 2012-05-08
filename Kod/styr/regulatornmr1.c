/*/*
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
#include "math.h"

unsigned char speed = 105;
//int old_distance_right = 0;
int old_distance_left = 0;
int old_line = 0;
int angle = 0;
int old_angle=0;
int old_angle_count=0;
signed char old_value=0x00;
int rep_count = 0;
int Kd = 1;
signed char old_outvalue = 0x00;
int WARNING=0;

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
		speed = 115;
        signed char outvalue;
		

        //Kollar om roboten rör sig åt höger eller vänster
        
		
		if(new_value==0 && abs(old_line)>75){
				if(old_line<0){
						angle=1;
				}
				else { 
						angle=-1;
				}
				outvalue=35;
				WARNING=1;
				}
		else if(new_value > old_line){
                angle = -1; // roboten går åt höger
				old_angle_count=0;
				WARNING=0;
        } 
		else if(new_value < old_line){
                angle = 1; // roboten går åt vänster
				old_angle_count=0;
				WARNING=0;
		}
		else if(new_value==old_line && old_angle_count<8){
				angle=old_angle;
				old_angle_count++;
				WARNING=0;
		}
		else if(new_value==old_line && old_angle_count>=8){ 
				outvalue=0;
			    angle=0;
			    WARNING=0;
		}
		
		if(rep_count<15){
				rep_count++;
				if(abs(new_value-old_value)>38){
						Kd=2;
						rep_count=19;
		
				}
				else {
						Kd=1;
			
				}
		}
		else {
				old_value=new_value;
				rep_count=0;
		}
	
			  
							
	
	if(angle==1 && WARNING==0){
				switch(new_value){
					
						case -127:
							outvalue = 42;
							break;
						
						case -90:
							outvalue = 40;
							break;
				
						case -75:
							outvalue = 36;
							break;

						case -50: 
							outvalue = 34;
							break;	
				
						case -25:
							outvalue = 28;
							break;
						case 0:
							outvalue = 23;
							break;
						case 25:
							outvalue = 20;
							break;
				
						case 50:
							outvalue = 17;
							break;
						
						case 75: 
							outvalue = 11;
							break;	
					
						case 90:
							outvalue = 7;
							break;
						
						case 127:
							outvalue = 6;
							break;
				
						default: 
							outvalue = 0;
							break;
				}
		}
		else if(angle==-1 && WARNING==0){
					
					switch(new_value){
						case -127:
							outvalue = 6;
							break;
						case -90:
							outvalue = 7;
							break;
						
						case -75:
							outvalue = 11;
							break;
						case -50: 
							outvalue = 17;
							break;	
					
						case -25:
							outvalue = 20;
							break;
						case 0:
                        	outvalue = 23;
							break;
						case 25:
							outvalue = 28;
							break;
							
						case 50:
							outvalue = 34;
							break;
						case 75: 
							outvalue = 36;
							break;	
							
						case 90:
							outvalue = 40;
							break;
						case 127:
							outvalue = 42;
							break;
						default: 
							outvalue = 0;
							break;
					}
	
		}
	
		if(!WARNING){
				old_line = new_value;
		}
	old_angle=angle;
	old_outvalue=outvalue;

        // sätter max- och minvärden på utvärdet

		return(outvalue*Kd);
}

/*-----------------------------------------------------------------------------
 *  drive_engines styr motorerna beroende på value
 *      input: 
 *              value, värde att reglera på
 *-----------------------------------------------------------------------------*/
void drive_engines(signed char value)
{
        if(angle < 0){//V?nstersv?ng

			
					OCR2 = speed - (value+3);  // Vänstermotor
              		OCR0 = speed;			// Högermotor
					
                
        } else {// H?gersv?ng


		
					OCR2 = speed+3;  // Vänstermotor
              		OCR0 = speed - value;// Högermotor
					
                
        }
}
