# Target file name (without extension)
TARGET = log_analyzer
# List C source files here (C dependencies are automatically generated)
SRC = $(TARGET).c ks0108.c

# List Assembler source files here. Files ending by .S
ASRC = asmFuncs.S





# AVR board by Sebesta
MCU = atmega16
# Arduino Uno LCD Keypad Shield
#MCU = atmega328p

# Programming support using avrdude. Settings and variables
# AVR board by Sebesta
AVRDUDE_PROGRAMMER = avrispmkII
AVRDUDE_PORT = usb
# Arduino Uno LCD Keypad Shield
#AVRDUDE_PROGRAMMER = arduino
#AVRDUDE_PORT = /dev/ttyUSB0
AVRDUDE_FLAGS = -p $(MCU) -c $(AVRDUDE_PROGRAMMER) -P $(AVRDUDE_PORT)





# Place -D options here
CDEFS  = -DF_CPU=16000000
CDEFS += -D_LCD_DEFINITIONS_FILE

# Place -I options here
CINCS =

# Compiler flags
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -Wall...:     warning level
CFLAGS  = -mmcu=$(MCU)
CFLAGS += $(CDEFS)
CFLAGS += -Os
CFLAGS += -Wall -Wstrict-prototypes

# Assembler flags
#  -Wa,...:      tell GCC to pass this to the assembler
#  -adhlns...:   create assembler listing
#  -gstabs:      have the assembler create line number information
ASFLAGS  = -mmcu=$(MCU)
ASFLAGS += -Wa,-gstabs
ASFLAGS += -Wall -x assembler-with-cpp



# Define programs and commands
CC      = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE    = avr-size
NM      = avr-nm
AVRDUDE = avrdude
REMOVE  = rm -f


# Define messages
MSG_FLASH = Creating load file for Flash:
MSG_EXTENDED_LISTING = Creating extended listing:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:
MSG_PROGRAMMING = Programming load file:


# Define all object files
OBJ = $(SRC:.c=.o) $(ASRC:.S=.o)

# Define all listing files
LST = $(SRC:.c=.lst) $(ASRC:.S=.lst)


# Default target
all: gccversion build

build: elf hex lss

elf: $(TARGET).elf
hex: $(TARGET).hex
lss: $(TARGET).lss


# Display compiler version information
gccversion :
	@$(CC) --version


# Program the device
program: elf hex lss
	@echo
	@echo $(MSG_PROGRAMMING) $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$(TARGET).hex

# Create final output files (.hex) from ELF output file
%.hex: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Create extended listing file from ELF output file
%.lss: %.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S $< > $@

# Link: create ELF output file from object files
%.elf: $(OBJ)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(CFLAGS) $(OBJ) --output $@

# Compile: create object files from C source files
%.o: %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(CFLAGS) $< -o $@

# Assemble: create object files from assembler source files
%.o : %.S
	@echo
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(ASFLAGS) $< -o $@


# Erase the device
erase:
	$(AVRDUDE) $(AVRDUDE_FLAGS) -e


# Clean project
clean:
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).lss
	$(REMOVE) $(TARGET).sym
	$(REMOVE) $(OBJ)
	$(REMOVE) *~


# Inspired by:
# http://people.ece.cornell.edu/land/courses/ece4760/labs/s2011/lab1code/makefile
