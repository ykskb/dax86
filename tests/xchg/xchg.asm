BITS 32
    org 0x7c00
    mov dword eax, 3
    push 1
    mov edx, esp
    xchg byte [edx], al

    mov dword eax, 4
    push 2
    mov edx, esp
    xchg dword [edx], eax
    jmp 0