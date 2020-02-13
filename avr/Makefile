# tool locations
# get the correct location of avr-gcc on OS X
AVR_INSTALL=$(shell dirname $(shell which avr-gcc))

# get the correction location of the include files for gcc-avr
AVR_INCL_DIR=$(shell dirname $(shell locate unistd.h | grep avr-gcc))

# executables
CC=$(AVR_INSTALL)/avr-gcc
CXX=$(AVR_INSTALL)//avr-g++
OBJCOPY=$(AVR_INSTALL)/avr-objcopy
OBJDUMP=$(AVR_INSTALL)/avr-objdump
AVRSIZE=$(AVR_INSTALL)/avr-size
SREC=$(AVR_INSTALL)/srec_cat

# for loading the redboard
DUDE=/usr/local/homebrew/bin/avrdude
BOARD=$(shell ls -1 /dev/tty.SLAB*)

OBJDIR=./obj/
OUTDIR=./bin/
TARGET=$(OUTDIR)metronome

CFLAGS=-Wall -mmcu=atmega328p -DF_CPU=16000000UL -DLITTLE_ENDIAN -Os -I$(AVR_INCL_DIR)
CFLAGS+=-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections
CFLAGS+=-DDEBUG

LDFLAGS=-mmcu=atmega328p -Wl,-u,--gc-sections,-Map=$(TARGET).map,--cref
LDLIBS=

CSOURCES=$(shell find . -type f -name '*.c')
CPPSOURCES=$(shell find . -type f -name '*.cpp')
HEADERS=$(shell find . -type f -name '*.h')

CPPOBJECTS=$(addprefix $(OBJDIR),$(CPPSOURCES:.cpp=.o))
COBJECTS=$(addprefix $(OBJDIR),$(CSOURCES:.c=.o))

$(OBJDIR)%.o: %.c $(HEADERS) Makefile
	@mkdir -p $(@D)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) $(INCDIRS) -c -o $@ $<;

$(OBJDIR)%.o: %.cpp $(HEADERS) Makefile
	@mkdir -p $(@D)
	@echo "Compiling $<"
	@$(CXX) $(CFLAGS) $(INCDIRS) -c -o $@ $<;

$(TARGET).elf: $(COBJECTS) $(CPPOBJECTS)
	@mkdir -p $(@D)
	@echo ""
	@echo "Linking $@"
	@echo ""
	@$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

	@echo "Finished size:"
	@$(AVRSIZE) $(TARGET).elf
	
	@echo ""
	@echo "Dumping list and building hex files..."
	@$(OBJDUMP) -h -S $(TARGET).elf > $(TARGET).lss
	@$(OBJCOPY) -R .eepromp -R .fuse -R .lock -R .signature -O ihex $(TARGET).elf $(TARGET).hex
	@$(OBJCOPY) --no-change-warnings -j .eeprom --change-section-lma .eeprom=0 -O ihex $(TARGET).elf $(TARGET).eep
	@$(OBJCOPY) --no-change-warnings -j .lock --change-section-lma .lock=0 -O ihex $(TARGET).elf $(TARGET).lock
	@$(OBJCOPY) --no-change-warnings -j .signature --change-section-lma .signature=0 -O ihex $(TARGET).elf $(TARGET).sig
	@$(OBJCOPY) --no-change-warnings -j .fuse --change-section-lma .fuse=0 -O ihex $(TARGET).elf $(TARGET).fuse
	@$(SREC) $(TARGET).fuse -Intel -crop 0x00 0x01 -offset  0x00 -O $(TARGET).lfs -Intel
	@$(SREC) $(TARGET).fuse -Intel -crop 0x01 0x02 -offset -0x01 -O $(TARGET).hfs -Intel
	@$(SREC) $(TARGET).fuse -Intel -crop 0x02 0x03 -offset -0x02 -O $(TARGET).efs -Intel

	@echo ""
	@echo "DONE!!"

.PHONY: all clean load rebuild

rebuild: clean $(TARGET).hex

all: $(TARGET).hex

clean:
	rm -rf $(OUTDIR) $(OBJDIR)

load:
	@echo target device is $(BOARD), writing flash
	$(DUDE) -F -V -c arduino -p m328p -P $(BOARD) -b 115200 -U flash:w:$(TARGET).hex

wfuses:
	#echo targed device is $(BOARD), writing fuses
	$(DUDE) -F -V -u -c arduino -p m328p -P $(BOARD) -b 115200 -U lfuse:w:$(TARGET).lfs
	$(DUDE) -F -V -u -c arduino -p m328p -P $(BOARD) -b 115200 -U hfuse:w:$(TARGET).hfs
	$(DUDE) -F -V -u -c arduino -p m328p -P $(BOARD) -b 115200 -U efuse:w:$(TARGET).efs

