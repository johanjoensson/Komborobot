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
#include<stdio.h>

unsigned char next_special_command = 0xE0;

//int main(int argc, char *argv[])
//{
//        set_next_special_command(generate_special_command(atoi(argv[1])));
//	send_special_command(get_next_special_command());
//        return 0;
//}

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
 *      input:  command_code 0 för inget specialkommando
 *                           1 för sväng höger
 *                           2 för sväng vänster
 *                           3 för kör rakt fram
 *                           4 för återuppta vanlig reglering
 *      output: ett specialkommando
 *-----------------------------------------------------------------------------*/
unsigned char generate_special_command(unsigned char command_code)
{
        unsigned char special_command = 0xE0;
        switch(command_code){
                case 1:         special_command = 0x60;
                                break;
                case 2:         special_command = 0x40;
                                break;
                case 3:         special_command = 0x20;
                                break;
                case 4:         special_command = 0x00;
                                break;
                default:        special_command = 0xE0;
                                break;
        }
	return special_command;
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