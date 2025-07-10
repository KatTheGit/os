# compile the kernel into an object file
if i686-elf-gcc -c src/kernel.c -o target/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra; then
  echo "Kernel compiled."
else
  echo "An error occured during compilation."
  exit 1
fi
# assemble the multiboot header into an object file
echo ""
if nasm -f elf32 src/multiboot.asm -o target/multiboot.o; then
  echo "Multiboot header assembled"
else
  echo "An error occured while assembling the header."
  exit 1
fi
# link the two into the final binary
echo ""
if i686-elf-gcc -T src/linker_script.ld -o target/myos.bin -ffreestanding -O2 -nostdlib target/multiboot.o target/kernel.o -lgcc; then
  echo "Everything linked, your majesty."
else
  echo "Failed to link files."
  exit 1
fi

# Make grub bootable image
echo ""
cp -r src/grub-iso target/grub-bootable-iso
if ./grubiso.sh; then
  echo "Created bootable image"
else
  echo "Failed to create bootable image"
  exit 1
fi


echo ""
echo "done"
