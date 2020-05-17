BITS 16
    org 0x7c00

load_gdt:
    lgdt [GDT_DESC] ; 0F 01 15 (14 7C 00 00): 15=21: 000 10(op) 101(r/m)
    ; xgetbv ; 0F 01 D0: D0(so): 110 10(op) 000(r/m)
    ; xsetbv ; 0F 01 D1: D1(so): 110 10(op) 001(r/m)
    ; monitor ; 0F 01 C8: C8(so): 110 01(op) 000(r/m)
    ; mwait ; 0F 01 C9: C9=201: 110 01(op) 001(r/m)
    
    ; Enable Protected Mode: PE bit on CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 8:pe_enter ; 1000b

BITS 32
pe_enter:
    mov eax, 0x11 ; 10001b
    mov ds, eax
    mov ss, eax
    jmp [NEAR_ADDR1]
    jmp 0x1234:0x56789ABC ; Will be skipped.
    push cs

    jmp 0x8:0x7C39 ; EA 04 00 00 00 7C 00 ; 0x7C00 + 0xE = 0x7C0E
    jmp 0x2345:0x6789ABCD ; EA jmp ptr16:16/32 ; EA BC 9A 78 56 34 12 ; Will be skipped.
    push cs

    jmp far [FAR_ADDR1] ; FF 2D (same as [0x7C25]) ; FF 2D (0010 1011) 23 7C 00 00 ;
    push cs ; Will be skipped.
    jmp 0x3456:0x789ABCDE ; Will be skipped.
    push cs

    jmp 0:0

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

; 07 00 0C 7C 00 00
GDT_DESC:
    dw (GDT_DESC - GDT - 1)
    dd GDT

FAR_ADDR1:
    dw 0x8  ; Seg
    dd 0x7C48   ; Offset

NEAR_ADDR1:
    dd 0x7C2A