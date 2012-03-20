#include<stdio.h>
#include<stdlib.h>

int calculate_distance_diff(unsigned char distance_left, unsigned char distance_right);

int main(int argc, char *argv[])
{
        if(argc == 1){
                printf("Användning: avstandsskillnad avstand_vänster avstånd_höger\n");
                return 1;
        } else if (argc == 2){
                printf("Användning: avstandsskillnad avstand_vänster avstånd_höger\n");
                return 1;
        }
        unsigned char distance_1 = atoi(argv[1]);
        unsigned char distance_2 = atoi(argv[2]);
        int diff = calculate_distance_diff(distance_1, distance_2);
        printf("Skillnad: %d\n", diff);

        return 0;
}

// returns a value based on how far away the robot is from the center of the 
// line. Value is 11 bits that represent the converted values 
// from the line sensor
int calculate_distance_diff(unsigned char distance_left, unsigned char distance_right)
{
        int diff = distance_left - distance_right;
        diff = diff >> 1;
        return diff;
}
