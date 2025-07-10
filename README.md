## OS

An attempt at a homemade OS kernel, using wiki.OSDev.org and the necessary docs. I'm making this to learn more about OS dev and the secret low level computer shenanigans.

### To do

- [x] GCC cross-compiler
- [x] Grub bootloader
  - [x] Bootloader.asm
    - [x] grub-file --is-x86-multiboot
    - [x] Actually test it
      - [x] Set up QEMU and GDB
      - [x] Link kernel and bootloader (link NASM asm and GCC C.)
        - [x] Get it to work
        - [ ] Understand how it works and customise
- [ ] Interrupts
  - [x] GDT
  - [ ] IDT
- [ ] 
- [ ] Video
  - [x] Basic VGA video for basic debug
  - [ ] GPU stuff
    - [ ] kms
- [ ] Use a makefile instead of a bash script

### Random stuff

```sh
xxd
-b (binary dump)
-c 8 (show address every 8 bytes (column size))
```
x86 is little endian. 0x1234 -> 0x34 0x12

```sh
i686-elf-gcc -c src/kernel.c -o target/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
nasm -f elf32 src/multiboot.asm -o target/multiboot.o
# Link
i686-elf-gcc -T src/linker_script.ld -o target/myos.bin -ffreestanding -O2 -nostdlib target/multiboot.o target/kernel.o -lgcc
```

### Compile

Build a [cross-compiler](https://wiki.osdev.org/GCC_Cross-Compiler).

gcc 14.2.0 compiled with 15.1.1

binutils 2.43.1 compiled with gcc 15.1.1

run compile.sh

### Project structure

- `grub-iso/` directory used to create the grub bootable image with grub-mkrescue
- `kernel_drivers/` directory for hardcoded kernel drivers.