#include <avr/io.h>
#include <avr/interrupt.h>
#include "styr_SPI.h"
#include "motor_test.h"

void specialkommando()
{
        unsigned char kommando_kod = 0x40;//data & 0xE0;

		if(0x40 == kommando_kod)
		{
                        /*Sv�ng v�nster 90 grader
                        F�rslag: sl� p� timer och l�t roboten k�ra rakt fram <tid>,
                        rotera v�nster <tid>, k�r fram <tid>. jag tror det �r enklast
                        att anv�nda flaggan f�r uppfyllt tidskrav h�r (man v�ntar p�
                        att den �r lika med 1)
                        */
                        OCR1A = 0x3E80;                  // st�ller in klockan p� 1 sek
                        TCCR1B = (1<<CS12) | (1<<CS10);  //prescaler p� 16-bits klocka till 1024 
                                                        // och startar klockan
                        forward(0x0A);
                        while(!(TIFR & (1<<OCF1A))){    // V�nta tills klockan r�knat klart
                               ;  
                        }
                        stop(0);

                        TCCR1B = (0<<CS12) | (0<<CS11) | (0<<CS10); //Stanna klockan
                        TCNT1 = 0;                      //Nollst�ll klockan
                        OCR1A = 0x0FFF;                 // st�ller in klockan p� 0.25s
                        TCCR1B = (1<<CS12) | (1<<CS10);  //starta klockan
                        rotate_left(0x0A);
                        while(!(TIFR & (1<<OCF1A))){    // V�nta tills klockan r�knat klart
                               ;  
                        }
                        stop(0);
                        TCCR1B = (0<<CS12) | (0<<CS11) | (0<<CS10); //Stanna klockan
                        TCNT1 = 0;                      //Nollst�ll klockan
                        OCR1A = 0x3E80;                  // st�ller in klockan p� 1 sek
                        TCCR1B = (1<<CS12) | (1<<CS10);  //prescaler p� 16-bits klocka till 1024 
                                                        // och startar klockan
                        forward(0x0A);
                        while(!(TIFR & (1<<OCF1A))){    // V�nta tills klockan r�knat klart
                               ;  
                        }
                        TCCR1B = (0<<CS12) | (0<<CS11) | (0<<CS10); //Stanna klockan
                        TCNT1 = 0;                      //Nollst�ll klockan
                        return;

		}
		else if(0x60==kommando_kod)
		{
                        //sv�ng h�ger 90 grader
                        OCR1A = 0x3E80;                  // st�ller in klockan p� 1 sek
                        TCCR1B = (1<<CS12) | (1<<CS10);  //prescaler p� 16-bits klocka till 1024 
                                                        // och startar klockan
                        forward(0x0A);
                        while(!(TIFR & (1<<OCF1A))){    // V�nta tills klockan r�knat klart
                               ;  
                        }
                        stop(0);

                        TCCR1B = (0<<CS12) | (0<<CS11) | (0<<CS10); //Stanna klockan
                        TCNT1 = 0;                      //Nollst�ll klockan
                        OCR1A = 0x0FA0;                 // st�ller in klockan p� 0.25s
                        TCCR1B = (1<<CS12) | (1<<CS10);  //starta klockan
                        rotate_right(0x0A);
                        while(!(TIFR & (1<<OCF1A))){    // V�nta tills klockan r�knat klart
                               ;  
                        }
                        stop(0);
                        TCCR1B = (0<<CS12) | (0<<CS11) | (0<<CS10); //Stanna klockan
                        TCNT1 = 0;                      //Nollst�ll klockan
                        OCR1A = 0x3E80;                  // st�ller in klockan p� 1 sek
                        TCCR1B = (1<<CS12) | (1<<CS10);  //prescaler p� 16-bits klocka till 1024 
                                                        // och startar klockan
                        forward(0x0A);
                        while(!(TIFR & (1<<OCF1A))){    // V�nta tills klockan r�knat klart
                               ;  
                        }
                        TCCR1B = (0<<CS12) | (0<<CS11) | (0<<CS10); //Stanna klockan
                        TCNT1 = 0;                      //Nollst�ll klockan
                        return;
		}
		else if(0x20==kommando_kod)
		{
                        //k�r rakt fram
                        OCR1A = 0xBB80;                  // st�ller in klockan p� 1 sek
                        TCCR1B = (1<<CS12) | (1<<CS10);  //prescaler p� 16-bits klocka till 1024 
                                                        // och startar klockan
                        forward(8);
                        while(!(TIFR & (1<<OCF1A))){    // V�nta tills klockan r�knat klart
                               ;  
                        }
                        TCCR1B = (0<<CS12) | (0<<CS11) | (0<<CS10); //Stanna klockan
                        TCNT1 = 0;                      //Nollst�ll klockan
                        return;
		}
}
