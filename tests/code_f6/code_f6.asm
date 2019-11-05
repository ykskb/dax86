BITS 32
    org 0x7c00

    push 0xFF ;
    mov dword eax, esp
    test byte [eax], 0x0
    pushfd ; 01000000 (0x40) (zero flag)

    push 0xFF ;
    mov dword eax, esp
    test byte [eax], 0x80
    pushfd ; 10000000 (0x80) (sign flag)

    push 5 ; 0000 0101
    mov dword eax, esp
    not byte [eax] ; F6 2 not rm8 ; 1111 1010 (0xFA)

    push 5 ; 0000 0101
    mov dword eax, esp
    neg byte [eax] ; F6 3 neg rm8 ; 1111 1011 (0xFB): -128 + 123 (0111 1011) = 5

    push -5 ; 1111 1011
    mov dword eax, esp
    neg byte [eax] ; 0x05

    ; code F7 ;

    push 0xFF ;
    mov dword eax, esp
    test dword [eax], 0x0
    pushfd ; 01000000 (0x40) (zero flag)

    push 0xFF ;
    mov dword eax, esp
    test dword [eax], 0x80
    pushfd ; 10000000 (0x80) (sign flag)

    push 5 ; 0x00 0x00 0x00 0000 0101
    mov dword eax, esp
    not dword [eax] ; F6 2 not rm8 ; 0xFF 0xFF 0xFF 1111 1010 (0xFA)

    push 5 ; 0x00 0x00 0x00 0000 0101
    mov dword eax, esp
    neg dword [eax] ; F6 3 neg rm8 ; 0xFF 0xFF 0xFF 1111 1011 (0xFB): -128 + 123 (0111 1011) = 5

    push dword 0xFAFFFFFB ; -83886085: 1111 1010 0xFF 0xFF 1111 1011
    mov dword eax, esp
    neg dword [eax] ; 83886085: 0x05 00 00 05

    jmp 0