#ifndef MOTOR_STYRNING_H
#define MOTOR_STYRNING_H

/*******************************************************************************
 * Init är en initsiering som anropas för sätta alla register rätt
 * så att körning ska möjliggöras genom att ställa in vilken typ av PWM signal
 * som ska användas och vilka register som ska räkna
 * Inparameter -
 * Retunerar - Alla register satta för att kunna köra motorerna
 ******************************************************************************/
void init();

/*******************************************************************************
 * Motorernas hastighet sätts till noll
 * Inparameter - Behövs inte
 * Retunerar - Hastighetenregistrerna sätts till 0 på båda hjulen
 ******************************************************************************/
void stop(unsigned char speed);

/*******************************************************************************
 * Sätter växeln till framåt och lägger hastigheten på motorerna
 * Inparameter - speed (den önskade hastigheten 0-F)
 * Retunerar - Hastighetsregistrerna på hjulen sätts
 ******************************************************************************/
void forward(unsigned char speed);

/*******************************************************************************
 * Sätter växeln till framåt och lägger hastigheten på motorerna
 * Inparameter - speed (den önskade hastigheten 0-F)
 * Retunerar - Hastighetsregistrerna på hjulen sätts, högre hastighet på höger
 * hjul
 ******************************************************************************/
void forward_left(unsigned char speed);

/*******************************************************************************
 * Sätter växeln till framåt och lägger hastigheten på motorerna
 * Inparameter - speed (den önskade hastigheten 0-F)
 * Retunerar - Hastighetsregistrerna på hjulen sätts, högre hastighet på vänster
 * hjul
 ******************************************************************************/
void forward_right(unsigned char speed);

/*******************************************************************************
 * Sätter växeln till framåt på höger hjul, bakåt på vänster hjul och lägger 
 * hastigheten på motorerna
 * Inparameter - speed (den önskade hastigheten 0-F)
 * Retunerar - Hastighetsregistrerna på hjulen sätts och riktning
 ******************************************************************************/
void rotate_left(unsigned char speed);

/*******************************************************************************
 * Sätter växeln till bakåt på höger hjul, framåt på vänster hjul och lägger 
 * hastigheten på motorerna
 * Inparameter - speed (den önskade hastigheten 0-F)
 * Retunerar - Hastighetsregistrerna på hjulen sätts och riktning
 ******************************************************************************/
void rotate_right(unsigned char speed);

/*******************************************************************************
 * Sätter växeln till bakåt och lägger hastigheten på motorerna
 * Inparameter - speed (den önskade hastigheten 0-F)
 * Retunerar - Hastighetsregistrerna på hjulen sätts
 ******************************************************************************/
void back(unsigned char speed);

/*******************************************************************************
 * Ökar hastigheten på höger hjul
 * Inparameter - 
 * Retunerar - trim minskar. Högre hastighet sätts på högerhjulet
 ******************************************************************************/
void trim_left();

/*******************************************************************************
 * Ökar hastigheten på vänster hjul
 * Inparameter - 
 * Retunerar - trim ökar. Högre hastighet sätts på vänsterhjulet
 ******************************************************************************/
void trim_right();

/*******************************************************************************
 * Nollställer trimen
 * Inparameter - 
 * Retunerar - trim = 0. ingen skillnad i hastighet på hjulen
 ******************************************************************************/
void trim_zero();

/*******************************************************************************
 * Sätter parametrern speed_right och växeln framåt
 * Inparameter - speed (önskad hastighet 0-F)
 * Retunerar - speed_right sätts till önskad hastighet
 ******************************************************************************/
void set_speed_left(unsigned char speed);

/*******************************************************************************
 * Sätter parametrern speed_left och växeln framåt
 * Inparameter - speed (önskad hastighet 0-F)
 * Retunerar - speed_left sätts till önskad hastighet
 ******************************************************************************/
void set_speed_right(unsigned char speed);

/*******************************************************************************
 * Sätter parametrarna speed_right och speed_left
 * Inparameter - speed (önskad hastighet 0-F)
 * Retunerar - speed_right och speed_left sätts till önskad hastighet med 
 * Kalibrering så tt den kör rakare och med trimvärde
 ******************************************************************************/
void set_speed(unsigned char speed);
#endif
