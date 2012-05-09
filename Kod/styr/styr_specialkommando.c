#include <avr/io.h>
#include <avr/interrupt.h>
#include "styr_SPI.h"
#include "motor_styrning.h"
#include "styr_tolka_data.h"
#include "regulator.h"

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
        TCCR1B = (1<<CS12) | (1<<CS10); //prescaler på 16-bits klocka till 1024
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
        TIFR = 0x10;	// Nollställ avbrottsflagga
        return;
}


/*
   kommandokoder
   00	undef
   10	kör rakt
   20	sväng höger
   30	sväng vänster
   40	rotera höger
   50	rotera vänster
   60	stanna
   70	inget sparat kommando
   */
void specialkommando(unsigned char kommando_kod)
{

        //unsigned char kommando_kod = 0x40;//data & 0xE0;

        if(0x10==kommando_kod){		//Rakt fram 100 cm
                special_help(0x5200, 0x08, 0);  // Kör fram ett tag (ca 2 sek)
                special_help(0x2000, 0, 3);   	// Stanna
                return;
        }
        else if(0x20==kommando_kod){	//sväng höger 90 grader
                special_help(0x2200, 0x08, 0);  // Kör fram i en sekund
                special_help(0x0F00, 0, 3); 	// Stanna i en kort stund
                special_help(0x1500, 0x07,2);   // Rotera höger
                special_help(0x0F00, 0, 3);     // Stanna i en kort stund
                special_help(0x3600, 0x09, 0);  // Kör fram i en sekund
                special_help(0x2000, 0, 3);   
                return;
        }
        else if(0x30 == kommando_kod){	//Sväng vänster 90 grader
                special_help(0x2200, 0x08, 0);  // Kör fram i en sekund
                special_help(0x0F00, 0, 3); 	// Stanna i en kort stund
                special_help(0x1500, 0x07,1);   // Rotera vänster
                special_help(0x0F00, 0, 3);     // Stanna i en kort stund
                special_help(0x3600, 0x09, 0);  // Kör fram i en sekund
                special_help(0x2000, 0, 3);   
                return;
        }
        else if(0x40==kommando_kod){	//Rotera höger 90
                special_help(0x0F00, 0, 3); 	// Stanna i en kort stund
                special_help(0x1400, 0x07,2);   // Rotera höger
                special_help(0x0F00, 0, 3);     // Stanna i en kort stund
                special_help(0x0010, 0x01, 0);  // Ställ om riktningarna
                // på motorerna
                return;
        }
        else if(0x50 == kommando_kod){	//Rotera vänster 90	
                special_help(0x0F00, 0, 3); 	// Stanna i en kort stund
                special_help(0x1400, 0x07,1);   // Rotera vänster
                special_help(0x0F00, 0, 3);     // Stanna i en kort stund  
                special_help(0x0010, 0x01, 0);  // Ställ om riktningarna
                // på motorerna
                return;						
        }
        else if(0x60==kommando_kod){	//stanna och stäng av autonom styrning
                stop(0);
                start=0;
                return;
        }
        else if(kommando_kod==0x70){		//Inget sparat kommando
                special_help(0x4000, 0, 3); 	// Stanna i en kort stund
                special_help(0x5000, 0x07,1);   // Rotera vänster ca 360 grader
                special_help(0x4000, 0, 3);     // Stanna i en kort stund  
                special_help(0x0010, 0x01, 0);  // Kör fram i en sekund
        }


}


void set_wall(int vaggen){
        wall=vaggen;
}



