#ifndef DB_H
#define DB_H
/******************************************************************************
 * Gränssnitt för att hantera en enkel databas
 * 
 * Skapad:      06/04/2012
 * Av:          Johan Jönsson
 *
 * Redigerad:   08/05/2012
 *****************************************************************************/
#include<stdio.h>
#include<stdlib.h>

/******************************************************************************
 * Initialiser databasen, enbart läs
 * path är sökvägen till databasen
 * Inparametrar - char *path sökväg till databasen
 ******************************************************************************/
FILE *init_read(char *path);

/******************************************************************************
 * Initialisera databasen, enbart skriv
 * path är sökvägen till databasen
 * Inparametrar - char *path sökväg till databasen 
 ******************************************************************************/
FILE *init_write(char *path);

/****************************************************************************** 
 * Initialisera databasen, läs och skriv
 * path är sökvägen till databasen
 *****************************************************************************/
FILE *init_rw(char *path);

/******************************************************************************
 * Skriver n bytes av data till databasen db
 * Kräver skrivläge, exempelvis init_write
 * Inparametrar - FILE *db databasen
 *              - void *data lista med datan att skriva in
 *              - int n antalet byte av data som ska skrivas in i db
 * Returvärde   -
 ******************************************************************************/
void add_to_db(FILE *db, void *data, int n);

/******************************************************************************
 * Läs n bytes från db till data
 * Kräver läsläge, exempelvis init_read
 * Inparametrar - FILE *db databasen
 *              - void *data data att skriva till
 *              - int n antalet byte att läsa in i data från db
 *Returvärde    -
 ******************************************************************************/
void read_from_db(FILE *db, void *data, int n);

#endif
// DB_H
