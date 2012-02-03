 
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

LOPS = -Wall -m32 -emit-llvm

LLCOPS = -march=arm

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts

stuff = -O2 -std-compile-opts -strip-debug



all : toarm.elf xmega01.gcc.bin 
llvm : xmega01.clang.norm.bin xmega01.clang.opt.bin


toarm.elf : toarm.s memmap
	$(ARMGNU)-as toarm.s -o toarm.o
	$(ARMGNU)-ld -T memmap toarm.o -o toarm.elf
	$(ARMGNU)-objdump -D toarm.elf > toarm.list



vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

xmega01.gcc.o : xmega01.c blinker.h
	$(ARMGNU)-gcc $(COPS) -c xmega01.c -o xmega01.gcc.o

xmega01.gcc.bin : memmap vectors.o xmega01.gcc.o
	$(ARMGNU)-ld -o xmega01.gcc.elf -T memmap vectors.o xmega01.gcc.o
	$(ARMGNU)-objdump -D xmega01.gcc.elf > xmega01.gcc.list
	$(ARMGNU)-objcopy xmega01.gcc.elf xmega01.gcc.bin -O binary


xmega01.clang.bc : xmega01.c blinker.h
	clang $(LOPS) -c xmega01.c -o xmega01.clang.bc

xmega01.clang.norm.bin : memmap vectors.o xmega01.clang.bc
	llc $(LLCOPS) xmega01.clang.bc -o xmega01.clang.norm.s
	$(ARMGNU)-as xmega01.clang.norm.s -o xmega01.clang.norm.o
	$(ARMGNU)-ld -o xmega01.clang.norm.elf -T memmap vectors.o xmega01.clang.norm.o
	$(ARMGNU)-objdump -D xmega01.clang.norm.elf > xmega01.clang.norm.list
	$(ARMGNU)-objcopy xmega01.clang.norm.elf xmega01.clang.norm.bin -O binary

xmega01.clang.opt.bin : memmap vectors.o xmega01.clang.bc
	opt $(OOPS) xmega01.clang.bc -o xmega01.clang.opt.bc
	llc $(LLCOPS) xmega01.clang.opt.bc -o xmega01.clang.opt.s
	$(ARMGNU)-as xmega01.clang.opt.s -o xmega01.clang.opt.o
	$(ARMGNU)-ld -o xmega01.clang.opt.elf -T memmap vectors.o xmega01.clang.opt.o
	$(ARMGNU)-objdump -D xmega01.clang.opt.elf > xmega01.clang.opt.list
	$(ARMGNU)-objcopy xmega01.clang.opt.elf xmega01.clang.opt.bin -O binary

dumphex : dumphex.c
	gcc -o dumphex dumphex.c

blinker3.h : blinker03.hex dumphex
	./dumphex blinker03.hex

blinker.h : blinker01.s.hex dumphex
	./dumphex blinker01.s.hex


blinker01.s.hex : blinker01.s
	avra -fI blinker01.s

blinker02.s.hex : blinker02.s
	avra -fI blinker02.s

blinker03.hex : blinker03.s xblinker.c avrmap
	avr-as blinker03.s -o blinker03.o
	avr-gcc -c $(COPS) xblinker.c -o xblinker.o
	avr-ld -T avrmap blinker03.o xblinker.o -o blinker03.elf
	avr-objdump -D blinker03.elf > blinker03.list
	avr-objcopy blinker03.elf -O ihex blinker03.hex

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s
	rm -f blinker01.s.*
	rm -f blinker02.s.*
	rm -f dumphex
	rm -f *.hex


