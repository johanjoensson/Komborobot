#include <avr/io.h>
#include <avr/interrupt.h>
#include "sensor_ad.h"
#include "sensor_spi.h"
#include "linjeskillnad.h"
#include "upptack_tejp.h"
#include "sensorvarde_omvandling.h"
#include "special.h"
#include "displayenhet.h"

void create_line_array(int trunc_value, int vect_id);
int truncate(unsigned char inbyte);
void start_next_ad();
int get_first_one(int value);
unsigned char control_mux();
int display_ctr = 0;
int ad_counter=0;
int duplicate;
int lost_one_wall=0;



ISR(ADC_vect)
{
        start_next_ad();
}


ISR(TIMER1_OVF_vect)
{
        ADCSRA |= (1<<ADSC);
        count=0;			//starta om
        OCR1A=0x1745;
        OCR1B=0x2E8A;
        ad_counter=0;
}

ISR(TIMER1_COMPA_vect)
{
        PORTC &= ~(1<<PC0) & ~(1<<PC1) & ~(1<<PC6) & ~(1<<PC7);	//kanal 0
        ADMUX |= (1<<MUX0) | (1<<MUX1);		//byt till PA3
        count=7;	//starta på linjesensorer
        ADCSRA |= (1<<ADSC);
        ad_counter++;
        if(ad_counter==1){
                OCR1A=0x45CF;
        }
        else if(ad_counter==3){
                OCR1A=0x7459;
        }
        else if(ad_counter==5){
                OCR1A=0xA2E3;
        }
        else if(ad_counter==7){
                OCR1A=0xD16D;
        }
}

ISR(TIMER1_COMPB_vect)
{
        PORTC &= ~(1<<PC0) & ~(1<<PC1) & ~(1<<PC6) & ~(1<<PC7);	//kanal 0
        ADMUX |= (1<<MUX0) | (1<<MUX1);		//byt till PA3
        count=7;       //starta på linjesensorer
        ADCSRA |= (1<<ADSC);
        ad_counter++;
        if(ad_counter==2){
                OCR1B=0x5D14;
        }
        else if(ad_counter==4){
                OCR1B=0x8B9E;
        }
        else if(ad_counter==6){
                OCR1B=0xBA28;
        }
        else if(ad_counter==8){
                OCR1B=0xE8B2;
        }

}



/* 
 * ===  FUNCTION  =============================================================
 *         Name:  start_next_ad
 *  Description:  Styr muxar, ad och anropar omvandlingar samt anropar bussen
 *                                för att  skicka data. Det här kan ses som
 *                                sensorenhetens huvudprogram då alla
 *				  andra funktioner anropas härifrån.
 * ============================================================================
 */
