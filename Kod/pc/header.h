#ifndef HEADER_H
#define HEADER_H

/******************************************************************************
 * Generera lämplig header till instruktion att skicka till Komborobot
 * Inparametrar - int destination Målenhet på Komborobot
 *              0 Kommunikationsenhet
 *              1 Sensorenhet
 *              2 Styrenhet
 *              3 PC
 *              4 PC och Styrenhet
 *
 * Returvärde   - unsigned char Headern som genererats
 *****************************************************************************/
extern unsigned char generate_header(int destination, int datakod);

#endif
