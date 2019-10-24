BITS 32
    org 0x7c00
    push 0xFF ; 0x7BFC: 0xFF
    mov dword eax, 0x80
    mov edx, esp
    test byte [edx], al
    pushfd ; 0x7BF8: 10000000 (0x80) (sign flag)
    
    push 0xFF ; 0x7BF4: 0xFF
    mov dword eax, 0
    mov edx, esp
    test dword [edx], eax
    pushfd ; 0x7BF0: 01000000 (0x40) (zero flag)

    mov dword eax, 0xFF
    test byte al, 0x80
    pushfd ; 0x7BEC: 10000000 (0x80) (sign flag)

    mov dword eax, 0
    test byte al, 0x80
    pushfd ; 0x7BE8: 01000000 (0x40) (zero flag)

    mov dword eax, 0xF0000000
    test dword eax, 0x80000000
    pushfd ; 0x7BE4: 10000000 (0x80) (sign flag)

    mov dword eax, 0
    test dword eax, 0xFFFFFFFF
    pushfd ; 0x7BE0: 01000000 (0x40) (zero flag)

    jmp 0