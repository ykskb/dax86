BITS 32
    org 0x7c00
    push 3
    mov dword eax, esp
    pop dword [eax - 4]
    jmp 0