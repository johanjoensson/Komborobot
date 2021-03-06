/*
 * =====================================================================================
 *
 *       Filename:  special.c
 *
 *    Description:  Hanterar riktningsmarkeringar
 *
 *        Version:  1.0
 *        Created:  2012-04-13 13:34:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Gustav (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include"special.h"
#include"sensor_spi.h"
#include"sensor_ad.h"
#include<stdio.h>

unsigned char next_special_command; //= 0x70;

/*-----------------------------------------------------------------------------
 *  set_next_special_command 
 *      input: command
 *  sätter next_special_command till command
 *-----------------------------------------------------------------------------*/
void set_next_special_command(unsigned char command)
{
        next_special_command = command;
}


/*-----------------------------------------------------------------------------
 *  get_next_special_command
 *      output: next_special_command
 *
 *  returnerar nästa specialkommando
 *-----------------------------------------------------------------------------*/
unsigned char get_next_special_command()
{
        return next_special_command;
}


/*-----------------------------------------------------------------------------
 *  generate_special_command
 *      Sätter next_special_command om command_code är 1-4, annars inget
 *      input:  command_code 0 för inget specialkommando
 *                           1 för sväng höger
 *                           2 för sväng vänster
 *                           3 för kör rakt fram
 *                           4 för återuppta vanlig reglering
 *      
 *-----------------------------------------------------------------------------*/
void generate_special_command(unsigned char command_code)
{
        unsigned char special_command = 0x70;
        switch(command_code){
                case 1:         special_command = 0x20;
                                set_next_special_command(special_command);
                                break;
                case 2:         special_command = 0x30;
                                set_next_special_command(special_command);
                                break;
                case 3:         special_command = 0x10;
                                set_next_special_command(special_command);
                                break;
                case 4:         special_command = 0x00;
                                set_next_special_command(special_command);
                                break;
                default:        break;
        }
}


/*-----------------------------------------------------------------------------
 *  send_special_command skickar specialkommando till styrenheten
 *      input: command  0x70 - gör inget
 *                      0x00 - återuppta vanlig reglering
 *                      0x01 - kör rakt fram
 *                      0x03 - sväng vänster
 *                      0x02 - sväng höger
 *-----------------------------------------------------------------------------*/
void send_special_command(unsigned char command)
{
        if(command == 0x70){

                return;
        } else {
                header = 0xC3;
                data = command;
                req_sending();
        }
}
/*-----------------------------------------------------------------------------
 *  search_for_crossroads returnerar 1 om en korsning påträffats
 *      input: void
 *		output: 1 om korsning upptäckts, 0 annars
 *-----------------------------------------------------------------------------*/

int search_for_crossroad(){


		if((dist_right_front>75 && dist_left_front>75)) {
				return 1;
		}
		else if((dist_front>=110 && dist_left_front>75 && dist_left_back>75) ||
				(dist_front>=110 && dist_right_front>75 && dist_right_back>75))
				{
				return 2;
		}
		else if((dist_front>=110 && dist_left_front>=80) ||
                (dist_front>=110 && dist_right_front>=80)){
				return 3;
		}
		else if(dist_right_front >= 80 && dist_right_back >= 80 &&
                                dist_front <= 35){
				return 4;
		}
		else if(dist_left_front >= 80 && dist_left_back >= 80 &&
                                dist_front <= 35){
				return 5;
		}
		else {
				return 0;
		}
}
