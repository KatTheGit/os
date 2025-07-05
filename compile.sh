# compile the kernel into an object file
i686-elf-gcc -c src/kernel.c -o target/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
echo "kernel compiled"
# assemble the multiboot header into an object file
nasm -f elf32 src/multiboot.asm -o target/multiboot.o
echo "multiboot header assembled"
# link the two into the final binary
i686-elf-gcc -T src/linker_script.ld -o target/myos.bin -ffreestanding -O2 -nostdlib target/multiboot.o target/kernel.o -lgcc
echo "both linked"
./grubiso.sh
echo "grub bootable image done"
