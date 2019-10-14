BITS 32
    org 0x7c00
    push 3
    mov dword eax, esp
    pop dword [eax - 4] ; 0x7CFC: 3
    sub esp, 4

    mov dword eax, 3
    sub dword eax, 3 ; EFLAGS: 0000 0000 0100 0000
    pushfd ; 9C ; 0x7CF8: 0x40
    mov dword eax, 3
    sub dword eax, 4 ; EFLAGS: 1000 0001 ; EAX: 0xFFFF FFFF
    popfd ; 9D ; EFLAGS: 0000 0000 0100 0000
    pushfd ; 0x7CF4: 0x40

    mov byte ah, 0xFF
    sahf ; EFLAGS: 0000 0000 1101 0101
    lahf ; EAX: 0xFFFFD5FF (D5FF: 1101 0101 1111 1111)

    jmp 0