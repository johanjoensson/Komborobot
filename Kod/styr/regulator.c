/*
 * =====================================================================================
 *
 *       Filename:  regulator.c
 *
 *    Description:  PD-reglering sköts här minsann
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
int Kd = 1;
unsigned char speed = 110;


signed char regulator(signed char new_value)
{
        new_value = new_value >> 2;
        int outvalue;
        outvalue = Kp*new_value; // P-delen

        outvalue -= Kd*(new_value-old); // D-delen
		old = new_value;
        // Inför max- och minvärden
        if(outvalue > 70){
                outvalue = 70;
        } else if(outvalue < -70) {
                outvalue = -70; 
        }
        return outvalue;
}

void drive_engines(signed char value)
{
/*        if((value < 10) & (value > -10)){
                        value = 0;
        }
*/        if(value > 0){
                OCR2 = speed + 0 - value; // Vänstermotor
                OCR0 = speed + 2 + value; // Högermotor
        } else {
                value = -value;

                OCR2 = speed + 0 + value; // Vänstermotor
                OCR0 = speed + 2 - value; // Högermotor
        }
}
