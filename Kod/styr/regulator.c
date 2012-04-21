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

int old = 0;
int Kp = 1;
int Kd = 8;
unsigned char speed = 110;


/*-----------------------------------------------------------------------------
 *  distance_regulator   
 *      input:  
 *              left: avstånd till vänster (20 --- 120)
 *              right: avstånd till höger (20 --- 120)
 *      output: 
 *              värde som ska styra motorer (-70 --- 70)
 *-----------------------------------------------------------------------------*/
signed char distance_regulator(unsigned char left, unsigned char right)
{
        signed char outvalue;

        signed char difference = right - left;
        outvalue = Kp*difference;               // P-delen

        outvalue += Kd*(difference - old);      // D-delen
        old = difference;

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
	// Kod här
	return 0;
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