void start_next_ad()
{
        unsigned char state=control_mux();

        if (state==1){				//left_front klar
                if(maze_mode==1 && auto_mode==1){
                        header = 0xC1;	//Skicka till 
                        // styr&pc med E-flagga
                }
                else if(maze_mode == 0 && auto_mode==1){ 
                        //linjeläge
                        header = 0x91;	//Skicka till pc
                }
                else {
                        header = 0x80;
                }
                data=dist_left_front;
                req_sending();

        }
        else if (state==2){			//left_back klar
                if(maze_mode==1 && auto_mode==1){
                        header = 0xC5;	//Skicka till styr&pc med E-flagga
                }
                else if(maze_mode == 0 && auto_mode==1){  //linjeläge
                        header = 0x95;	//Skicka till pc
                }
                else {
                        header = 0x84;
                }
                data= dist_left_back;
                req_sending();

        }		
        else if (state==3){			//right_front klar
                if(maze_mode==1 && auto_mode==1){
                        header = 0xC9;	//Skicka till styr&pc med E-flagga
                }
                else if(maze_mode == 0 && auto_mode==1){  //linjeläge
                        header = 0x99;	//Skicka till pc
                }
                else {
                        header = 0x88;
                }
                data= dist_right_front;
                req_sending();

        }
        else if (state==4){			//right_back klar
                if(maze_mode==1 && auto_mode==1){
                        header = 0xCD;	//Skicka till styr&pc med E-flagga
                }
                else if(maze_mode == 0 && auto_mode==1){  //linjeläge
                        header = 0x9D;	//Skicka till pc
                }
                else {
                        header = 0x8C;
                }
                data= dist_right_back;
                req_sending();

        }
        else if(state==5){	//front klar
                if(display_ctr == 10){
                        data_to_display(dist_right_front,0x00);
                        data_to_display(dist_left_front,0x01);
                        data_to_display(dist_right_back,0x02);
                        data_to_display(dist_left_back,0x03);
                        data_to_display(dist_front,0x04);
                        if(maze_mode == 1){
                                data_to_display(//
                                        (get_next_special_command()/16+100),//
                                        0x05);
                        }
                        else{
                                data_to_display(//
                                        get_next_special_command()/16,0x05);
                        }
                        display_ctr = 0;
                }
                display_ctr++;
        }		
        else if(state==6){
                header=0x4D;
                data=dist_right_short | 0x80;
                req_sending();
        }
        else if(state==7){
                header=0x4D;
                data=dist_left_short | 0xC0;
                req_sending();				
        }

        else if(state==8){			//linjesensor 0-7 pågår 
                create_line_array(truncate(ADCH), 1);
        }
        else if(state==9){			//linjesensor 8-10 pågår

                create_line_array(truncate(ADCH), 2);
        }


        if (count==17){				//alla linjesensorer omvandlade
                if (maze_mode==0 && auto_mode==1){


                        //kod som kollar om banan är slut

                        data=calculate_diff(line_array_1, line_array_2); 

                        if(data==0x60){		//bana slut?
                                header=0xC3;	//skicka till styr och dator 
                                                //med stopp-kod, D-flagga satt
                        }
                        else {
                                header=0x51;	//Skicka till styr med A- och 
                                                //E-flagga
                        }
                        req_sending();



                        //inga linjer? byt till maze_mode=1 om väggar finns 
                        if(line_array_1==0 && line_array_2==0) {
                                decide_maze_mode(1);
                        }
                }
                else if(maze_mode==1 && auto_mode==1){

                        int temp = markning(find_ones(line_array_1));
                        generate_special_command(temp);



                        int temp2 = search_for_crossroad();

                        if(temp2==1 && 
                                (((dist_right_front > 65) && 
                                (dist_right_back < 65)) || 
                                 ((dist_left_front > 65) && 
                                (dist_left_back < 65)))){
                                //Om en korsning upptackts: skicka 
                                //specialkommandot som ska utforas till 
                                //styrenheten

                                if(get_next_special_command()!=0x10){
                                        send_special_command(
                                                get_next_special_command());
                                        //Resetar next_special_command
                                        generate_special_command(4);
                                }
                                else if(dist_right_front > 65 &&
                                                dist_left_front > 65){
                                        send_special_command(
                                                get_next_special_command());
                                        //Resetar next_special_command
                                        generate_special_command(4);
                                        lost_one_wall=0;
                                }
                                else{
                                        lost_one_wall=1;
                                }

                        }
                        else if(temp2==2){		//vanlig 90 högersväng
                                header=0xC3;
                                if(duplicate==1){
                                        data=0x00;
                                        duplicate=0;
                                }
                                else {
                                        duplicate=1;
                                        data=0x40;
                                }
                                req_sending();
                        }
                        else if(temp2==3){		//vanlig 90 vänstersväng
                                header=0xC3;
                                if(duplicate==1){
                                        data=0x00;
                                        duplicate=0;
                                }
                                else {
                                        duplicate=1;
                                        data=0x50;
                                }
                                req_sending();
                        }

                        if(dist_left_front < 65 && 
                                dist_right_front < 65 && lost_one_wall ==1){
                                lost_one_wall=0;
                                generate_special_command(4);
                        }								

                        //linjer? byt till maze_mode=0 om inga väggar finns
                        if(line_array_1!=0 || line_array_2!=0) {
                                decide_maze_mode(0);
                        }
                }



                create_line_array(0,0);		//Nollställ

        }
        else if (count<17){
                count++;
                ADCSRA |= (1<<ADSC);		//starta nästa omvandling
        }


}


