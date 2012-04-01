CC = gcc
CFLAGS = -Wall -Wextra -W -pedantic -std=c99 -g
LDFLAGS = -lbluetooth
LIB =  -I/usr/include/bluetooth/

SIMPLE_OBJ = simplescan.o

BLUE_PC_OBJ = blue_pc.o

TESTER_OBJ = tester.o\
	     blue_pc.o


OBJ = $(SIMPLE_OBJ)\
      $(BLUE_PC_OBJ)\
      $(TESTER_OBJ)\

EXE = simplescan\
      tester

all: $(EXE)

%.o : %.c
	$(CC) -c $(CFLAGS) $< $(LDFLAGS) -o $@

simplescan : $(SIMPLE_OBJ)
	$(CC) $(CFLAGS) $(LIB) $^ $(LDFLAGS) -o $@

tester : $(TESTER_OBJ)
	$(CC) $(CFLAGS) $(LIB) $^ $(LDFLAGS) -o $@

clean :
	rm *.o $(EXE)

