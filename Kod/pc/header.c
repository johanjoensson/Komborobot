/*
 * =====================================================================================
 *
 *       Filename:  header.c
 *
 *    Description:  Headerfunktioner
 *
 *        Version:  1.0
 *        Created:  2012-03-25 17:14:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Gustav (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <stdio.h>


unsigned char header_komm(int datakod);
unsigned char header_sens(int datakod);
unsigned char header_styr(int datakod);
unsigned char header_pc(int datakod);
unsigned char header_pc_styr(int datakod);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  generate_header
 *  Description:  Generar en header baserat på indatat
 *                Input: målenhetetskod och datakod
 *                Målenhetskoder:
 *                      0     Kommunkationsenhet
 *                      1     Sensorenhet
 *                      2     Styrenhet
 *                      3     PC
 *                      4     PC och Styrenhet
 * =====================================================================================
 */
unsigned char generate_header(int destination, int datakod)
{
        unsigned char header;
        switch(destination){
                case 0:
                        header = header_komm(datakod);
                        break;
                case 1:
                        header = header_sens(datakod);
                        break;
                case 2:
                        header = header_styr(datakod);
                        break;
                case 3:
                        header = header_pc(datakod);
                        break;
                default:
                        header = header_pc_styr(datakod);
                        break;
        }
        return header;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  header_komm
 *  Description:  Genererar headerfil för kommunikationsenhet
 *                Input: datakod
 *                Output: header
 * =====================================================================================
 */
unsigned char header_komm(int datakod)
{
        return 0b00000000;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  header_sens
 *  Description:  Genererar headerfil för sensorenhet
 *                Input: datakod
 *                Output: header
 * =====================================================================================
 */
unsigned char header_sens(int datakod)
{

        return 0b00100000 + datakod;;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  header_styr
 *  Description:  Genererar headerfil för styrenhet
 *                Input: datakod
 *                Output: header
 *                      Datakoder:
 *                      0       autonomt läge, vanligt sensordata
 *                      1       autonomt läge, specialkommando
 *                      2       fjärrstyrt läge
 * =====================================================================================
 */
unsigned char header_styr(int datakod)
{
        if(datakod == 0){ //autonomt läge, vanligt sensordata
                return 0b01000001;
        } else if(datakod == 1) { // autonomt läge, specialkommando
                return 0b01000011;
        } else if(datakod == 2){ // fjärrstyrt läge
                return 0b01000000;
        } else { // felaktig datakod, anta fjärrstyrt läge
                return 0b01000000;
        }
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  header_pc
 *  Description:  Genererar headerfil för pc
 *                Input: datakod
 *                Output: header
 *                      Datakoder:
 *                      0       sensorinfo från sensorenhet
 *                      1       styrinfo från styrenhet 
 *                      2       specialkommando från sensorenhet
 * =====================================================================================
 */
unsigned char header_pc(int datakod)
{
        if(datakod == 0){ // Sensorinfo
                return 0b10000000;
        } else if(datakod == 1){ // Styrinfo
                return 0b10000100; 
        } else if(datakod == 2){ // Specialinfo
                return 0b10001000; 
        } else { // Annat (fel)
                return 0b1000000; 
        }
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  header_pc_styr
 *  Description:  Genererar headerfil för styrenhet och pc
 *                Input: datakod
 *                Output: header
 *                      Datakoder:
 *                      0       Autonomt läge, vanligt sensorinfo
 *                      1       Autonomt läge, specialkommando
 * =====================================================================================
 */
unsigned char header_pc_styr(int datakod)
{
        if(datakod == 0){ // sensorinfo
                return 0b11000001;
        } else if(datakod == 1){ // specialkommando
                return 0b11001011;
        } else { // Annars (fel)
                return 0b11000001;
        }
}
