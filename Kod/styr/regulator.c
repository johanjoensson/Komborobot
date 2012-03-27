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

int old = 0;
int Kp = 1;
int Kd = 3;

signed char regulator(signed char new)
{
        new = new >> 2;
        int outvalue;
        outvalue = Kp*new; // P-delen
        //outvalue -= Kd*(new-old); // D-delen
        //old = new;
        outvalue -= Kd*(new-old); // D-delen
        old = old >> 1;
        old += new;
        // Inför max- och minvärden
        if(outvalue > 127){
                outvalue = 127;
        } else if(outvalue < -128) {
                outvalue = -128; 
        }
        return outvalue;
}

void drive_engines(signed char value)
{
        if(value > 0){
                //TODO
                //Sväng höger value mycket
        } else {
                //TODO
                // Sväng vänster value mycket
        }
}

int main()
{
       printf("Value is %d\n", regulator(125));
       printf("Value is %d\n", regulator(120));
       printf("Value is %d\n", regulator(110));
       printf("Value is %d\n", regulator(89));
       printf("Value is %d\n", regulator(75));
       printf("Value is %d\n", regulator(53));
       printf("Value is %d\n", regulator(42));
       printf("Value is %d\n", regulator(30));
       printf("Value is %d\n", regulator(10));
       printf("Value is %d\n", regulator(0));
       printf("Value is %d\n", regulator(0));
       printf("Value is %d\n", regulator(0));
       printf("Value is %d\n", regulator(0));
       return 0;
}
