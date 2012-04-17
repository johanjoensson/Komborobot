/*-----------------------------------------------------------------------------
 *  Xomvandling
 *      input:  sensorvarde: ett varde för avstandet pa en ickelinjar skala
 *      output: Avstandet i cm
 *-----------------------------------------------------------------------------*/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"sensorvarde_omvandling.h"


int hogeromvandling(int sensorvarde){
	
	int cmvarde;

	if(sensorvarde<=123 && sensorvarde>=94){
		cmvarde=round((151-sensorvarde)/1.44);

	}
	else if(sensorvarde<=93 && sensorvarde>=82){
		cmvarde=round((121-sensorvarde)/0.68);
	}
	else if(sensorvarde<=81 && sensorvarde>=66){
		cmvarde=round((91-sensorvarde)/0.21);
	}
	else if(sensorvarde>=124){
		cmvarde=20;
	}
	else{
		cmvarde=120;
	}
	return cmvarde;
}

int vansteromvandling(int sensorvarde){
	
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
	else if(sensorvarde>=126){
		cmvarde=20;
	}
	else{
		cmvarde=120;
	}
	return cmvarde;
}

int framomvandling(int sensorvarde){
	
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
