CC = i686-elf-gcc
AS = i686-elf-as

all: axiom.bin

clean:
	rm *.o *.bin

.PHONY: all clean

axiom.bin: src/linker.ld boot.o kernel.o
	${CC} -T src/linker.ld -o axiom.bin -ffreestanding -O2 -nostdlib boot.o  kernel.o -lgcc

boot.o: src/arch/i686/boot.s
	${AS} src/arch/i686/boot.s -o boot.o

kernel.o: src/kernel.c
	${CC} -c src/kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
