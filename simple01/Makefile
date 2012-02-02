
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

all : simple01.bin

simple01.bin : simple01.s memmap
	$(ARMGNU)-as  simple01.s -o simple01.o
	$(ARMGNU)-ld -o simple01.elf simple01.o -T memmap
	$(ARMGNU)-objdump -D simple01.elf > simple01.list
	$(ARMGNU)-objcopy simple01.elf simple01.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

