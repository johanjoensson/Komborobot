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

unsigned char speed = 110;
int old = 0;


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
        int Kp = 1;
        int Kd = 10;

        signed char outvalue;

        signed char difference = left_front - left_back;

        outvalue = Kp*difference;               // P-delen

        //outvalue += Kd*(difference - old);      // D-delen
        //old = difference;

        // sätter max- och minvärden på utvärdet
        if(outvalue > 70){
                outvalue = 70;
        } else if(outvalue < -70){
                outvalue = -70;
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

        outvalue = Kp*new_value;

        // sätter max- och minvärden på utvärdet
        if(outvalue > 60){
                outvalue = 60;
        } else if(outvalue < -60){
                outvalue = -60;
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
                OCR0 = speed + value; // Högermotor
        } else {
                value = -value;

                OCR2 = speed + value; // Vänstermotor
                OCR0 = speed - value; // Högermotor
        }
}
