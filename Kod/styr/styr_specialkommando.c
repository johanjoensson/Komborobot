#include <avr/io.h>
#include <avr/interrupt.h>
#include "styr_SPI.h"
#include "motor_test.h"

void specialkommando()
{
        unsigned char kommando_kod = 0x40;//data & 0xE0;

		if(0x40 == kommando_kod)
		{
                        /*Sväng vänster 90 grader
                        Förslag: slå på timer och låt roboten köra rakt fram <tid>,
                        rotera vänster <tid>, kör fram <tid>. jag tror det är enklast
                        att använda flaggan för uppfyllt tidskrav här (man väntar på
                        att den är lika med 1)
                        */
                        OCR1A = 0x3E80;                  // ställer in klockan på 1 sek
                        TCCR1B = (1<<CS12) | (1<<CS10);  //prescaler på 16-bits klocka till 1024 
                                                        // och startar klockan
                        forward(0x0A);
                        while(!(TIFR & (1<<OCF1A))){    // Vänta tills klockan räknat klart
                               ;  
                        }
                        stop(0);

                        TCCR1B = (0<<CS12) | (0<<CS11) | (0<<CS10); //Stanna klockan
                        TCNT1 = 0;                      //Nollställ klockan
                        OCR1A = 0x0FFF;                 // ställer in klockan på 0.25s
                        TCCR1B = (1<<CS12) | (1<<CS10);  //starta klockan
                        rotate_left(0x0A);
                        while(!(TIFR & (1<<OCF1A))){    // Vänta tills klockan räknat klart
                               ;  
                        }
                        stop(0);
                        TCCR1B = (0<<CS12) | (0<<CS11) | (0<<CS10); //Stanna klockan
                        TCNT1 = 0;                      //Nollställ klockan
                        OCR1A = 0x3E80;                  // ställer in klockan på 1 sek
                        TCCR1B = (1<<CS12) | (1<<CS10);  //prescaler på 16-bits klocka till 1024 
                                                        // och startar klockan
                        forward(0x0A);
                        while(!(TIFR & (1<<OCF1A))){    // Vänta tills klockan räknat klart
                               ;  
                        }
                        TCCR1B = (0<<CS12) | (0<<CS11) | (0<<CS10); //Stanna klockan
                        TCNT1 = 0;                      //Nollställ klockan
                        return;

		}
		else if(0x60==kommando_kod)
		{
                        //sväng höger 90 grader
                        OCR1A = 0x3E80;                  // ställer in klockan på 1 sek
                        TCCR1B = (1<<CS12) | (1<<CS10);  //prescaler på 16-bits klocka till 1024 
                                                        // och startar klockan
                        forward(0x0A);
                        while(!(TIFR & (1<<OCF1A))){    // Vänta tills klockan räknat klart
                               ;  
                        }
                        stop(0);

                        TCCR1B = (0<<CS12) | (0<<CS11) | (0<<CS10); //Stanna klockan
                        TCNT1 = 0;                      //Nollställ klockan
                        OCR1A = 0x0FA0;                 // ställer in klockan på 0.25s
                        TCCR1B = (1<<CS12) | (1<<CS10);  //starta klockan
                        rotate_right(0x0A);
                        while(!(TIFR & (1<<OCF1A))){    // Vänta tills klockan räknat klart
                               ;  
                        }
                        stop(0);
                        TCCR1B = (0<<CS12) | (0<<CS11) | (0<<CS10); //Stanna klockan
                        TCNT1 = 0;                      //Nollställ klockan
                        OCR1A = 0x3E80;                  // ställer in klockan på 1 sek
                        TCCR1B = (1<<CS12) | (1<<CS10);  //prescaler på 16-bits klocka till 1024 
                                                        // och startar klockan
                        forward(0x0A);
                        while(!(TIFR & (1<<OCF1A))){    // Vänta tills klockan räknat klart
                               ;  
                        }
                        TCCR1B = (0<<CS12) | (0<<CS11) | (0<<CS10); //Stanna klockan
                        TCNT1 = 0;                      //Nollställ klockan
                        return;
		}
		else if(0x20==kommando_kod)
		{
                        //kör rakt fram
                        OCR1A = 0xBB80;                  // ställer in klockan på 1 sek
                        TCCR1B = (1<<CS12) | (1<<CS10);  //prescaler på 16-bits klocka till 1024 
                                                        // och startar klockan
                        forward(8);
                        while(!(TIFR & (1<<OCF1A))){    // Vänta tills klockan räknat klart
                               ;  
                        }
                        TCCR1B = (0<<CS12) | (0<<CS11) | (0<<CS10); //Stanna klockan
                        TCNT1 = 0;                      //Nollställ klockan
                        return;
		}
}
