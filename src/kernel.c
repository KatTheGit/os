#include <stdint.h>

#include "kernel_drivers/vga.c"


void kernel_main(void) {

	// For now, this just removes the cursor and blanks the screen.
	vga_init();

	// Kernel shell
	vga_term_print("Hello world!");

  asm("hlt"); // Don't waste my precious cpu cycles grrrr arf arf arf
}
