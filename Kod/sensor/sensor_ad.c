#include <avr/io.h>
#include <avr/interrupt.h>
#include "sensor_ad.h"
#include "sensor_spi.h"
#include "avstandsskillnad.h"
#include "linjeskillnad.h"
#include "upptack_tejp.h"
#include "sensorvarde_omvandling.h"
//inkludera fšr tejpmarkeringar?

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



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  start_next_ad
 *  Description:  Styr muxar, ad och anropar omvandlingar
 * =====================================================================================
 */
void start_next_ad()
{
		int state=control_mux();

		if (state==1){				//left&right klara
				header = 0x00;
				//Obs, dum ide! AnvŠnd cm vŠrde
				data=calculate_distance_diff(dist_left, dist_right);
				req_sending();
		}
		else if(state==2){			//front klar

		}
		else if(state==3){			//linjesensor 0-7 klara
				int temp = truncate(ADCH);
				create_line_array(temp, 1);
		}
		else if(state==4){			//
				int temp = truncate(ADCH);
				create_line_array(temp, 2);
		}
		else {
				;
		}
				

		if (count==13){
				if (maze_mode==0){
						data=calculate_diff(line_array_1, line_array_2); 
						decide_header();
						req_sending();
				}
				else if(maze_mode==1){
						EEDR=markning(find_ones(line_array_1));
				}


				if(line_array_1==0 && line_array_2==0) {
						decide_maze_mode(1);
				}

				create_line_array(0,0);		//Nollställ

		}
		else if (count<13){
			count++;
			ADCSRA |= (1<<ADSC);		//starta nästa omvandling
		}


						//Rå linjedata till PC
/*						header= (0x80 | line_array_2);
						data=line_array_1;
						req_sending();
*/


}

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
				//kanal noll på extern mux/demux
				dist_front=framomvandling(ADCH);
				return 2;
		case(3):
				PORTC |= (1<<PC0);					//välj kanal 1
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

void decide_header()
{
		header = 0x41;		//sätter E-falaggan
}

