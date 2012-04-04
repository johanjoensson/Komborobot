#include<stdio.h>
#include<stdlib.h>

int get_first_one(int value);
int get_last_one(int index, unsigned char value);


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
		else{
		index = ((index + get_last_one(index, value)) >> 1);
		}
        switch(index){
                case 0 :
                        return -100;
                case 1:
                        return -90;
                case 2:
                        return -75;
                case 3:
                        return -50;
                case 4:
                        return -25;
                case 5:
                        return 0;
                case 6:
                        return 25;
                case 7:
                        return 50;
                case 8:
                        return 75;
                case 9:
                        return 90                 ;
                case 10:
                        return 100;
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
int get_last_one(int index, unsigned char value)
{
		for(int i=10;i>index;i--){
                if(0x40 == (0x40 & value)){
                        return i; 
                }
                value = value << 1;
        }
		return 0;
}
