
#ARMGNU=arm-none-linux-gnueabi
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : uart02.sram.bin uart02.flash.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
	rm -f *.list

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

uart02.o : uart02.c
	$(ARMGNU)-gcc $(COPS) -c uart02.c -o uart02.o

uart02.sram.bin : novectors.o uart02.o    sram.ld
	$(ARMGNU)-ld  novectors.o uart02.o -T sram.ld -o uart02.sram.elf
	$(ARMGNU)-objdump -D uart02.sram.elf > uart02.sram.list
	$(ARMGNU)-objcopy uart02.sram.elf uart02.sram.bin -O binary

uart02.flash.bin : novectors.o uart02.o    flash.ld
	$(ARMGNU)-ld   novectors.o uart02.o -T flash.ld -o uart02.flash.elf
	$(ARMGNU)-objdump -D uart02.flash.elf > uart02.flash.list
	$(ARMGNU)-objcopy uart02.flash.elf uart02.flash.bin -O binary



