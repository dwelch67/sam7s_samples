
#ARMGNU=arm-none-linux-gnueabi
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : blinker03.sram.bin blinker03.flash.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
	rm -f *.list

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

blinker03.o : blinker03.c
	$(ARMGNU)-gcc $(COPS) -c blinker03.c -o blinker03.o

blinker03.sram.bin : novectors.o blinker03.o    sram.ld
	$(ARMGNU)-ld     novectors.o blinker03.o -T sram.ld -o blinker03.sram.elf
	$(ARMGNU)-objdump -D blinker03.sram.elf > blinker03.sram.list
	$(ARMGNU)-objcopy blinker03.sram.elf blinker03.sram.bin -O binary

blinker03.flash.bin : novectors.o blinker03.o    flash.ld
	$(ARMGNU)-ld      novectors.o blinker03.o -T flash.ld -o blinker03.flash.elf
	$(ARMGNU)-objdump -D blinker03.flash.elf > blinker03.flash.list
	$(ARMGNU)-objcopy blinker03.flash.elf blinker03.flash.bin -O binary



