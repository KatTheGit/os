# compile the kernel into an object file
echo "compiling kernel..."
i686-elf-gcc -c src/kernel.c -o target/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
# assemble the multiboot header into an object file
echo ""
echo "assembling multiboot header..."
nasm -f elf32 src/multiboot.asm -o target/multiboot.o
# link the two into the final binary
echo ""
echo "linking both..."
i686-elf-gcc -T src/linker_script.ld -o target/myos.bin -ffreestanding -O2 -nostdlib target/multiboot.o target/kernel.o -lgcc
echo ""
echo "making grub bootable image"
cp -r src/grub-iso target/grub-bootable-iso
./grubiso.sh
echo ""
echo "done"
