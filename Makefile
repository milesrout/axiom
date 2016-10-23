CC = i686-elf-gcc
AS = i686-elf-as

all: axiom.bin

clean:
	rm *.o *.bin

run: all
	qemu-system-i386 -kernel axiom.bin

.PHONY: all clean run

axiom.bin: src/linker.ld boot.o kernel.o term.o string.o mem.o
	${CC} -T src/linker.ld -o axiom.bin -ffreestanding -O2 -nostdlib boot.o  kernel.o term.o string.o mem.o -lgcc

boot.o: src/boot.s
	${AS} src/boot.s -o boot.o

kernel.o: src/kernel.c src/term.h
	${CC} -c src/kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

term.o: src/term.c src/term.h src/string.h
	${CC} -c src/term.c -o term.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

string.o: src/string.c src/string.h
	${CC} -c src/string.c -o string.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

mem.o: src/mem.c src/mem.h
	${CC} -c src/mem.c -o mem.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
