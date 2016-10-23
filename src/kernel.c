#include <stddef.h>
#include <stdint.h>

#include "term.h"

void kernel_main(void)
{
	terminal_initialise();
	for (;;) {
		terminal_writestring("Hello, kernel\n");
		terminal_writestring("Hello, kernel!\n");
	}
}
