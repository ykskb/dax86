BITS 32
    org 0x7c00
    mov esp, 0
    mov eax, 0x101001
    mov es, ax ; 8EC0 ; ModRM:1100 0000 -> reg_index:000
    
    ; mov eax, 0x1002
    ; mov cs, ax ; 8EC8 ; ModRM:1100 1000 -> reg_index:001
    
    ; changes SS to 0x7BE0
    mov eax, 0x7BE
    mov ss, ax ; 8ED0 ; ModRM:1101 0000 -> reg_index:010
    
    mov eax, 0x1004
    mov ds, ax ; 8ED8 ; ModRM:1101 1000 -> reg_index:011
    
    mov eax, 0x1005
    mov fs, ax ; 8EE0 ; ModRM:1110 0000 -> reg_index:100
    
    mov eax, 0x1006
    mov gs, ax ; 8EE8 ; ModRM:1110 1000 -> reg_index:101
    
    push es
    ; push cs
    push ss
    push ds

    pop es ; ES: 0x1004 from DS
    ; no pop cs ; CS: 0x1002
    pop ds ; DS: 0x7BE from SS
    pop ss ; SS: 0x1001 from ES

    mov eax, ss ; EAX: 0x1001 from ES
    
    jmp 0