BITS 16
    org 0x7c00

load_gdt:
    lgdt [GDT_DESC]
    ; Enable Protected Mode: PE bit on CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 8:pe_enter ; 1000b

BITS 32
pe_enter:
    ; Set data and stack segements.
    mov eax, 0x10 ; 10000b
    mov ds, eax
    mov ss, eax

    mov eax, 0
    mov ebx, 0x10
    mov ecx, 0x11
    call 0x8:0x7C4D ; eax: 0x10 after addEbx
    mov edx, 0x12
    call [indirect] ; eax: 0x21 after addEcx
    add eax, edx ; eax: 0x33
    call far [farAddr] ; eax: 43 after addEbx
    jmp 0:0
addEbx:
    add eax, ebx
    retf
addEcx:
    add eax, ecx
    ret
indirect:
    dd addEcx
farAddr:
    dw 0x8  ; Seg
    dd 0x7C4D   ; Offset
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
GDT_DESC:
    dw (GDT_DESC - GDT - 1)
    dd GDT