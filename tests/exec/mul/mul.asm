BITS 32
    org 0x7c00

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

    mov dword [ebp], 0xFFFFFFFF
    mov eax, 0xFFFFFFFF ; 
    mul dword [ebp]
    push edx ; 7BD4: 0xFFFFFFFE
    push eax ; 7BD0: 0x1 (0xFFFFFFFE 00000001: 18446744065119617025)
    pushf ; 7BCC: 0x801: 0x0 0x0 00 00 1(OF)0 00 | 00 00 00 01(CF)

    jmp 0