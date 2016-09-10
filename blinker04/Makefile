
#ARMGNU=arm-none-linux-gnueabi
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : blinker04.sram.bin blinker04.flash.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
	rm -f *.list

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

blinker04.o : blinker04.c
	$(ARMGNU)-gcc $(COPS) -c blinker04.c -o blinker04.o

blinker04.sram.bin : novectors.o blinker04.o    sram.ld
	$(ARMGNU)-ld     novectors.o blinker04.o -T sram.ld -o blinker04.sram.elf
	$(ARMGNU)-objdump -D blinker04.sram.elf > blinker04.sram.list
	$(ARMGNU)-objcopy blinker04.sram.elf blinker04.sram.bin -O binary

blinker04.flash.bin : novectors.o blinker04.o    flash.ld
	$(ARMGNU)-ld      novectors.o blinker04.o -T flash.ld -o blinker04.flash.elf
	$(ARMGNU)-objdump -D blinker04.flash.elf > blinker04.flash.list
	$(ARMGNU)-objcopy blinker04.flash.elf blinker04.flash.bin -O binary



