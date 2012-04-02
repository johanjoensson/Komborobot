#include <avr/io.h>
#include <avr/interrupt.h>
#include "styr_SPI.h"

void specialkommando()
{
		if(0==data)
		{
				/*Sväng vänster 90 grader
				Förslag: slå på timer och låt roboten köra rakt fram <tid>,
				rotera vänster <tid>, kör fram <tid>. jag tror det är enklast
				att använda flaggan för uppfyllt tidskrav här (man väntar på
				att den är lika med 1)
				*/
		}
		else if(0==data)
		{
				//sväng höger 90 grader
		}
		else if(0==data)
		{
				//kör rakt fram
		}
}
				
