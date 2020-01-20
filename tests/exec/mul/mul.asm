BITS 32
    org 0x7c00

    ; mul ;

    sub esp, 16 ; ESP: 7BF0
    mov ebp, esp ; EBP: 7BF0

    mov byte [ebp], 0xF
    mov al, 0xF
    mul byte [ebp]
    push eax ; 7BEC: 0xE1(255)
    pushf ; 7BE8: 0x0

    mov byte [ebp], 0xFF ; 255
    mov al, 0xFF
    mul byte [ebp]
    push eax ; 7BE4: 0xFE01(65025)
    pushf ; 7BE0: 0x801: 0x0 0x0 00 00 1(OF)0 00 | 00 00 00 01(CF)

    mov dword [ebp], 0xFF
    mov eax, 0xFF
    mul dword [ebp]
    push eax ; 7BDC: 0xFE01
    pushf ; 7BD8: 0x0

    mov dword [ebp], 0xFFFFFFFF ; 4294967295
    mov eax, 0xFFFFFFFF
    mul dword [ebp]
    push edx ; 7BD4: 0xFFFFFFFE
    push eax ; 7BD0: 0x1 (0xFFFFFFFE 00000001: 18446744065119617025)
    pushf ; 7BCC: 0x801

    ; imul ;

    mov byte [ebp], 0x3
    mov al, 0x2
    imul byte [ebp]
    push eax ; 7BC8: 0x6
    pushf ; 7BC4: 0x0
    
    mov byte [ebp], 0x80 ; -128
    mov al, 0xF
    imul byte [ebp]
    push eax ; 7BC0: 0xF880(-1920)
    pushf ; 7BBC: 0x0801

    mov dword [ebp], 0xFFFFFFFF ; -1
    mov eax, 0xFFFFFFFF ; -1
    imul dword [ebp]
    push edx ; 7BB8: 0x0
    push eax ; 7BB4: 0x1
    pushf ; 7BB0: 0x0

    mov dword [ebp], 0x80000000 ; -2147483648
    mov eax, 0x0FFFFFFF ; 268435455
    imul dword [ebp]
    push edx ; 7BAC: 0xF8000000
    push eax ; 7BA8: 0x80000000 (-576460750155939840)
    pushf ; 7BA4: 0x0

    mov dword [ebp], 0xFFFFFFFF ; -1
    imul eax, [ebp], byte 3
    push eax ; 7BA0: 0xFFFFFFFD (-3)

    imul eax, byte 3 ; 6B C0 03; C0: 11 000 000: eax
    push eax ; 7B9c: 0xFFFFFFF7 (-9)

    jmp 0