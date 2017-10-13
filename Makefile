###############################################################################
#
# Simple Makefile for Arduino MEGA 2560 C projects
#
###############################################################################

# Programs names and locations
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
CODE_FORMATTER = tooling/format-code.sh
AVRSIZE = avr-size

# Microcontroller type and CPU frequency
MCU = atmega2560
MCU_CPU_FREQ = 16000000UL

# Firmware version from git describe
GIT_DESCR=\"$(shell git describe --abbrev=8 --dirty --always --tags --long)\"

# Built directory and files
BINDIR = bin
TARGET = $(BINDIR)/$(MCU)-user-code.ihx
ELF = $(BINDIR)/$(MCU)-user-code.elf

# Blank firmware which can be used to "erase" user code
BLANK_FW = tooling/atemega2560-blank-user-code/bin/atmega2560-user-code-blank.ihx

# Arduino device file defaults to /dev/ttyACM0
# Use shell command export to define alternative device file
# Example: export ARDUINO=/dev/ttyACM0
ARDUINO ?= /dev/ttyACM0

# Source files. wildcard "uses" all .c files in src and lib directory
SRCDIR = src
BUILD_LIBS_DIR = lib
SRC = $(wildcard $(SRCDIR)/*.c $(BUILD_LIBS_DIR)/*/*.c)

# Define object files from .c files defined above
OBJ = $(SRC:.c=.o)

# Compiler flags
# Note that those beginning with -D are actually pre-processor definitions.
# -Wall ... -Wfatal-errors almost all possible warning options.
# -Os Optimize code. The special option -Os is meant to turn on all -O2.
# optimisations that are not expected to increase code size.
# -std=c11 use C11 standard.
CFLAGS =	-Wall \
			-Wextra \
			-Wpedantic \
			-Wformat \
			-pedantic-errors \
			-Werror \
			-Wfatal-errors \
			-Os \
			-flto \
			-fdata-sections \
			-ffunction-sections \
			-ffreestanding \
			-std=c11 \
			-mmcu=$(MCU) \
			-DF_CPU=$(MCU_CPU_FREQ) \
			-DFW_VERSION=$(GIT_DESCR)


# Linker flags
LDFLAGS =	-mmcu=$(MCU) \
			-flto \
			-Wl,-gc-sections

# Object copy arguments
# Do not copy EEPROM section content
OBJCOPYARGS =	-O ihex \
				-R .eeprom

# FIXME Find out why some Arduinos require -D to write user code.
# Do not check device signature (-F)
AVRDUDEARGS =	-p $(MCU) \
				-c wiring \
				-F \
				-P $(ARDUINO) \
				-b 115200 \
				-D

AVRSIZEARGS =	-C \
				--mcu=$(MCU)

# Default target. Build firmware
all: $(ELF) $(TARGET)

# Build object files from source
%.o : %.c
	$(CC) -c $(CFLAGS) -o $*.o $<

# Link object files to elf file
$(ELF): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

# Copy elf to ihx format
$(TARGET):
	$(OBJCOPY) $(OBJCOPYARGS) $(ELF) $(TARGET)

# Delete all built object files and built firmware files
# Do not remove .placeholder in BINDIR
clean:
	find $(BINDIR) -type f -not -name '.placeholder' -print0 | xargs -0 rm -f --
	rm -f $(SRCDIR)/*.o
	rm -fr $(BUILD_LIBS_DIR)/*/*.o

# Delete also code formatter .orig files
dist-clean: clean
	rm -f $(SRCDIR)/*.orig

# Load built firmware to Arduino
install:
	$(AVRDUDE) $(AVRDUDEARGS) -U flash:w:$(TARGET)

# "Erase" user code part by loading blank firmware
erase:
	$(AVRDUDE) $(AVRDUDEARGS) -U flash:w:$(BLANK_FW)

# Format code using code formatter script
format:
	$(CODE_FORMATTER) $(SRCDIR)/*.c

# Print user code size
size:
	$(AVRSIZE) $(AVRSIZEARGS) $(ELF)

.PHONY: all clean dist-clean install erase format size

