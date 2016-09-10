
#ARMGNU=arm-none-linux-gnueabi
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : uart03.sram.bin uart03.flash.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
	rm -f *.list

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

uart03.o : uart03.c
	$(ARMGNU)-gcc $(COPS) -c uart03.c -o uart03.o

uart03.sram.bin : novectors.o uart03.o    sram.ld
	$(ARMGNU)-ld  novectors.o uart03.o -T sram.ld -o uart03.sram.elf
	$(ARMGNU)-objdump -D uart03.sram.elf > uart03.sram.list
	$(ARMGNU)-objcopy uart03.sram.elf uart03.sram.bin -O binary

uart03.flash.bin : novectors.o uart03.o    flash.ld
	$(ARMGNU)-ld   novectors.o uart03.o -T flash.ld -o uart03.flash.elf
	$(ARMGNU)-objdump -D uart03.flash.elf > uart03.flash.list
	$(ARMGNU)-objcopy uart03.flash.elf uart03.flash.bin -O binary



