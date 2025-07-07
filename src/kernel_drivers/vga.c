// VGA-compatible video cards will read the memory buffer at 0xB8000, and
// display what's there. The BIOS seems to like starting us in text mode
// already, so I won't bother for now with more complicated stuff.

#include <stdint.h>
#include <stddef.h>

// value of the pointer to the memory buffer for vga text mode
#define VGA_MEM (uint16_t *) 0xb8000

// buffer to keep scrollable text
#define TERMINAL_BUFFER_SIZE 80 * 1024 //1024 lines
uint8_t terminal_text[TERMINAL_BUFFER_SIZE] = { 'A' }; // initialise with 0s

// color byte:
// 0000 0000  a = bright (1) dark (0)
// argb argb  r, g, b = red, green, blue
//  bg   fg   fg = foreground, bg = background

#define WHITE 0b00111111 // bright white on dark cyan

/**
 * Do what's necessary to prep for VGA text mode. (This assumes text mode is
 * already enabled)
 */
void vga_init(void) {
	asm(".intel_syntax noprefix"); // tell the compiler to use based syntax
	// disable the text mode cursor by writing to the appropriate port
	asm("mov al, 0x0a");
	asm("mov dx, 0x3d4");
	asm("out dx, al");
	asm("mov al, 0x20");
	asm("mov dx, 0x3d5");
	asm("out dx, al");
	// go back to the syntax C likes or bad stuff happens because idk
	asm(".att_syntax prefix");
	// blank the screen
	for (int i = 0; i < (80*25); i++) {
		*(VGA_MEM + i) = 0;
	}
	// initialise the terminal buffer
	for (int i = 0; i<TERMINAL_BUFFER_SIZE; i++) {	
		terminal_text[i] = 0;
	}
}

/**
 * While in VGA text mode, write a character to a certain position on the
 * 25x80 screen. 0-indexed. (0-24, 0-79)
 */
void write_at_vga(uint8_t character, uint8_t color, uint8_t x, uint8_t y) {
	uint16_t offset = y * 80 + x;
	*(VGA_MEM + offset) = character | ( color << 8 );
}

void display_terminal_text(int scroll_index) {;
	for (int x = 0; x < 80; x++) {
		for (int y = 0; y < 25; y++) {
			// calculate the start of the screen in the buffer
			int address = TERMINAL_BUFFER_SIZE - scroll_index*80 - 80*25 + x + y*80;
			write_at_vga(terminal_text[address], WHITE, x, y);
		}
	}
}

// stolen from osdev hehehehehehe
int strlen(char* str) 
{
	int len = 0;
	while (str[len])
		len++;
	return len;
}

// Add a line of text to the terminal buffer
void terminal_addline(char* line) {
	int length = strlen(line);
	// for every character...
	for (int i = 0; i<length; i++) {
		// if it's first in a line...
		if ((i % 80) == 0) {
			// shift the buffer by one line
			for (int j = 0; j<TERMINAL_BUFFER_SIZE-80; j++) {
				terminal_text[j] = terminal_text[j+80];
				terminal_text[j+80] = 0;
			}
		}
		// add the character to the current line
		terminal_text[TERMINAL_BUFFER_SIZE-80+i%80] = line[i];
	}
}

// adds a line of text to the terminal buffer and prints the terminal buffer.
void vga_term_print(char* line) {
	terminal_addline(line);
	display_terminal_text(0);
}
