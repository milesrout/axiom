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

include Makefile.mk

axiom.bin: linker.ld ${OBJFILES}
	${CC} -T linker.ld -o axiom.bin ${LDFLAGS} ${OBJFILES} -lgcc
