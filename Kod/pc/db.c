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
	return fopen(path, "rw");
}

void add_to_db(FILE *db, void *data, int n)
{
	fwrite(data, 1, n, db);

	fflush(db);

	return;

}

void read_from_db(FILE *db, void *data, int n)
{

	fseek(db, -(n + 1), SEEK_END);

	if(n == 0){
		data = NULL;
		return;
	}

	unsigned char c[n + 1];
	fread(data, 1, n, db);

//	data = (void *) c;

	return;

}
