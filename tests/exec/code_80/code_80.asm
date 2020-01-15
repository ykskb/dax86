BITS 32
    org 0x7c00
    ; code: 80
    push 0
    mov dword eax, esp
    add byte [eax], 3 ; EBP(0x7BFC): 3
    or byte [eax], 4 ; EBP(0x7BFC): 7
    adc byte [eax], 1 ; EBP(0x7BFC): 8
    sbb byte [eax], 1 ; EBP(0x7BFC): 7
    and byte [eax], 15 ; EBP(0x7BFC): 7
    sub byte [eax], 1 ; EBP(0x7BFC): 6 (0110) xor (0100)
    xor byte [eax], 4 ; EBP(0x7BFC): 2
    cmp byte [eax], 3 

    ; code: 83
    push 0
    mov dword eax, esp
    add dword [eax], 0x3000 ; EBP(0x7BF8): 0x3000
    or dword [eax], 0x4000 ; EBP(0x7BF8): 0x7000
    adc dword [eax], 0x1000 ; EBP(0x7BF8): 0x8000
    sbb dword [eax], 0x1000 ; EBP(0x7BF8): 0x7000
    and dword [eax], 0xF000 ; EBP(0x7BF8): 0x5000(0110 0000 x 3) and 0xF000(1111 0000 x 3) = 0x7000
    sub dword [eax], 0x1000 ; EBP(0x7BF8): 0x6000 
    xor dword [eax], 0x4000 ; EBP(0x7BF8): 0x7000(0110 0000 x 3) xor 0x6000(0100 0000 x 3) = 0x2000
    cmp dword [eax], 0x3000

    ; code: 83
    push 0
    mov dword eax, esp
    add dword [eax], 3 ; EBP(0x7BF4): 3
    or dword [eax], 4 ; EBP(0x7BF4): 7
    adc dword [eax], 1 ; EBP(0x7BF4): 8
    sbb dword [eax], 1 ; EBP(0x7BF4): 7
    and dword [eax], 15 ; EBP(0x7BF4): 7
    sub dword [eax], 1 ; EBP(0x7BF4): 6 (0110) xor (0100)
    xor dword [eax], 4 ; EBP(0x7BF4): 2
    cmp dword [eax], 3 

    jmp 0