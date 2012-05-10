#include<stdio.h>
#include<stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "sensor_spi.h"

int find_index(int value);
int delay_stop=0;

// returns a value based on how far away the robot is from the center of the 
// line. Value is 11 bits that represent the converted values 
// from the line sensor
int calculate_diff(int byte1, int byte2)
{
        byte2 &= 0b00000111;
        int value = (byte2 << 8) + byte1;
        int index = find_index(value);
        switch(index){
                case 0 :
                        return -127;
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
                        return 90;
                case 10:
                        return 127;
                case 20:
                        return 96;	//Felkod
                default:
                        return 0;
        }
}

// Returns the position of the first one in value (value is 11 bits)
// returns -1 if no ones
int find_index(int value)
{
        int found_one = 0;
        int one_index = 0;
        int zero_index = 10;
        int found_first_zero=0;
        int last = (value%2);
        int number_of_switches=0;
        if(last){
                number_of_switches++; // Räkna som en ?ndring om f?rsta dioden
                // är svart 
        }
        for(int i=0;i<11; i++){
                if(!found_one){
                        if(value%2 == 1){
                                one_index = i;
                                found_one = 1;
                        }

                } else{
                        if(value%2 == 0 && found_first_zero == 0){
                                zero_index = i;
                                found_first_zero=1;
                        }

                }
                if(value%2 != last){
                        number_of_switches++;
                }
                last=value%2;
                value = value >>1;

        }
        if(last){
                number_of_switches++;   // Räkna som ändring om sista dioden är
                                        // svart
        }
        int index=(one_index + zero_index) >> 1;
        if(number_of_switches>5){
                delay_stop++;
                if (delay_stop>5){
                        index=20;
                }
        } else {
                delay_stop=0;
        }
        return index;
}
