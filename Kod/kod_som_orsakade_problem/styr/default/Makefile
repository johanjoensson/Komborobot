###############################################################################
# Makefile for the project Styr
###############################################################################

## General Flags
PROJECT = Styr
MCU = atmega16
TARGET = Styr.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=Styr.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = regulator.o styr_main.o styr_specialkommando.o styr_SPI.o styr_tolka_data.o motor_styrning.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) Styr.hex Styr.eep Styr.lss size

## Compile
regulator.o: ../regulator.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

styr_main.o: ../styr_main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

styr_specialkommando.o: ../styr_specialkommando.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

styr_SPI.o: ../styr_SPI.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

styr_tolka_data.o: ../styr_tolka_data.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

motor_styrning.o: ../motor_styrning.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) Styr.elf dep/* Styr.hex Styr.eep Styr.lss Styr.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

