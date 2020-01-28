BITS 32
    org 0x7c00

    ; div rm8 ;

    sub esp, 16 ; ESP: 7BF0
    mov ebp, esp ; EBP: 7BF0

    mov byte [ebp], 0xF
    mov al, 0xF
    div byte [ebp]
    push eax ; 7BEC: 0x01

    mov byte [ebp], 0x2
    mov al, 0x7
    div byte [ebp]
    push eax ; 7BE8: 00 00 01 03

    ; div rm32 ;

    mov dword [ebp], 0xFFFFFFFE ; 4294967294
    mov edx, 0x1
    mov eax, 0x11111111 ; 4581298449
    div dword [ebp]
    push edx ; 7BE4: 0x1F ; 0x11111113(rem: 286331155)
    push eax ; 7BE0: 0x1

    mov dword [ebp], 0xFFFFFFFF ; 4294967295
    mov edx, 0x0 ; 
    mov eax, 0xFFFFFFFF
    div dword [ebp]
    push edx ; 7BDC: 0x0
    push eax ; 7BD8: 0x1

    ; idiv rm8 ;

    mov byte [ebp], 0xF
    mov al, 0xF
    idiv byte [ebp]
    push eax ; 7BD4: 0x01

    mov byte [ebp], -2
    mov al, 7
    idiv byte [ebp]
    push eax ; 7BD0: 00 00 01(1: same sign as dividend) FD(1111 1101 = -3)

    ; idiv rm32 ;

    mov eax, 0
    mov dword [ebp], 0

    mov dword [ebp], 3
    mov edx, -1
    mov eax, -1
    idiv dword [ebp]
    push eax ; 7BCC: 0x0
    push edx ; 7BC8: -1 (0xFFFFFFFF)

    mov dword [ebp], 0x7FFFFFFF ; 2147483647
    mov edx, 0x7FFF
    mov eax, 0xFFFFFFFF ; 0x7FFF FFFF FFFF: 140737488355327
    idiv dword [ebp]
    push eax ; 7BC4: 0x10000 (65536)
    push edx ; 7BC0: 0xFFFF (65535)
    
    jmp 0