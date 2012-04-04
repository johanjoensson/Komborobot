#include <avr/io.h>
#include <avr/interrupt.h>
#include "sensor_ad.h"
#include "sensor_spi.h"
#include "avstandsskillnad.h"
#include "linjeskillnad.h"

void create_line_array(int trunc_value, int vect_id);
int truncate(unsigned char inbyte);
int convert_to_distance(unsigned char analog_distance);
void start_next_ad();
int get_first_one(int value);



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

		if (count==0){
				ADMUX |= (1<<MUX0); 				//byt till PA1			
				dist_left=ADCH;
		}
		else if (count==1){
				ADMUX |= (1<<MUX1);
				ADMUX &= ~(1<<MUX0);				//byt till PA2
				dist_right=ADCH;
				header = 0x00;
				data=calculate_distance_diff(dist_left, dist_right);
				req_sending();
			
		}
		else if (count==2){
				ADMUX |= (1<<MUX0);					//byt till PA3
				PORTC &= ~(1<<PC0) & ~(1<<PC1) & ~(1<<PC6) & ~(1<<PC7);	
				//kanal noll på extern mux/demux
				dist_front=convert_to_distance(ADCH);
				header = 0x00;
				data=ADCH;
				req_sending();
		}
		else if (count==3){
				PORTC |= (1<<PC0);					//välj kanal 1
		}
		else if (count==4){
				PORTC |= (1<<PC1);
				PORTC &= ~(1<<PC0);					//kanal 2
		}
		else if (count==5){
				PORTC |= (1<<PC0);					//kanal 3
		}
		else if (count==6){
				PORTC |= (1<<PC6);
				PORTC &= ~(1<<PC0) & ~(1<<PC1);		//kanal 4
		}
		else if (count==7){
				PORTC |= (1<<PC0);					//kanal 5
		}
		else if (count==8){
				PORTC |= (1<<PC1);
				PORTC &= ~(1<<PC0);					//kanal 6
		}
		else if (count==9){
				PORTC |= (1<<PC0);					//kanal 7
		}
		else if (count==10){
				PORTC &= ~(1<<PC0) & ~(1<<PC1) & ~(1<<PC6);
				PORTC |= (1<<PC7);					//kanal 8
		}
		else if (count==11){
				PORTC |= (1<<PC0);					//kanal 9
		}
		else if (count==12){
				PORTC |= (1<<PC1);
				PORTC &= ~(1<<PC0);					//kanal 10
		}
		else if (count==13){
				ADMUX &= ~(1<<MUX0) & ~(1<<MUX1); 	//PA0
				PORTC |= (1<<PC0);					//kanal 11
				count++;
		}


		if (count<13){
			count++;
			ADCSRA |= (1<<ADSC);		//starta nästa omvandling

		}

		if ((count>3)&&(count<12)){
				int temp = truncate(ADCH);
				create_line_array(temp, 1);
		}
		else if (count<15){
				int temp = truncate(ADCH);
				create_line_array(temp, 2);
				if (count==14)
				{
						//Rå linjedata till PC
/*						header= (0x80 | line_array_2);
						data=line_array_1;
						req_sending();
*/
						data=calculate_diff(line_array_1, line_array_2); 
						decide_header();
						req_sending();
						create_line_array(0,0);		//Nollställ
				}
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
 *         Name:  convert_to_distance
 *  Description:  Input: digital sensorvärde
 *  		  Output: avstånd i cm
 * =====================================================================================
 */
int convert_to_distance(unsigned char analog_distance)
{
		//TODO
		return 0;
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
