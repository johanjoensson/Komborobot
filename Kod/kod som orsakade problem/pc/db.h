#ifndef DB_H
#define DB_H

#include<stdio.h>
#include<stdlib.h>

/*
 * Initiate the database
 * Read only
 * Path is the searchpath to the file to be opened
 */
FILE *init_read(char *path);

/*
 * Initiate the database
 * Write only
 * Path is the searchpath to the file to be opened
 */
FILE *init_write(char *path);

/* 
 * Initiate the database
 * Read, Write
 * Path is the searchpath to the file to be opened
 */
FILE *init_rw(char *path);

/*
 * Write n bytes starting at data to the database db
 * Requires write acces
 */
void add_to_db(FILE *db, void *data, int n);

/*
 * Read n bytes from database, store at location data
 * Requires read access
 */
void read_from_db(FILE *db, void *data, int n);

#endif
// DB_H
