
#ARMGNU=arm-none-linux-gnueabi
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : blinker02.sram.bin blinker02.flash.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
	rm -f *.list

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

blinker02.o : blinker02.c
	$(ARMGNU)-gcc $(COPS) -c blinker02.c -o blinker02.o

blinker02.sram.bin : novectors.o blinker02.o    sram.ld
	$(ARMGNU)-ld     novectors.o blinker02.o -T sram.ld -o blinker02.sram.elf
	$(ARMGNU)-objdump -D blinker02.sram.elf > blinker02.sram.list
	$(ARMGNU)-objcopy blinker02.sram.elf blinker02.sram.bin -O binary

blinker02.flash.bin : novectors.o blinker02.o    flash.ld
	$(ARMGNU)-ld      novectors.o blinker02.o -T flash.ld -o blinker02.flash.elf
	$(ARMGNU)-objdump -D blinker02.flash.elf > blinker02.flash.list
	$(ARMGNU)-objcopy blinker02.flash.elf blinker02.flash.bin -O binary



