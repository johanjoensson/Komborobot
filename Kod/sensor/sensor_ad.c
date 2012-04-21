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
int control_mux();



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
		count=3;
		ADCSRA |= (1<<ADSC);
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  start_next_ad
 *  Description:  Styr muxar, ad och anropar omvandlingar samt anropar bussen f�r att
 *				  skicka data.
 * =====================================================================================
 */
void start_next_ad()
{
		int state=control_mux();

		if (state==1){				//left&right klara
				if(maze_mode==1 && auto_mode==1){
						header = 0x41;	//Skicka till styr med E-flagga
						data=calculate_distance_diff(dist_left, dist_right);
						req_sending();
				}
		}
		else if(state==2){			//front klar
				//visa p� display
		}
		else if(state==3){			//linjesensor 0-7 p�g�r 
				create_line_array(truncate(ADCH), 1);
		}
		else if(state==4){			//linjesensor 8-10 p�g�r
				create_line_array(truncate(ADCH), 2);
		}
		else {
				;
		}
				

		if (count==13){				//alla linjesensorer omvandlade
				if (maze_mode==0 && auto_mode==1){
						data=calculate_diff(line_array_1, line_array_2); 
						header=0x41; 	//Skicka till styr med E-flagga
						req_sending();
						
						//inga linjer? byt till maze_mode=1 om v�ggar finns 
						if(line_array_1==0 && line_array_2==0) {
								decide_maze_mode(1);
						}
				}
				else if(maze_mode==1 && auto_mode==1){
						
						generate_special_command(markning(find_ones(line_array_1)));

						//kod f�r att hitta korsningar och k�r specialkommando in h�r

						//linjer? byt till maze_mode=0 om inga v�ggar finns
						if(line_array_1!=0 && line_array_2!=0) {
								decide_maze_mode(0);
						}
				}



				create_line_array(0,0);		//Nollst�ll

		}
		else if (count<13){
			count++;
			ADCSRA |= (1<<ADSC);		//starta n�sta omvandling
		}

						//R� linjedata till PC
/*						header= (0x80 | line_array_2);
						data=line_array_1;
						req_sending();
*/


}


 /***************************************************************************\
|	Namn: control_mux														  |
|	Beskr: St�ller om interna och externa muxar samt anropar omvandlingar,	  |
|		   returnerar ett v�rde beroende p� muxarnas inst�llning.			  |
 \***************************************************************************/


int control_mux()
{
		switch (count){
		case(0):
				ADMUX |= (1<<MUX0); 				//byt till PA1			
				dist_left=vansteromvandling(ADCH);
				return 0;
		case(1):
				ADMUX |= (1<<MUX1);
				ADMUX &= ~(1<<MUX0);				//byt till PA2
				dist_right=vansteromvandling(ADCH);
				return 1;
		case(2):
				ADMUX |= (1<<MUX0);					//byt till PA3
				PORTC &= ~(1<<PC0) & ~(1<<PC1) & ~(1<<PC6) & ~(1<<PC7);	
				//kanal noll p� extern mux/demux
				dist_front=framomvandling(ADCH);
				return 2;
		case(3):
				PORTC |= (1<<PC0);					//v�lj kanal 1
				return 3;
		case(4):
				PORTC |= (1<<PC1);
				PORTC &= ~(1<<PC0);					//kanal 2
				return 3;
		case(5):
				PORTC |= (1<<PC0);					//kanal 3
				return 3;
		case(6):
				PORTC |= (1<<PC6);
				PORTC &= ~(1<<PC0) & ~(1<<PC1);		//kanal 4
				return 3;
		case(7):
				PORTC |= (1<<PC0);					//kanal 5
				return 3;
		case(8):
				PORTC |= (1<<PC1);
				PORTC &= ~(1<<PC0);					//kanal 6
				return 3;
		case(9):
				PORTC |= (1<<PC0);					//kanal 7
				return 3;
		case(10):
				PORTC &= ~(1<<PC0) & ~(1<<PC1) & ~(1<<PC6);
				PORTC |= (1<<PC7);					//kanal 8
				return 3;
		case(11):
				PORTC |= (1<<PC0);					//kanal 9
				return 4;
		case(12):
				PORTC |= (1<<PC1);
				PORTC &= ~(1<<PC0);					//kanal 10
				return 4;
		case(13):
				ADMUX &= ~(1<<MUX0) & ~(1<<MUX1); 	//PA0
				PORTC |= (1<<PC0);					//kanal 11
				return 4;
		default:
				return 0;
		}
				
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  create_line_array
 *  Description:  Fyller tv� byte med data d�r bitarna visar 1 f�r svart och 0 f�r vit
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
 *  Description:  Tr�sklar inv�rdet
 *  		  Input: en byte som ska tr�sklas
 *  		  Output: 1 om byten �r mindre �n tr�skelv�rdet och 0 annars
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

