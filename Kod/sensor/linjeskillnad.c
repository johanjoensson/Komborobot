#include<stdio.h>
#include<stdlib.h>

int calculate_diff(int value);
int get_first_one(int value);

int main(int argc, char *argv[])
{
        if(argc == 1){
                printf("I wants arguments\n");
                return 1;
        }
        int sensorvalue = atoi(argv[1]);
        int diff = calculate_diff(sensorvalue);
        printf("difference is %d\n", diff);

        return 0;
}

// returns a value based on how far away the robot is from the center of the 
// line. Value is 11 bits that represent the converted values 
// from the line sensor
int calculate_diff(int value)
{
        int index = get_first_one(value);
        printf("Index is %d\n", index);
        if(index == -1){
                printf("No ones in value\n");
                return 0;
        }
        switch(index){
                case 0 :
                        return 127;
                case 1:
                        return 102;
                case 2:
                        return 78;
                case 3:
                        return 53;
                case 4:
                        return 25;
                case 5:
                        return 0;
                case 6:
                        return -25;
                case 7:
                        return -53;
                case 8:
                        return -78;
                case 9:
                        return -103;
                case 10:
                        return -128;
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
