
/*
Fyller en byte med data där bitarna visar 1 för svart och 0 för vit
*/

void create_line_array(int trunc_value, int vect_id);

int truncate(unsigned char inbyte);

/*
Omvandlar digitalt värde till avstånd (trots sitt namn). Koden behöver skrivas.
*/
int convert_to_distance(unsigned char analog_distance);

/*
Styr muxar, startar ad och anropar omvandlingar
*/

void start_next_ad();



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
Denna funktion ska slå upp de digitala värdena i en taball och returnera
vilket avstånd de motsvarar*/

int convert_to_distance(unsigned char analog_distance)
{
		return 0;
}

int truncate(unsigned char inbyte)
{
		if(inbyte < level){
				return 1;
		} else {
				return 0;
		}
}
