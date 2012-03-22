/*
 * =====================================================================================
 *
 *       Filename:  sensor.c
 *
 *    Description:  Sensorenhetens funktioner
 *
 *        Version:  1.0
 *        Created:  2012-03-22 17:34:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Gustav (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include<stdlib.h>
#include<stdio.h>


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  calculate_distance_diff
 *  Description:  Beräknar skillnad i avstånd givet avståndssensorvärden
 * =====================================================================================
 */
int calculate_distance_diff(unsigned char distance_left, unsigned char distance_right)
{
        int diff = distance_left - distance_right;
        diff = diff >> 1;
        return diff;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  calculate_diff
 *  Description:  Input: 11bitar med linjesensorvärde (t ex 00001100000)
 *                Output: Värde som ska regleras på (positivt om roboten är till höger 
 *                och negativt om roboten är till vänster).
 * =====================================================================================
 */
int calculate_diff(int value)
{
        int index = get_first_one(value);
        printf("Index är %d\n", index);
        if(index == -1){
                printf("Inga ettor i datat\n");
                return 0;
        }
        switch(index){
                case 0 :
                        return -128;
                case 1:
                        return -102;
                case 2:
                        return -78;
                case 3:
                        return -53;
                case 4:
                        return -25;
                case 5:
                        return 0;
                case 6:
                        return 25;
                case 7:
                        return 53;
                case 8:
                        return 78;
                case 9:
                        return 102;
                case 10:
                        return 127;
                default:
                        return 0;
        }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_first_one
 *  Description:  Funktion som hittar den första ettan från vänster.
 *                Input: value (11-bitar)
 *                Output: index på första ettan från vänster. -1 om ingen etta.
 * =====================================================================================
 */
int get_first_one(int value)
{
        for(int i=0;i<11;i++){
                if(value%2 == 1){
                        return i; 
                }
                value = value >> 1;
        }
        return -1;
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  väntar på avbrott
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
        while(1){
        }
        return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
