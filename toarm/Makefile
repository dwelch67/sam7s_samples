
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : blinker02.bin toarm.elf

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

blinker02.o : blinker02.c
	$(ARMGNU)-gcc $(COPS) -c blinker02.c -o blinker02.o

blinker02.bin : memmap novectors.o blinker02.o 
	$(ARMGNU)-ld novectors.o blinker02.o -T memmap -o blinker02.elf
	$(ARMGNU)-objdump -D blinker02.elf > blinker02.list
	$(ARMGNU)-objcopy blinker02.elf -O binary blinker02.bin 






