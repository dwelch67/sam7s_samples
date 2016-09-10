
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

all : blinker01.sram.bin blinker01.flash.bin

blinker01.o : blinker01.s
	$(ARMGNU)-as  blinker01.s -o blinker01.o

blinker01.sram.bin : blinker01.o    sram.ld
	$(ARMGNU)-ld     blinker01.o -T sram.ld -o blinker01.sram.elf
	$(ARMGNU)-objdump -D blinker01.sram.elf > blinker01.sram.list
	$(ARMGNU)-objcopy blinker01.sram.elf blinker01.sram.bin -O binary

blinker01.flash.bin : blinker01.o    flash.ld
	$(ARMGNU)-ld      blinker01.o -T flash.ld -o blinker01.flash.elf
	$(ARMGNU)-objdump -D blinker01.flash.elf > blinker01.flash.list
	$(ARMGNU)-objcopy blinker01.flash.elf blinker01.flash.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

