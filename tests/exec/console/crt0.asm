BITS 16
extern main
global start
start:
    lgdt [GDT_DESC]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 8:pe_enter ; 1000b

BITS 32
pe_enter:
    mov dword eax, 0x10 ; 10000b
    mov ss, ax
    mov ds, ax
    call main
    jmp 0:0 ; E9 (F4 83 FF FF)

; FF FF 00 00 00 92 CF 00
GDT:
    ; null segment descriptor
    dw 0
    dw 0
    db 0
    db 0
    db 0
    db 0
    ; code segment descriptor
    ; 0000 FFFF
    ; 00CF 9800
    dw 0xFFFF       ; limit low
    dw 0            ; base low
    db 0            ; base middle
    db 10011000b    ; access
    db 11001111b    ; limit + flags
    db 0            ; base high
    ; data segment descriptor
    ; 0000 FFFF
    ; 00CF 9200
    dw 0xFFFF       ; limit low
    dw 0            ; base low
    db 0            ; base middle
    db 10010010b    ; access
    db 11001111b    ; limit + flags
    db 0            ; base high
    ; staci segment descriptor
    ; 0000 FFFF
    ; 00CF 9200
    dw 0xFFFF       ; limit low
    dw 0            ; base low
    db 0            ; base middle
    db 10010010b    ; access
    db 11001111b    ; limit + flags
    db 0            ; base high

; 07 00 0C 7C 00 00
GDT_DESC:
    dw (GDT_DESC - GDT - 1)
    dd GDT