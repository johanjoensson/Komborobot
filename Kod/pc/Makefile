CC = gcc
CFLAGS = -Wall -Wextra -W -pedantic -std=c99 -g
LIB = -lbluetooth -I/usr/include/bluetooth/ \
      -lSDL -I/usr/include/SDL

SIMPLE_OBJ = simplescan.o

BLUE_PC_OBJ = blue_pc.o

INPUT_OBJ = input_control.o\
	    header.o\
	    keypress.o

OBJ = $(SIMPLE_OBJ)\
      $(BLUE_PC_OBJ)\
      $(INPUT_OBJ)

EXE = simplescan\
      blue_pc\
      input_obj

all: $(EXE)

%.o : %.c
	$(CC) -c $(CFLAGS) $< $(LDFLAGS) -o $@

simplescan : $(SIMPLE_OBJ)
	$(CC) $(CFLAGS) $(LIB) $^ $(LDFLAGS) -o $@

blue_pc : $(BLUE_PC_OBJ)
	$(CC) $(CFLAGS) $(LIB) $^ $(LDFLAGS) -o $@

input_control: $(INPUT_OBJ)
	$(CC) $(CFLAGS) $(LIB) $^ $(LDFLAGS) -o $@

clean :
	rm *.o $(EXE)
