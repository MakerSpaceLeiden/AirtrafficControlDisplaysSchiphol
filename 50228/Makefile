PRG            = dsip
SRC            = display.c ../shared/pd44.c schiphol50228.c
OBJS           = display.o pd44.o schiphol50228.o

MCU_TARGET     = atmega161
OPTIMIZE       = -O3

AVRDUDE        = /Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avrdude
AVRDUDECONF    = /Applications/Arduino.app/Contents/Java/hardware/tools/avr/etc/avrdude.conf

DEFS           =
LIBS           =

CC             = avr-gcc

override CFLAGS        = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS) -D F_CPU=8000000 -I../shared -I.
override LDFLAGS       = -Wl,-Map,$(PRG).map 

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

all: $(PRG).elf lst text eeprom

download: flash

flash: $(PRG).hex
	# uisp -dprog avrisp -dpart ATmega161 $<
	# $(AVRDUDE) -C $(AVRDUDECONF) -c avrisp2 -p m8 -v -v -U flash:w:$<:i
	$(AVRDUDE) -C $(AVRDUDECONF) -c avrispmkII -p m161 -U flash:w:$<:i

pd44.o:  ../shared/pd44.c
	${CC} ${CFLAGS} -c $<

.c.o:
	${CC} ${CFLAGS} -c $<

$(PRG).elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

# dependency:
$(PRG).o: $(SRC) *.h

clean:
	rm -rf *.o $(PRG).elf  *.bak *.s
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)

lst:  $(PRG).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

# Rules for building the .text rom images

text: hex bin srec

hex:  $(PRG).hex
bin:  $(PRG).bin
srec: $(PRG).srec

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

# Rules for building the .eeprom rom images

eeprom: ehex ebin esrec

ehex:  $(PRG)_eeprom.hex
ebin:  $(PRG)_eeprom.bin
esrec: $(PRG)_eeprom.srec

%_eeprom.hex: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@ \
	|| { echo empty $@ not generated; exit 0; }

%_eeprom.srec: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O srec $< $@ \
	|| { echo empty $@ not generated; exit 0; }

%_eeprom.bin: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O binary $< $@ \
	|| { echo empty $@ not generated; exit 0; }

