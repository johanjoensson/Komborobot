/*
 * =====================================================================================
 *
 *       Filename:  sensor.c
 *
 *    Description:  Sensorenhetens funktioner
 *
 *        Version:  1.0
 *        Created:  2012-03-22 17:34:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Gustav 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "sensor_ad.h"


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
        printf("Index är %d\n", index);
        if(index == -1){
                printf("Inga ettor i datat\n");
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

/*
Skapad av: Tobias
Datum: 19/3 2012

Koden ställer in de interna och externa muxarna för adomvandlaren.
Omvandlingskedjan startas av en intern timer ca var 35 ms. (16Mhz).
De digitala omvandlas till avstånd i convert_to_distance (behöver skrivas)
*/

ISR(ADC_vect)
{
		start_next_ad();
}


ISR(TIMER1_OVF_vect)
{
		ADCSRA |= (1<<ADSC);
		count=0;			//starta om
}

volatile int a=0;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * 		Koden ställer in de interna och externa muxarna för adomvandlaren.
 *		Omvandlingskedjan startas av en intern timer ca var 35 ms. (16Mhz).
 *		De digitala omvandlas till avstånd i convert_to_distance (behöver skrivas)
 * =====================================================================================
 */
int main(void){
		
		ADMUX |= (1<<ADLAR);				//Vänsterjustering
		ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
		//Tillåt ADC avbrott, dela frekvens med 128
		DDRC |= (1<<DDC0) | (1<<DDC1) | (1<<DDC6) | (1<<DDC7);
		PORTC |= (1<<PC0) | (1<<PC1) | (1<<PC6) | (1<<PC7);
		TCCR1B |= (1<<CS11);		//Prescaler delar med 8, startar räknare
		TIMSK |= (1<<TOV1);			//Tillåt interrupts på timer overflow
		SREG |= (1<<7);				//Globala avbrott
		//test
		DDRD = 0xFF;
		//test end

                while (a<255) {
                                a++;
                                if (a>254) {
                                        a = 1;
                                }
                }
}
