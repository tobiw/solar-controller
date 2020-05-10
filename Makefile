AVR=avrdude
MCU=m328p
ARGS=-C ./avrdude.conf -P usb -cusbasp -p $(MCU) -v

CLOCK=16000000
DEVICE=atmega328p
COMPILE=avr-gcc -Wall -I./include -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

fuse:
	$(AVR) $(ARGS) -u -U lfuse:w:0xdf:m -U hfuse:w:0xde:m -U efuse:w:0xff:m

program:
	$(COMPILE) -c src/hw/atmega328p.c -o atmega328p.o
	$(COMPILE) -c src/core/program.c -o program.o
	$(COMPILE) -c src/main.c -o main.o
	$(COMPILE) -o main.elf main.o program.o atmega328p.o
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size main.hex

install: main.hex
	$(AVR) $(ARGS) -u -U flash:w:main.hex:i

clean:
	rm -f *.elf main.hex *.o

read:
	$(AVR) $(ARGS) -v
