#include <stdint.h>

#include "kernel_drivers/vga.c"
// Global Descriptor Table
#include "gdt.c"


void kernel_main(void) {

	// Load the Global Descriptor Table
	gdt_init();
	
	// For now, this just removes the cursor and blanks the screen.
	vga_init();

	// beautiful characters
	// minor distraction from making the gdt oops
	char charlist[255];
	for (uint8_t i = 0; i< 254; i++) {
		charlist[i] = i+1;
	}
	charlist[254] = 0;
	vga_term_print("         1         2         3         4         5         6         7         8");
	vga_term_print("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
	vga_term_print(" ");
	vga_term_print(&charlist[0]);
	// Kernel shell
	vga_term_print("TERMINAL SHELL"); // So apparently using "quotes" null
																		// terminates automatically, very demure
	vga_term_print(">");
	

  asm("hlt"); // Don't waste my precious cpu cycles grrrr arf arf arf
}
