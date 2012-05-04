#include <avr/io.h>
#include <avr/interrupt.h>
#include "styr_SPI.h"
#include "motor_styrning.h"

/*
 *	Hjälpfunktion som tar time, speed och command som argument och 
 *	kör kommandot: 
 *			command		kommando
 *			0			forward
 *			1			rotate_left
 *			2			rotate_right
 *			*			stop
 *	med hastighet speed i tiden time*1024 klockpulser
 */
void special_help(int time, unsigned char speed, unsigned char command)
{
		OCR1A = time;                  // ställer in klockan på 1 sek
        TCCR1B = (1<<CS12) | (1<<CS10);  //prescaler på 16-bits klocka till 1024 
                                                        // och startar klockan
        if(command == 0){
			forward(speed);
		} else if(command == 1){
			rotate_left(speed);
		} else if(command == 2){
			rotate_right(speed);
		} else{
			stop(speed);
		}

        while(!(TIFR & (1<<OCF1A))){    // Vänta tills klockan räknat klart
                               ;  
        }
        stop(0);
        TCCR1B = (0<<CS12) | (0<<CS11) | (0<<CS10); //Stanna klockan
        TCNT1 = 0;                      //Nollställ klockan
		TIFR = 0x10;					// Nollställ avbrottsflagga
		return;
}

void specialkommando(unsigned char kommando_kod)
{
		
        //unsigned char kommando_kod = 0x40;//data & 0xE0;
	

		if(0x40 == kommando_kod)
		{
                        /*Sväng vänster 90 grader
                        Förslag: slå på timer och låt roboten köra rakt fram <tid>,
                        rotera vänster <tid>, kör fram <tid>. jag tror det är enklast
                        att använda flaggan för uppfyllt tidskrav här (man väntar på
                        att den är lika med 1)
                        */
                        special_help(0x2600, 0x08, 0);    // Kör fram i en sekund
                        special_help(0x0F00, 0, 3); 	  // Stanna i en kort stund
                        special_help(0x1800, 0x07,1);     // Rotera vänster ca en 1/3 sekund
                        special_help(0x0F00, 0, 3);       // Stanna i en kort stund
                        special_help(0x3200, 0x08, 0);    // Kör fram i en sekund
                        special_help(0x2000, 0, 3);   
                        return;

		}
		else if(0x60==kommando_kod)
		{
                        //sväng höger 90 grader
                        special_help(0x2600, 0x08, 0);    // Kör fram i en sekund
                        special_help(0x0F00, 0, 3); 	  // Stanna i en kort stund
                        special_help(0x1800, 0x07,2);     // Rotera höger ca en 1/3 sekund
                        special_help(0x0F00, 0, 3);       // Stanna i en kort stund
                        special_help(0x3200, 0x08, 0);    // Kör fram i en sekund
                        special_help(0x2000, 0, 3);   
                        return;
		}
		else if(0x20==kommando_kod)
		{
                        //kör rakt fram
                        special_help(0x5200, 0x08, 0);    // Kör fram i ett tag (ca 2 sek)
                        special_help(0x2000, 0, 3);   	  // Stanna
                        return;
		}

}

int straight(unsigned char dist_left_front, unsigned char dist_left_back, unsigned char dist_right_front, unsigned char dist_right_back)
{
		signed char difference_left = ((dist_left_front -1) - dist_left_back);
		signed char difference_right = (dist_right_front - dist_right_back);

		if(dist_left_front < 40){
				if(difference_left < 2 && difference_left > -2){
						return 0;
				} else if (difference_left < 0){
						unsigned char left_cm = - difference_left;
						special_help(left_cm << 3, 0x07,2);     // Rotera h�ger
						return 1;
				} else {
						unsigned char left_cm = difference_left;
						special_help(left_cm << 3, 0x07,1);     // Rotera vänster
						return 1;
				}
		} else if(dist_right_front < 40){
				if(difference_right < 2 && difference_right > -2){
						return 0;
				} else if (difference_right < 0){
						unsigned char right_cm = - difference_right;
						special_help(right_cm << 3, 0x07,1);     // Rotera h�ger
						return 1;
				} else {
						unsigned char right_cm = difference_left;
						special_help(right_cm << 3, 0x07,2);     // Rotera vänster
						return 1;
				}
		} else {
			return 0;
		}			
}
