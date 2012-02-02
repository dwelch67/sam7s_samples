
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : blinker04.bin toarm.elf

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

blinker04.o : blinker04.c
	$(ARMGNU)-gcc $(COPS) -c blinker04.c -o blinker04.o

blinker04.bin : memmap novectors.o blinker04.o 
	$(ARMGNU)-ld novectors.o blinker04.o -T memmap -o blinker04.elf
	$(ARMGNU)-objdump -D blinker04.elf > blinker04.list
	$(ARMGNU)-objcopy blinker04.elf -O binary blinker04.bin 






