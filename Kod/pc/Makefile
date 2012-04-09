CC = gcc
CFLAGS = -Wall -Wextra -W -pedantic -std=c99 -g
LDFLAGS = -lbluetooth -I/usr/include/bluetooth/ -lSDL -I/usr/include/SDL
LIB =  -I/usr/include/bluetooth/ -I/usr/include/SDL

SIMPLE_OBJ = simplescan.o

BLUE_PC_OBJ = blue_pc.o

INPUT_OBJ = input_control.o\
	    keypress.o\
	    header.o\
	    blue_pc.o\
	    db.o

TESTER_OBJ = tester.o\
	     blue_pc.o

DB_TEST_OBJ = db_test.o\
	      db.o

SEND_REC_OBJ = blue_pc.o\
	       db.o\
	       send_receive.o


OBJ = $(SIMPLE_OBJ)\
      $(BLUE_PC_OBJ)\
      $(TESTER_OBJ)\
      $(INPUT_OBJ)\
      $(DB_TEST_OBJ)\
      $(SEND_REC_OBJ)\

EXE = simplescan\
      tester\
      input_control\
      db_test\
      send_receive

all: $(EXE)

%.o : %.c
	$(CC) -c $(CFLAGS) $< $(LDFLAGS) -o $@

simplescan : $(SIMPLE_OBJ)
	$(CC) $(CFLAGS) $(LIB) $^ $(LDFLAGS) -o $@

tester : $(TESTER_OBJ)
	$(CC) $(CFLAGS) $(LIB) $^ $(LDFLAGS) -o $@

input_control: $(INPUT_OBJ)
	$(CC) $(CFLAGS) $(LIB) $^ $(LDFLAGS) -o $@

db_test : $(DB_TEST_OBJ)
	$(CC) $(CFLAGS) $(LIB) $^ $(LDFLAGS) -o $@

send_receive : $(SEND_REC_OBJ)
	$(CC) $(CFLAGS) $(LIB) $^ $(LDFLAGS) -o $@

clean :
	rm *.o $(EXE)

