#include<stdio.h>
#include<stdlib.h>


/*-----------------------------------------------------------------------------
 *  calculate_distance_diff
 *              input: distance_left och distance_right
 *              output: int som anger hur mycket roboten ska svänga till 
 *                      höger/vänster
 *-----------------------------------------------------------------------------*/
int calculate_distance_diff(unsigned char distance_left, unsigned char distance_right)
{
        // Reglera inte på stora värden
        if(distance_left > 60 || distance_right > 60){
                return 0;
        } else {
                int diff = distance_left - distance_right;
                diff *= 2;
                return diff;
        }
}
