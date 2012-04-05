#include<stdio.h>
#include<stdlib.h>

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
	unsigned char *c = (unsigned char *) data;
	for(int i = 0; i < n; i++){
		fprintf(db, "%c", c[i]);
	}

	return;

}

void read_from_db(FILE *db, void *data, int n)
{

	fseek(db, n, SEEK_END);

	if(n == 0){
		data = NULL;
		return;
	}

	unsigned char c[n];
	for(int i = 0; i < n; i++){
		fscanf(db, "%c", &c[i]);
	}

	data = (void *) c;

	return;

}
