#include<stdio.h>
#include<stdlib.h>

int get_first_one(int value);


// returns a value based on how far away the robot is from the center of the 
// line. Value is 11 bits that represent the converted values 
// from the line sensor
int calculate_diff(int byte1, int byte2)
{
		byte2 &= 0b00000111;
		int value = (byte2 << 8) + byte1;
        int index = get_first_one(value);
        if(index == -1){
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

// Returns the position of the first one in value (value is 11 bits)
// returns -1 if no ones
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