/***************************************************************************\
Namn: control_mux														  
Beskr: Ställer om interna och externa muxar samt anropar omvandlingar,
returnerar ett värde beroende på muxarnas inställning.
\***************************************************************************/


unsigned char control_mux()
{
        switch (count){
                case(0):
                        ADMUX |= (1<<MUX2); 	//byt till PA4			
                        dist_left_front=vansteromvandling_front(ADCH);
                        return 1;
                case(1):
                        ADMUX |= (1<<MUX0);	//byt till PA1
                        ADMUX &= ~(1<<MUX2);
                        dist_left_back=vansteromvandling_back(ADCH);
                        return 2;
                case(2):
                        ADMUX |= (1<<MUX2) | (1<<MUX0);		//byt till PA5

                        dist_right_front=hogeromvandling_front(ADCH);
                        return 3;
                case(3):
                        ADMUX |= (1<<MUX1);	//byt till PA2
                        ADMUX &= ~(1<<MUX0) & ~(1<<MUX2);
                        dist_right_back=hogeromvandling_back(ADCH);
                        return 4;
                case(4):
                        ADMUX |= (1<<MUX2);	//PA6
                        dist_front=framomvandling(ADCH);
                        return 5;
                case(5):
                        ADMUX |= (1<<MUX0);	//PA7
                        dist_right_short=korthogeromvandling(ADCH);	
                        return 6;
                case(6):
                        ADMUX &= ~(1<<MUX2);	//PA3
                        PORTC &= ~(1<<PC0) & ~(1<<PC1) & ~(1<<PC6) & ~(1<<PC7);	
                        //kanal noll på extern mux/demux
                        dist_left_short=kortvansteromvandling(ADCH);
                        return 7;
                case(7):
                        PORTC |= (1<<PC0);	//välj kanal 1
                        return 8;
                case(8):
                        PORTC |= (1<<PC1);
                        PORTC &= ~(1<<PC0);	//kanal 2
                        return 8;
                case(9):
                        PORTC |= (1<<PC0);	//kanal 3
                        return 8;
                case(10):
                        PORTC |= (1<<PC6);
                        PORTC &= ~(1<<PC0) & ~(1<<PC1);		//kanal 4
                        return 8;
                case(11):
                        PORTC |= (1<<PC0);	//kanal 5
                        return 8;
                case(12):
                        PORTC |= (1<<PC1);
                        PORTC &= ~(1<<PC0);	//kanal 6
                        return 8;
                case(13):
                        PORTC |= (1<<PC0);	//kanal 7
                        return 8;
                case(14):
                        PORTC &= ~(1<<PC0) & ~(1<<PC1) & ~(1<<PC6);
                        PORTC |= (1<<PC7);	//kanal 8
                        return 8;
                case(15):
                        PORTC |= (1<<PC0);	//kanal 9
                        return 9;
                case(16):
                        PORTC |= (1<<PC1);
                        PORTC &= ~(1<<PC0);	//kanal 10
                        return 9;
                case(17):
                        ADMUX &= ~(1<<MUX0) & ~(1<<MUX1); 	//PA0
                        PORTC |= (1<<PC0);	//kanal 11
                        return 9;
                default:
                        return 0;
        }
}


/* 
 * ===  FUNCTION  =============================================================
 *         Name:  create_line_array
 *  Description:  Fyller två byte med data där bitarna visar 1 för svart och 0
 *                för vit
 * ============================================================================
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
 * ===  FUNCTION  =============================================================
 *         Name:  truncate
 *  Description:  Trösklar invärdet
 *  		  Input: en byte som ska trösklas
 *  		  Output: 1 om byten är mindre än tröskelvärdet och 0 annars
 * ============================================================================
 */
int truncate(unsigned char inbyte)
{
        if(inbyte > level){
                return 1;
        } else {
                return 0;
        }
}

