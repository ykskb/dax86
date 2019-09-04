; only for test at moment.
; rep doesn't work with in al, dx (EC) instruction
; todo: replace with supported instructions.
BITS 32
    org 0x7c00
    mov ecx, 5
    mov edx, 0x03f8
    rep in al, dx
    jmp 0