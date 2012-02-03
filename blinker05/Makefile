
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : blinker05.bin toarm.elf

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

blinker05.o : blinker05.c
	$(ARMGNU)-gcc $(COPS) -c blinker05.c -o blinker05.o

blinker05.bin : memmap novectors.o blinker05.o 
	$(ARMGNU)-ld novectors.o blinker05.o -T memmap -o blinker05.elf
	$(ARMGNU)-objdump -D blinker05.elf > blinker05.list
	$(ARMGNU)-objcopy blinker05.elf -O binary blinker05.bin 






