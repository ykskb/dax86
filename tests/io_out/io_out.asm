BITS 32
    org 0x7c00
    mov edx, 0x03f8
    mov eax, 0x41
    out dx, al
    jmp 0
