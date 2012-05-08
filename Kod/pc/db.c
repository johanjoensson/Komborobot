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
#include<stdint.h>
#include<unistd.h>

FILE *init_read(char *path)
{
	return fopen(path, "r");
}

FILE *init_write(char *path)
{
	return fopen(path, "w");
}

FILE *init_rw(char *path)
{
	return fopen(path, "w+");
}

void add_to_db(FILE *db, void *data, int n)
{
	fseek(db, -n, SEEK_END);

	fwrite(data, 1, n, db);

	fflush(db);

	return;

}

void read_from_db(FILE *db, void *data, int n)
{

	fseek(db, -n, SEEK_END);
	if(n == 0){
		data = NULL;
		return;
	}

	fread(data, 1, n, db);
	fflush(db);

	return;

}
