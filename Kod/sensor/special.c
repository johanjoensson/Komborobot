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

unsigned char next_special_command = 0xE0;

/*
int main(int argc, char *argv[])
{
        generate_special_command(atoi(argv[1]));
	send_special_command(get_next_special_command());
        return 0;
}
*/

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
        unsigned char special_command = 0xE0;
        switch(command_code){
                case 1:         special_command = 0x60;
                                set_next_special_command(special_command);
                                break;
                case 2:         special_command = 0x40;
                                set_next_special_command(special_command);
                                break;
                case 3:         special_command = 0x20;
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
 *      input: command  0xE0 - gör inget
 *                      0x00 - återuppta vanlig reglering
 *                      0x02 - kör rakt fram
 *                      0x04 - sväng vänster
 *                      0x06 - sväng höger
 *-----------------------------------------------------------------------------*/
void send_special_command(unsigned char command)
{
        if(command == 0xE0){
                return;
        } else {
                header = 0xCF;
                data = command;
		//printf("Header is %x\n", header);
		//printf("Data is %x\n", data);
                req_sending();
        }
}
/*-----------------------------------------------------------------------------
 *  search_for_crossroads returnerar 1 om en korsning påträffats
 *      input: void
 *		output: 1 om korsning upptäckts, 0 annars
 *-----------------------------------------------------------------------------*/

int search_for_crossroad(){


		if((dist_right>=60 && dist_left>=60) || (dist_front>=60 && dist_left>=60) || (dist_front>=60 && dist_right>=60)){
				return 1;
		}
		else{
				return 0;
		}
}
