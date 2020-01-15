BITS 32
    org 0x7c00

load_gdt:
    lgdt [GDT_DESC] ; 0F 01 15 (14 7C 00 00): 15=21: 000 10(op) 101(r/m)
    ; xgetbv ; 0F 01 D0: D0(so): 110 10(op) 000(r/m)
    ; xsetbv ; 0F 01 D1: D1(so): 110 10(op) 001(r/m)
    ; monitor ; 0F 01 C8: C8(so): 110 01(op) 000(r/m)
    ; mwait ; 0F 01 C9: C9=201: 110 01(op) 001(r/m)
    jmp 0 ; E9 (F4 83 FF FF)

; FF FF 00 00 00 92 CF 00
GDT:
    dw 0xFFFF       ; limit low
    dw 0            ; base low
    db 0            ; base middle
    db 10010010b    ; access
    db 11001111b    ; granularity
    db 0            ; base high

; 07 00 0C 7C 00 00
GDT_DESC:
    dw (GDT_DESC - GDT - 1)
    dd GDT