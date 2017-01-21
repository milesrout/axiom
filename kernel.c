#include <stddef.h>
#include <stdint.h>

#include "term.h"

extern void multiboot_init();

void kernel_main(void)
{
	terminal_initialise();
	terminal_printf("%s\n", "Hello, kernel!");

	multiboot_init();
}
