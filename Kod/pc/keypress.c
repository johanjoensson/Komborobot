/*
 * =====================================================================================
 *
 *       Filename:  keypresses.c
 *
 *    Description:  Innehåller funktioner som kallas när tangenter trycks ner
 *
 *        Version:  1.0
 *        Created:  2012-03-26 10:42:08
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Gustav (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>


/* * ===  FUNCTION  ======================================================================
 *         Name:  stopp
 *  Description:  Input: inget
 *  		  Output: databyte med styrkommandot stopp
 *  		
 * =====================================================================================
 */
unsigned char stop()
{
	return 0x00;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  forward
 *  Description:  Input: speed 0-15
 *  		  Output: databyte med styrkommandot framåt
 *  		
 * =====================================================================================
 */
unsigned char forward(int speed)
{
	if(speed > 15){
		speed = 15;
	}
	return 0x10 + speed;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  forward_left
 *  Description:  Input: speed (0-15)
 *  		  Output: databyte med styrkommandot framåt vänster
 *  		
 * =====================================================================================
 */
unsigned char forward_left(int speed)
{
	if(speed > 15){
		speed = 15;
	}
	return 0x20 + speed;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  forward_right
 *  Description:  Input: speed (0-15)
 *  		  Output: databyte med styrkommandot framåt höger
 *  		
 * =====================================================================================
 */
unsigned char forward_right(int speed)
{
	if(speed > 15){
		speed = 15;
	}
	return 0x30 + speed;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rotate_left
 *  Description:  Input: speed (0-15)
 *  		  Output: databyte med styrkommandot rotera vänster
 *  		
 * =====================================================================================
 */
unsigned char rotate_left(int speed)
{
	if(speed > 15){
		speed = 15;
	}
	return 0x40 + speed;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rotate_right
 *  Description:  Input: speed (0-15)
 *  		  Output: databyte med styrkommandot rotera höger
 *  		
 * =====================================================================================
 */
unsigned char rotate_right(int speed)
{
	if(speed > 15){
		speed = 15;
	}
	return 0x50 + speed;
}

	
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  reverse
 *  Description:  Input: speed (0-15)
 *  		  Output: databyte med styrkommandot back
 *  		
 * =====================================================================================
 */
unsigned char reverse(int speed)
{
	if(speed > 15){
		speed = 15;
	}
	return 0x60 + speed;
}
