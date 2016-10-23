#include <stddef.h>
#include <stdint.h>
#include "string.h"
#include "term.h"

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
	VGA_COLOUR_WHITE = 15,
};

static uint8_t vga_entry_colour(enum vga_colour fg, enum vga_colour bg)
{
	return fg | bg << 4;
}

static uint16_t vga_entry(unsigned char uc, uint8_t colour)
{
	return (uint16_t) uc | (uint16_t) colour << 8;
}


size_t strlen(char const *str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

static size_t const VGA_WIDTH = 80;
static size_t const VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_colour;
uint16_t *terminal_buffer;

void terminal_initialise(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_colour = vga_entry_colour(VGA_COLOUR_LIGHT_GREY, VGA_COLOUR_BLACK);
	terminal_buffer = (uint16_t*) 0xb8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			size_t const index = y*VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_colour);
		}
	}
}

void terminal_setcolour(uint8_t colour)
{
	terminal_colour = colour;
}

void terminal_putentryat(char c, uint8_t colour, size_t x, size_t y)
{
	size_t const index = y*VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, colour);
}

static void terminal_advanceline(void)
{
	/* if we're at the end of the terminal, start shuffling everything up
	   a row at a time */
	if (++terminal_row == VGA_HEIGHT) {
		/* actually stay on the last row */
		terminal_row--;

		/* move everything backwards by a row */
		memmove(terminal_buffer,
			terminal_buffer + VGA_WIDTH,
			sizeof(uint16_t) * VGA_WIDTH * (VGA_HEIGHT - 1));

		/* clear the last row */
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			size_t const index = terminal_row*VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_colour);
		}
	}

	/* go back to the beginning of the row */
	terminal_column = 0;
}

void terminal_putchar(char c)
{
	if (c == '\n') {
		terminal_advanceline();
		return;
	}
	terminal_putentryat(c, terminal_colour, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_advanceline();
	}
}

void terminal_write(char const *data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(char const *data)
{
	terminal_write(data, strlen(data));
}
