#include <avr/io.h>
#include <avr/interrupt.h>
#include "sensor_ad.h"
#include "sensor_spi.h"
#include "avstandsskillnad.h"
#include "linjeskillnad.h"
#include "upptack_tejp.h"
#include "sensorvarde_omvandling.h"
#include "special.h"

void create_line_array(int trunc_value, int vect_id);
int truncate(unsigned char inbyte);
void start_next_ad();
int get_first_one(int value);
unsigned char control_mux();



ISR(ADC_vect)
{
		start_next_ad();
}


ISR(TIMER1_OVF_vect)
{
		ADCSRA |= (1<<ADSC);
		count=0;			//starta om
}

ISR(TIMER1_COMPB_vect)
{
		PORTC &= ~(1<<PC0) & ~(1<<PC1) & ~(1<<PC6) & ~(1<<PC7);	//kanal 0
		ADMUX |= (1<<MUX0) | (1<<MUX1);							//byt till PA3
		count=5;												//starta på linjesensorer
		ADCSRA |= (1<<ADSC);
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  start_next_ad
 *  Description:  Styr muxar, ad och anropar omvandlingar samt anropar bussen för att
 *				  skicka data. Det h�r kan ses som sensorenhetens huvudprogram d� alla
 *				  andra funktioner anropas h�rifr�n.
 * =====================================================================================
 */
void start_next_ad()
{
		unsigned char state=control_mux();

		if (state==1){				//left_front klar
				if(maze_mode==1 && auto_mode==1){
						header = 0xC1;	//Skicka till styr&pc med E-flagga
						data=0x80 | dist_left_front;
						req_sending();
				}
		}
		 else if (state==2){			//left_back klar
				if(maze_mode==1 && auto_mode==1){
						header = 0xC1;	//Skicka till styr&pc med E-flagga
						data= dist_left_back;
						req_sending();
				}
		}
		
		else if (state==3){			//right_front klar
				if(maze_mode==1 && auto_mode==1){
						header = 0xC1;	//Skicka till styr&pc med E-flagga
						data= dist_right_front;
						req_sending();
				}
		}

		
		else if (state==4){			//right_back klar
				if(1){//maze_mode==1 && auto_mode==1){
						header = 0xC1;	//Skicka till styr&pc med E-flagga
						data= dist_right_back;
						req_sending();
				}
		}
		else if(state==5){			//front klar
				//visa på display
		}
		else if(state==6){			//linjesensor 0-7 pågår 
				create_line_array(truncate(ADCH), 1);
		}
		else if(state==7){			//linjesensor 8-10 pågår
				create_line_array(truncate(ADCH), 2);
		}
		else {
				;
		}
				

		if (count==15){				//alla linjesensorer omvandlade
				if (maze_mode==0 && auto_mode==1){
						data=calculate_diff(line_array_1, line_array_2); 
						header=0x51; 	//Skicka till styr med A- och E-flagga
						req_sending();
						
						//inga linjer? byt till maze_mode=1 om väggar finns 
						if(line_array_1==0 && line_array_2==0) {
								decide_maze_mode(1);
						}
				}
				else if(maze_mode==1 && auto_mode==1){
						
						int temp = markning(find_ones(line_array_1));
						generate_special_command(temp);
						

					
						if(search_for_crossroad()){
							//Om en korsning upptackts: skicka specialkommandot som ska utforas till styrenheten
 							send_special_command(get_next_special_command());
							//Resetar den globala variabeln next_special_command for att forma roboten att uppna vanlig reglering
							generate_special_command(4);
						}
						

						//linjer? byt till maze_mode=0 om inga väggar finns
						if(line_array_1!=0 && line_array_2!=0) {
								decide_maze_mode(0);
						}
				}



				create_line_array(0,0);		//Nollställ

		}
		else if (count<15){
			count++;
			ADCSRA |= (1<<ADSC);		//starta nästa omvandling
		}


}


 /***************************************************************************\
	Namn: control_mux														  
	Beskr: Ställer om interna och externa muxar samt anropar omvandlingar,
		   returnerar ett v�rde beroende p� muxarnas inst�llning.
 \***************************************************************************/


unsigned char control_mux()
{
		switch (count){
		case(0):
				ADMUX |= (1<<MUX2); 					//byt till PA4			
				dist_left_front=vansteromvandling_front(ADCH);
				return 1;
		case(1):
				ADMUX |= (1<<MUX0);					//byt till PA1
				ADMUX &= ~(1<<MUX2);
				dist_left_back=vansteromvandling_back(ADCH);
				return 2;
		case(2):
				ADMUX |= (1<<MUX2) | (1<<MUX0);		//byt till PA5
				
				dist_right_front=hogeromvandling_front(ADCH);
				return 3;
		case(3):
				ADMUX |= (1<<MUX1);					//byt till PA2
				ADMUX &= ~(1<<MUX0) & ~(1<<MUX2);
				dist_right_back=hogeromvandling_back(ADCH);
				return 4;
		case(4):
				ADMUX |= (1<<MUX0);					//byt till PA3
				PORTC &= ~(1<<PC0) & ~(1<<PC1) & ~(1<<PC6) & ~(1<<PC7);	
				//kanal noll på extern mux/demux
				dist_front=framomvandling(ADCH);
				return 5;
		case(5):
				PORTC |= (1<<PC0);					//välj kanal 1
				return 6;
		case(6):
				PORTC |= (1<<PC1);
				PORTC &= ~(1<<PC0);					//kanal 2
				return 6;
		case(7):
				PORTC |= (1<<PC0);					//kanal 3
				return 6;
		case(8):
				PORTC |= (1<<PC6);
				PORTC &= ~(1<<PC0) & ~(1<<PC1);		//kanal 4
				return 6;
		case(9):
				PORTC |= (1<<PC0);					//kanal 5
				return 6;
		case(10):
				PORTC |= (1<<PC1);
				PORTC &= ~(1<<PC0);					//kanal 6
				return 6;
		case(11):
				PORTC |= (1<<PC0);					//kanal 7
				return 6;
		case(12):
				PORTC &= ~(1<<PC0) & ~(1<<PC1) & ~(1<<PC6);
				PORTC |= (1<<PC7);					//kanal 8
				return 6;
		case(13):
				PORTC |= (1<<PC0);					//kanal 9
				return 7;
		case(14):
				PORTC |= (1<<PC1);
				PORTC &= ~(1<<PC0);					//kanal 10
				return 7;
		case(15):
				ADMUX &= ~(1<<MUX0) & ~(1<<MUX1); 	//PA0
				PORTC |= (1<<PC0);					//kanal 11
				return 7;
		default:
				return 0;
		}
				
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  create_line_array
 *  Description:  Fyller två byte med data där bitarna visar 1 för svart och 0 för vit
 * =====================================================================================
 */
void create_line_array(int trunc_value, int vect_id)
{
		if (vect_id==1){
				line_array_1 = (line_array_1>>1);
				line_array_1 |= (trunc_value<<7);
		}
		else if(vect_id == 2){
				line_array_2 = (line_array_2>>1);
				line_array_2 |= (trunc_value<<2);
		}
		else{
				line_array_2 = 0;
				line_array_1 = 0;
		}
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  truncate
 *  Description:  Trösklar invärdet
 *  		  Input: en byte som ska trösklas
 *  		  Output: 1 om byten är mindre än tröskelvärdet och 0 annars
 * =====================================================================================
 */
int truncate(unsigned char inbyte)
{
		if(inbyte > level){
				return 1;
		} else {
				return 0;
		}
}

