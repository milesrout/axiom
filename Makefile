
CC = i686-elf-gcc
AS = i686-elf-as
CFLAGS = -std=c89 -pedantic -ffreestanding -O2 -W -Wall
LDFLAGS = -ffreestanding -O2 -nostdlib

all: axiom.bin

clean:
	rm *.o *.bin

run: all
	qemu-system-i386 -kernel axiom.bin -initrd axiom.bin

.PHONY: all clean run

axiom.bin: linker.ld kernel.o string.o multiboot.o term.o mem.o boot.o
	${CC} -T linker.ld -o axiom.bin ${LDFLAGS} kernel.o string.o multiboot.o term.o mem.o boot.o -lgcc

boot.o: boot.s 
	${AS} boot.s -o boot.o

kernel.o: kernel.c term.h
	${CC} -c kernel.c -o kernel.o ${CFLAGS}

string.o: string.c string.h
	${CC} -c string.c -o string.o ${CFLAGS}

multiboot.o: multiboot.c term.h
	${CC} -c multiboot.c -o multiboot.o ${CFLAGS}

term.o: term.c mem.h string.h term.h
	${CC} -c term.c -o term.o ${CFLAGS}

mem.o: mem.c mem.h
	${CC} -c mem.c -o mem.o ${CFLAGS}
