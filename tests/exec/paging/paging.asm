BITS 32
    org 0x7c00

load_gdt:
    lgdt [GDT_DESC] ; 0F 01 15 (14 7C 00 00): 15=21: 000 10(op) 101(r/m)
    
    ; Enable Protected Mode: PE bit on CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 8:pe_enter ; GDT index: 1 << 3 = 1000b

pe_enter:
    ; Turn on page size extension for 4Mbyte pages
    mov    eax, cr4
    or     eax, 0x00000010
    mov    cr4, eax
    ; Set page directory
    mov    eax, PDE1
    mov    cr3, eax
    ; Turn on paging.
    mov    eax, cr0
    or     eax, 0x80010000
    mov    cr0, eax
    ; From here EIP will be translated. So we need 2 mappings:
    ; Before jmp (using PSE): PDE[10]: 0 | Offset[22]: 0 = 0x0 + EIP: 0x7C33 => 0x7C33
    ; After jmp (regular paging): PDE[10]: 1 | PTE[10]: 0 | Offset[12]: 2
    ; (0000 0000 0100 0000 0000 0000 0000 0010) = 0x400002
    jmp 8:0x400002

pg_enter:    
    mov dword eax, 0x10 ; 10000b
    mov ss, ax
    push 0xFFFF ; ESP: 0x7c00 => identical mapping (PDE1)
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

GDT_DESC:
    dw (GDT_DESC - GDT - 1)
    dd GDT

PDE1:
    ; 31_____________________________12_11_____9____________________________________0
    ; | Page table 4kb-aligned address | Avail. | G | S | 0 | A | D | W | U | R | P |
    ; |_____________________________________________________________________________|
    ; *Identical Mapping
    ; 0x0000 008F
    dw 0x8F
    dw 0x0

PDE2:
    ; Pointing to PGT
    dw 0x300F
    dw 0x7C7

; 0x7C73
PGT:
    ; 31_____________________________12_11_____9_______________________________________0
    ; | Page table 4kb-aligned address | Avail. | G* | 0 | D* | A | C* | W | U | R | P |
    ; |________________________________________________________________________________|
    dw 0x800F
    dw 0x7C3 ; 7C38 + 2 = pg_enter (0x7c3A)
    ; on memory: 0f80 c307
    ; get_memory32 at 0x7c73: 07C3 800F
