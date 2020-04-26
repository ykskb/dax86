BITS 32
    org 0x7c00
    mov edx, 0x03f8
    in al, dx
    in al, 0x64
    jmp 0
