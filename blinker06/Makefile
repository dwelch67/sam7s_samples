
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : blinker06.bin toarm.elf

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

blinker06.o : blinker06.c
	$(ARMGNU)-gcc $(COPS) -c blinker06.c -o blinker06.o

blinker06.bin : memmap novectors.o blinker06.o 
	$(ARMGNU)-ld novectors.o blinker06.o -T memmap -o blinker06.elf
	$(ARMGNU)-objdump -D blinker06.elf > blinker06.list
	$(ARMGNU)-objcopy blinker06.elf -O binary blinker06.bin 






