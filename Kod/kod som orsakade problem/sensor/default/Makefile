###############################################################################
# Makefile for the project Sensor
###############################################################################

## General Flags
PROJECT = Sensor
MCU = atmega16
TARGET = Sensor.elf
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
LDFLAGS +=  -Wl,-Map=Sensor.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = upptack_tejp.o avstandsskillnad.o linjeskillnad.o sensor.o sensor_ad.o sensor_init.o sensor_spi.o sensorvarde_omvandling.o special.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) Sensor.hex Sensor.eep Sensor.lss size

## Compile
upptack_tejp.o: ../upptack_tejp.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

avstandsskillnad.o: ../avstandsskillnad.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

linjeskillnad.o: ../linjeskillnad.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sensor.o: ../sensor.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sensor_ad.o: ../sensor_ad.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sensor_init.o: ../sensor_init.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sensor_spi.o: ../sensor_spi.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sensorvarde_omvandling.o: ../sensorvarde_omvandling.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

special.o: ../special.c
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
	-rm -rf $(OBJECTS) Sensor.elf dep/* Sensor.hex Sensor.eep Sensor.lss Sensor.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

