#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "mem.h"
#include "panic.h"
#include "string.h"
#include "term.h"

uint8_t vga_entry_colour(enum vga_colour fg, enum vga_colour bg)
{
	return fg | bg << 4;
}

static uint16_t vga_entry(unsigned char uc, uint8_t colour)
{
	return (uint16_t) uc | (uint16_t) colour << 8;
}

static size_t const VGA_WIDTH = 80;
static size_t const VGA_HEIGHT = 25;
static size_t const TAB_WIDTH = 8;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_colour;
uint16_t *terminal_buffer;

void terminal_initialise(void)
{
	size_t x, y;

	terminal_row = 0;
	terminal_column = 0;
	terminal_colour = vga_entry_colour(VGA_COLOUR_LIGHT_GREY, VGA_COLOUR_BLACK);
	terminal_buffer = (uint16_t*) 0xb8000;
	for (y = 0; y < VGA_HEIGHT; y++) {
		for (x = 0; x < VGA_WIDTH; x++) {
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
		size_t x;

		/* actually stay on the last row */
		terminal_row--;

		/* move everything backwards by a row */
		memmove(terminal_buffer,
			terminal_buffer + VGA_WIDTH,
			sizeof(uint16_t) * VGA_WIDTH * (VGA_HEIGHT - 1));

		/* clear the last row */
		for (x = 0; x < VGA_WIDTH; x++) {
			size_t const index = terminal_row*VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_colour);
		}
	}

	/* go back to the beginning of the row */
	terminal_column = 0;
}

void terminal_printf(char const *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    while ('\0' != *fmt) {
        if ('%' == *fmt) {
            fmt++;

            if (*fmt == 'l') {
                fmt++;

                if (*fmt == 'u') {
                    uint64_t v = va_arg(args, uint64_t);
                    terminal_put_u64(v);
                } else if (*fmt == 'b') {
                    uint64_t v = va_arg(args, uint64_t);
                    terminal_put_u64b(v);
                } else {
                    /* TODO: terminal_putstr(error message) */
                    panic();
                }
            } else if (*fmt == 'u') {
                uint32_t v = va_arg(args, uint32_t);
                terminal_put_u32(v);
            } else if (*fmt == 'b') {
                uint32_t v = va_arg(args, uint32_t);
                terminal_put_u32b(v);
            } else if (*fmt == 'p') {
                void *p = va_arg(args, void*);
                terminal_put_ptr(p);
            } else if (*fmt == 's') {
                char const *s = va_arg(args, char*);
                terminal_put_str(s);
            } else {
                /* TODO: terminal_putstr(error message) */
                panic();
            }
        } else {
            terminal_putchar(*fmt);
        }
        fmt++;
    }
}

void terminal_putchar(char c)
{
	if (c == '\n') {
		terminal_advanceline();
		return;
	}
	if (c == '\t') {
		terminal_column -= (terminal_column % TAB_WIDTH);
		terminal_column += TAB_WIDTH;
		if (terminal_column >= VGA_WIDTH)
			terminal_advanceline();
		return;
	};

	terminal_putentryat(c, terminal_colour, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_advanceline();
	}
}

void terminal_put_u32b(uint32_t n)
{
    terminal_put_u64b((uint64_t)n);
}

void terminal_put_u64b(uint64_t n)
{
	int i;
	terminal_putchar('0');
	terminal_putchar('b');
	for (i = 0; i < 64; i++) {
		bool b = (n & (1 << (63 - i))) >> (63 - i);
		if (i != 0 && i % 4 == 0) terminal_putchar('_');
		if (b)
			terminal_putchar('1');
		else
			terminal_putchar('0');
	}
}

void terminal_put_ptr(void const *p)
{
	terminal_put_u32((uint32_t)p);
}

void terminal_put_u32(uint32_t n)
{
	terminal_put_u64((uint64_t)n);
}

void terminal_put_u64(uint64_t n)
{
	/* a 64-bit unsigned integer has at most 20 digits */
	char num[21] = {0};
	int i = 0;
	char *p = num;

	if (n == 0) {
		terminal_putchar('0');
		return;
	}

	while (n != 0 && i != 20) {
		num[i++] = (n % 10) + '0';
		n /= 10;
	}

	while (*p != '\0') {
		p++;
	}

	p--;

	do {
		terminal_putchar(*p--);
	} while (p >= num);
}

void terminal_write(char const *data, size_t size)
{
	size_t i;

	for (i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_put_str(char const *data)
{
	terminal_write(data, strlen(data));
}
