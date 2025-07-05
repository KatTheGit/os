cp target/myos.bin target/grub-bootable-iso/boot/
grub-mkrescue -o target/os.iso target/grub-bootable-iso
