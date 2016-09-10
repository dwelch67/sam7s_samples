
#ARMGNU=arm-none-linux-gnueabi
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : blinker06.sram.bin blinker06.flash.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
	rm -f *.list
	rm -f pllmath

pllmath : pllmath.c
	gcc -O2 pllmath.c -o pllmath

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

blinker06.o : blinker06.c
	$(ARMGNU)-gcc $(COPS) -c blinker06.c -o blinker06.o

blinker06.sram.bin : novectors.o blinker06.o    sram.ld
	$(ARMGNU)-ld     novectors.o blinker06.o -T sram.ld -o blinker06.sram.elf
	$(ARMGNU)-objdump -D blinker06.sram.elf > blinker06.sram.list
	$(ARMGNU)-objcopy blinker06.sram.elf blinker06.sram.bin -O binary

blinker06.flash.bin : novectors.o blinker06.o    flash.ld
	$(ARMGNU)-ld      novectors.o blinker06.o -T flash.ld -o blinker06.flash.elf
	$(ARMGNU)-objdump -D blinker06.flash.elf > blinker06.flash.list
	$(ARMGNU)-objcopy blinker06.flash.elf blinker06.flash.bin -O binary



