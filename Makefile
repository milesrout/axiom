CC = i686-elf-gcc
AS = i686-elf-as

all: axiom.bin

clean:
	rm *.o *.bin

.PHONY: all clean

axiom.bin: linker.ld boot.o kernel.o
	${CC} -T linker.ld -o axiom.bin -ffreestanding -O2 -nostdlib boot.o  kernel.o -lgcc

boot.o: boot.s
	${AS} boot.s -o boot.o

kernel.o: kernel.c
	${CC} -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
