BITS 32
    org 0x7c00
    push 0
    mov edx, esp
    mov dword [edx], 0xFF
    mov dword eax, 0xFF
    test byte [edx], al ; flags: 10000000 (sign flag)
    ; pushf ;
    push 0
    mov edx, esp
    mov dword [edx], 0xFF
    mov dword eax, 0
    test dword [edx], eax ; flags: 01000000 (zero flag)
    ; pushf ;
    jmp 0