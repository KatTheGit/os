global set_gdt

gdtr:
  dw 0 ; 2 bytes (dword) for limit (size of gdt in bytes)
  dd 0 ; 4 bytes (dword) for base (pointer to gdt)


set_gdt:
  mov ax, [esp+4] ; function arg 1 (limit aka size of gdt)
  mov [gdtr], ax ; put that at the beginning
  mov eax, [esp+8] ; function arg 2 (base aka pointer)
  mov [gdtr+2], eax ; put that after

  lgdt [gdtr] ; load the value of gdtr (limit + pointer)

  ; reload segment registers.
  ; jump to the next part of the code, with 0x08 as selector (code offset in
  ; GDT) This sets register cs to 0x08.
  jmp 0x08:reload_segments


reload_segments:
  ; reload the rest of the segment registers
  mov   ax, 0x10 ; 0x10 is the kernel data segment offset
  mov   ds, ax
  mov   es, ax
  mov   fs, ax
  mov   gs, ax
  mov   ss, ax

  ret
