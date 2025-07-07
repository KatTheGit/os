section .multiboot
;bits 32 ; tell bro we're in 32 bit mode
;align 32 ; According to the multiboot standard, the header should be 32-bit
;aligned, and in the first 8192 bytes.

; byte  = 8  bits
; word  = 16 bits
; dword = 32 bits  (aka longword)
; qword = 64 bits

; multiboot header
magic:    dd 0x1badb002 ; -multiboot manual
flags:    dd 0 ; we don't want any flags
; "The field ‘checksum’ is a 32-bit unsigned value which, when added to the
; other magic fields (i.e. ‘magic’ and ‘flags’), must have a 32-bit unsigned
; sum of zero." - multiboot manual
; magic + flags + checksum = 0  <=> checksum = - (flags + magic)
checksum: dd -(0x1badb002 + 0) ; 

;times 20 db 0 ; Temporary padding, otherwise GRUB doesn't seem to like this.

; GRUB (as it follows the multiboot standard) will not set up a stack. We need
; to do that ourselves. We need to move the stack's address in the `esp`
; register. But first, set up the stack itself.
; C tends to use the stack (fun fact)


section .bss
; The stack grows downwards in x86.
stack_bottom:
  resb 16384 ; (16KiB) stack
stack_top:

extern kernel_main


section .text 
global _start 
_start:
  ; ENTRY POINT TO THE KERNEL

  ; Initialise the stack pointer
  mov esp, stack_top

  cli ; clear all interrupt flags

  ; run the kernel
  call kernel_main
