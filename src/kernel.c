#include <stddef.h>
#include <stdint.h>

#include "term.h"

extern struct multiboot_data *_multiboot_info;
extern void multiboot_init(struct multiboot_data *mb_info);

void kernel_main(void)
{
	terminal_initialise();
	terminal_writestring("Hello, kernel!\n");

	multiboot_init(_multiboot_info);
}
