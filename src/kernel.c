#include <stddef.h>
#include <stdint.h>

#include "term.h"

extern uint64_t _multiboot_info;

void kernel_main(void)
{
	terminal_initialise();
	terminal_writestring("Hello, kernel!\n");
	terminal_putint_u64(_multiboot_info);
	terminal_putchar('\n');
}
