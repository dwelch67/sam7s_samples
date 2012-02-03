
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : uart02.bin toarm.elf

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

uart02.o : uart02.c
	$(ARMGNU)-gcc $(COPS) -c uart02.c -o uart02.o

uart02.bin : memmap novectors.o uart02.o 
	$(ARMGNU)-ld novectors.o uart02.o -T memmap -o uart02.elf
	$(ARMGNU)-objdump -D uart02.elf > uart02.list
	$(ARMGNU)-objcopy uart02.elf -O binary uart02.bin 






