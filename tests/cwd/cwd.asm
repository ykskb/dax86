BITS 32
    org 0x7c00
    ; mov word ax, 3
    ; cwde
    ; push eax

    ; mov word ax, -3
    ; cwde
    ; push eax

    mov dword eax, 3
    cdq
    push edx
    push eax

    mov dword eax, -3
    cdq
    push edx
    push eax

    jmp 0