void kernel_main(void) {
  asm(".intel_syntax noprefix"); // tell the compiler to use based syntax
  asm("mov eax, 1234567"); // Tested, yay it works
  asm("hlt"); // this will segfault in userspace lol
}
