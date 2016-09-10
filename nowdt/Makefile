
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

all : nowdt.bin

nowdt.bin : nowdt.s memmap
	$(ARMGNU)-as  nowdt.s -o nowdt.o
	$(ARMGNU)-ld -o nowdt.elf nowdt.o -T memmap
	$(ARMGNU)-objdump -D nowdt.elf > nowdt.list
	$(ARMGNU)-objcopy nowdt.elf nowdt.hex -O ihex
	$(ARMGNU)-objcopy nowdt.elf nowdt.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

