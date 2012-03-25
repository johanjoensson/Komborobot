CC = gcc
CFLAGS = -Wall -Wextra -W -pedantic -std=c99 -g
LIB = -lbluetooth -I/usr/include/bluetooth/

SIMPLE_OBJ = simplescan.o

BLUE_PC_OBJ = blue_pc.o


OBJ = $(SIMPLE_OBJ)\
      $(BLUE_PC_OBJ)\

EXE = simplescan\
      blue_pc

all: $(EXE)

%.o : %.c
	$(CC) -c $(CFLAGS) $< $(LDFLAGS) -o $@

simplescan : $(SIMPLE_OBJ)
	$(CC) $(CFLAGS) $(LIB) $^ $(LDFLAGS) -o $@

blue_pc : $(BLUE_PC_OBJ)
	$(CC) $(CFLAGS) $(LIB) $^ $(LDFLAGS) -o $@

clean :
	rm *.o $(EXE)

