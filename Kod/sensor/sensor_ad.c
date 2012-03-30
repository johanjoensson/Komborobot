#include <avr/io.h>
#include <avr/interrupt.h>
#include "sensor_ad.h"

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
				dist_left=convert_to_distance(ADCH);
		}
		else if (count==1){
				ADMUX |= (1<<MUX1);
				ADMUX &= ~(1<<MUX0);				//byt till PA2
				dist_right=convert_to_distance(ADCH);
		}
		else if (count==2){
				ADMUX |= (1<<MUX0);					//byt till PA3
				PORTC &= ~(1<<PC0) & ~(1<<PC1) & ~(1<<PC6) & ~(1<<PC7);	
				//kanal noll på extern mux/demux
				dist_front=convert_to_distance(ADCH);
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
		}

		PORTD = ADCH;

		if (count<13){
			count++;
			ADCSRA |= (1<<ADSC);		//starta nästa omvandling
		}

		if ((count>3)&&(count<12)){
				int temp = truncate(ADCH);
				create_line_array(temp, 1);
		}
		else if (count>11){
				int temp = truncate(ADCH);
				create_line_array(temp, 2);
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
				line_array_1 = (line_array_1<<1);
				line_array_1 |= trunc_value;
		}
		else if(vect_id == 2){
				line_array_2 = (line_array_2<<1);
				line_array_2 |= trunc_value;
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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  calculate_distance_diff
 *  Description:  Beräknar skillnad i avstånd givet avståndssensorvärden
 * =====================================================================================
 */
int calculate_distance_diff(unsigned char distance_left, unsigned char distance_right)
{
        int diff = distance_left - distance_right;
        diff = diff >> 1;
        return diff;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  calculate_diff
 *  Description:  Input: 11bitar med linjesensorvärde (t ex 00001100000)
 *                Output: Värde som ska regleras på (positivt om roboten är till höger 
 *                och negativt om roboten är till vänster).
 * =====================================================================================
 */
int calculate_diff(int value)
{
        int index = get_first_one(value);
        //printf("Index är %d\n", index);
        if(index == -1){
                //printf("Inga ettor i datat\n");
                return 0;
        }
        switch(index){
                case 0 :
                        return -128;
                case 1:
                        return -102;
                case 2:
                        return -78;
                case 3:
                        return -53;
                case 4:
                        return -25;
                case 5:
                        return 0;
                case 6:
                        return 25;
                case 7:
                        return 53;
                case 8:
                        return 78;
                case 9:
                        return 102;
                case 10:
                        return 127;
                default:
                        return 0;
        }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_first_one
 *  Description:  Funktion som hittar den första ettan från vänster.
 *                Input: value (11-bitar)
 *                Output: index på första ettan från vänster. -1 om ingen etta.
 * =====================================================================================
 */
int get_first_one(int value)
{
        for(int i=0;i<11;i++){
                if(value%2 == 1){
                        return i; 
                }
                value = value >> 1;
        }
        return -1;
}
