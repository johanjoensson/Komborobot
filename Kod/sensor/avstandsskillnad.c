#include<stdio.h>
#include<stdlib.h>


// returns a value based on how far away the robot is from the center of the 
// line. Value is 11 bits that represent the converted values 
// from the line sensor
int calculate_distance_diff(unsigned char distance_left, unsigned char distance_right)
{
        int diff = distance_left - distance_right;
        diff = diff >> 1;
        return diff;
}
