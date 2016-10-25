#include <stddef.h>
#include <stdint.h>

#include "term.h"

extern void multiboot_init();

void kernel_main(void)
{
	terminal_initialise();
	terminal_put_str("Hello, kernel!\n");

	multiboot_init();
}
