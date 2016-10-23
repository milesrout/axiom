enum vga_colour {
	VGA_COLOUR_BLACK = 0,
	VGA_COLOUR_BLUE = 1,
	VGA_COLOUR_GREEN = 2,
	VGA_COLOUR_CYAN = 3,
	VGA_COLOUR_RED = 4,
	VGA_COLOUR_MAGENTA = 5,
	VGA_COLOUR_BROWN = 6,
	VGA_COLOUR_LIGHT_GREY = 7,
	VGA_COLOUR_DARK_GREY = 8,
	VGA_COLOUR_LIGHT_BLUE = 9,
	VGA_COLOUR_LIGHT_GREEN = 10,
	VGA_COLOUR_LIGHT_CYAN = 11,
	VGA_COLOUR_LIGHT_RED = 12,
	VGA_COLOUR_LIGHT_MAGENTA = 13,
	VGA_COLOUR_LIGHT_BROWN = 14,
	VGA_COLOUR_WHITE = 15
};
extern uint8_t vga_entry_colour(enum vga_colour fg, enum vga_colour bg);
extern void terminal_initialise(void);
extern void terminal_setcolour(uint8_t colour);
extern void terminal_putentryat(char c, uint8_t colour, size_t x, size_t y);
extern void terminal_putchar(char c);
extern void terminal_putint_u64(uint64_t c);
extern void terminal_write(char const *data, size_t size);
extern void terminal_writestring(char const *data);
