/*-----------------------------------------------------------------------------
 *  Xomvandling
 *      input:  sensorvarde: ett varde för avstandet pa en ickelinjar skala
 *      output: Avstandet i cm
 *-----------------------------------------------------------------------------*/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"sensorvarde_omvandling.h"


int hogeromvandling_front(int sensorvarde){
	
	int cmvarde;

	if(sensorvarde<=125 && sensorvarde>=75){
		cmvarde=round((177-sensorvarde)/2.62);

	}
	else if(sensorvarde<75 && sensorvarde>=50){
		cmvarde=round((123-sensorvarde)/1.25);
	}
	else if(sensorvarde<50 && sensorvarde>=25){
		cmvarde=round((70-sensorvarde)/0.39);
	}
	else if(sensorvarde>125){
		cmvarde=20;
	}
	else{
		cmvarde=120;
	}
	return cmvarde;
}

int hogeromvandling_back(int sensorvarde)
{		
	int cmvarde;

	if(sensorvarde<=127 && sensorvarde>=74){
		cmvarde=round((181-sensorvarde)/2.71);

	}



	else if(sensorvarde<74 && sensorvarde>=50){
		cmvarde=round((121-sensorvarde)/1.21);
	}
	else if(sensorvarde<50 && sensorvarde>=31){
		cmvarde=round((86-sensorvarde)/0.62);
	}
	else if(sensorvarde<31 && sensorvarde>=24){
		cmvarde=round((49-sensorvarde)/0.22);
	}
	else if(sensorvarde>127){
		cmvarde=20;
	}
	else{
		cmvarde=120;
	}
	return cmvarde;
}


int framomvandling(int sensorvarde){
	
	int cmvarde;

	if(sensorvarde<=125 && sensorvarde>=78){
		cmvarde=round((177-sensorvarde)/2.56);

	}
	else if(sensorvarde<78 && sensorvarde>=46){
		cmvarde=round((125-sensorvarde)/1.26);
	}
	else if(sensorvarde<=45 && sensorvarde>=24){
		cmvarde=round((72-sensorvarde)/0.4);
	}
	else if(sensorvarde>125){
		cmvarde=20;
	}
	else{
		cmvarde=120;
	}
	return cmvarde;
}

int vansteromvandling_back(int sensorvarde)
{
	int cmvarde;

	if(sensorvarde<=121 && sensorvarde>=75){
		cmvarde=round((171-sensorvarde)/2.44);

	}
	else if(sensorvarde<75 && sensorvarde>=50){
		cmvarde=round((120-sensorvarde)/1.17);
	}
	else if(sensorvarde<50 && sensorvarde>=23){
		cmvarde=round((73-sensorvarde)/0.4);
	}
	else if(sensorvarde>121){
		cmvarde=20;
	}
	else{
		cmvarde=120;
	}
	return cmvarde;
		
}

int vansteromvandling_front(int sensorvarde){
	
	int cmvarde;

	if(sensorvarde<=127 && sensorvarde>=77){
		cmvarde=round((183-sensorvarde)/2.71);

	}
	else if(sensorvarde<77 && sensorvarde>=50){
		cmvarde=round((128-sensorvarde)/1.31);
	}
	else if(sensorvarde<50 && sensorvarde>=26){
		cmvarde=round((73-sensorvarde)/0.43);
	}
	else if(sensorvarde>=128){
		cmvarde=20;
	}
	else{
		cmvarde=120;
	}
	return cmvarde;
}

int kortvansteromvandling(int sensorvarde)
{
	int cmvarde;
	
	if(sensorvarde<=161 && sensorvarde>146){
		cmvarde=round(165-sensorvarde);
	}
	else if(sensorvarde<=146 && sensorvarde>108){
		cmvarde=round(32-(sensorvarde)*0.17);
	}
	else if(sensorvarde<=108 && sensorvarde>93){
		cmvarde=round(98-(sensorvarde)*0.78);
	}
	else if(sensorvarde>161){
		cmvarde=4;
	}
	else{
		cmvarde=26;
	}
	
	return cmvarde;
}

int korthogeromvandling(int sensorvarde)
{
		int cmvarde;

		if(sensorvarde<=136 && sensorvarde>=84){
			cmvarde=round(12-(sensorvarde*0.06));
			}
		else if(sensorvarde<84 && sensorvarde>=50){
			cmvarde=round(20-(sensorvarde*0.17));
			}
		else if(sensorvarde<50 && sensorvarde>=20){
			cmvarde=round(35-(sensorvarde*0.47));
			}
		else if(sensorvarde>136){
			cmvarde=4;
			}
		else{
			cmvarde=26;
		}
		return cmvarde;
}
