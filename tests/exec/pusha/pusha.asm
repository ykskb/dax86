BITS 32
    org 0x7c00

    mov eax, 0x1
    mov ecx, 0x2
    mov edx, 0x3
    mov ebx, 0x4
    ; mov esp, 0x5
    mov ebp, 0x6
    mov esi, 0x7
    mov edi, 0x8
    pusha
    mov eax, 0x8
    mov ecx, 0x7
    mov edx, 0x6
    mov ebx, 0x5
    ; mov esp, 0x4
    mov ebp, 0x3
    mov esi, 0x2
    mov edi, 0x1
    popa
    jmp 0