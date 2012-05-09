#ifndef KEYPRESS_H
#define KEYPRESS_H

/******************************************************************************
 * Generera databyte med kommandot stopp
 * Inparametrar -
 * Returvärde   - unsigned char Databyte med kommandot stopp
 *****************************************************************************/
extern unsigned char stop();

/******************************************************************************
 * Generera databyte med kommandot kör framåt, med hastighet speed
 * Inparametrar - int speed Önskad hastighet på roboten
 * Returvärde   - unsigned char Databyte med kommandot kör framåt med hastighet
 * speed
 *****************************************************************************/
extern unsigned char forward(int speed);

/******************************************************************************
 * Generera databyte med kommandot gör en vänstersväng med hastighet speed
 * Inparametrar - int speed
 * Returvärde   - unsigned char Databyte med kommandot gör en vänstersväng med
 *              hastighet speed
 *****************************************************************************/
extern unsigned char forward_left(int speed);

/******************************************************************************
 * Generera databyte med kommandot gör en högerersväng med hastighet speed
 * Inparametrar - int speed
 * Returvärde   - unsigned char Databyte med kommandot gör en högersväng med
 *              hastighet speed
 *****************************************************************************/
extern unsigned char forward_right(int speed);

/******************************************************************************
 * Generera databyte med kommandot rotera vänster med hastighet speed
 * Inparametrar - int speed
 * Returvärde   - unsigned char Databyte med kommandot rotera vänster  med 
 * hastighet speed
 *****************************************************************************/
extern unsigned char rotate_left(int speed);

/******************************************************************************
 * Generera databyte med kommandot rotera höger med hastighet speed
 * Inparametrar - int speed
 * Returvärde   - unsigned char Databyte med kommandot rotera höger  med 
 * hastighet speed
 *****************************************************************************/
extern unsigned char rotate_right(int speed);

/******************************************************************************
 * Generera databyte med kommandot kör bakåt med hastighet speed
 * Inparametrar - int speed
 * Returvärde   - unsigned char Databyte med kommandot kör bakåt med
 * hastighet speed
 *****************************************************************************/
extern unsigned char reverse(int speed);

#endif
