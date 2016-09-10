
#ARMGNU=arm-none-linux-gnueabi
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : blinker05.sram.bin blinker05.flash.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
	rm -f *.list
	rm -f pllmath

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

blinker05.o : blinker05.c
	$(ARMGNU)-gcc $(COPS) -c blinker05.c -o blinker05.o

blinker05.sram.bin : novectors.o blinker05.o    sram.ld
	$(ARMGNU)-ld     novectors.o blinker05.o -T sram.ld -o blinker05.sram.elf
	$(ARMGNU)-objdump -D blinker05.sram.elf > blinker05.sram.list
	$(ARMGNU)-objcopy blinker05.sram.elf blinker05.sram.bin -O binary

blinker05.flash.bin : novectors.o blinker05.o    flash.ld
	$(ARMGNU)-ld      novectors.o blinker05.o -T flash.ld -o blinker05.flash.elf
	$(ARMGNU)-objdump -D blinker05.flash.elf > blinker05.flash.list
	$(ARMGNU)-objcopy blinker05.flash.elf blinker05.flash.bin -O binary



pllmath : pllmath.c
	gcc -O2 pllmath.c -o pllmath

