
#ARMGNU=arm-none-linux-gnueabi
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : uart01.sram.bin uart01.flash.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
	rm -f *.list

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

uart01.o : uart01.c
	$(ARMGNU)-gcc $(COPS) -c uart01.c -o uart01.o

uart01.sram.bin : novectors.o uart01.o    sram.ld
	$(ARMGNU)-ld  novectors.o uart01.o -T sram.ld -o uart01.sram.elf
	$(ARMGNU)-objdump -D uart01.sram.elf > uart01.sram.list
	$(ARMGNU)-objcopy uart01.sram.elf uart01.sram.bin -O binary

uart01.flash.bin : novectors.o uart01.o    flash.ld
	$(ARMGNU)-ld   novectors.o uart01.o -T flash.ld -o uart01.flash.elf
	$(ARMGNU)-objdump -D uart01.flash.elf > uart01.flash.list
	$(ARMGNU)-objcopy uart01.flash.elf uart01.flash.bin -O binary



