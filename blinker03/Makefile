
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : blinker03.bin toarm.elf

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
	rm -f *.list

toarm.elf : toarm.s memmap
	$(ARMGNU)-as toarm.s -o toarm.o
	$(ARMGNU)-ld -T memmap toarm.o -o toarm.elf
	$(ARMGNU)-objdump -D toarm.elf > toarm.list

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

blinker03.o : blinker03.c
	$(ARMGNU)-gcc $(COPS) -c blinker03.c -o blinker03.o

blinker03.bin : memmap novectors.o blinker03.o 
	$(ARMGNU)-ld novectors.o blinker03.o -T memmap -o blinker03.elf
	$(ARMGNU)-objdump -D blinker03.elf > blinker03.list
	$(ARMGNU)-objcopy blinker03.elf -O binary blinker03.bin 






