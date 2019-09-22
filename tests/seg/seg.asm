BITS 32
    org 0x7c00
    mov eax, 0x101009
    mov es, ax ; 8EC0 ; ModRM:1100 0000 -> reg_index:000
    mov eax, 0x8
    mov cs, ax ; 8EC8 ; ModRM:1100 1000 -> reg_index:001
    mov eax, 0x1007
    mov ss, ax ; 8ED0 ; ModRM:1101 0000 -> reg_index:010
    mov eax, 0x1006
    mov ds, ax ; 8ED8 ; ModRM:1101 1000 -> reg_index:011
    mov eax, 0x1005
    mov fs, ax ; 8EE0 ; ModRM:1110 0000 -> reg_index:100
    mov eax, 0x1004
    mov gs, ax ; 8EE8 ; ModRM:1110 1000 -> reg_index:101
    ; push es
    jmp 0