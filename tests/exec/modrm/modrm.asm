BITS 32
    org 0x7c00
    sub esp, 16 ; 7BF0
    mov ebx, esp ; EBP: 7BF0
    mov eax, 0 ; EAX: 0
    add al, 2 ; EAX: 1
    add byte [ebx], al ; op: 00 ; 7BF0: 2
    add byte al, [ebx] ; op: 02 ; EAX: 4
    mov dword [ebx+4], 5 ; 7BF4: 5
    add dword [ebx+4], eax ; 7BF4: 9
    add dword eax, [ebx+4] ; EAX : 13
    mov esi, [ebx+4] ; ESI: 9
    inc dword [ebx+4] ; 7BF4: 10
    mov edi, [ebx+4] ; EDI: 10
    ; ModRM: 7C (01: Mod-01 111: Reg-EDI 100: SIB indicator) ;
    ; SIB: 95 (10: *4, 010: EDX, 101: EBP) ;
    mov edx, 2 ; EDX: 2
    mov [ebx + edx * 4], edi ; op: 89 ; 7BF0(EBP) + 2(EDX) * 4 = 7BF8: 10 ;
    mov [ebx + edx * 4 + 0x1000], edi ; 8BF0: 10 ;
    mov ecx, [ebx + edx * 4 + 0x1000] ; ECX: 10 ;
    add dword eax, 0x1000 ; EAX: 1
    

    inc byte [ebx - 4] ; 7BF0(EBP) - 4 = 7BEC: 01
    inc byte [ebx - 4]
    dec byte [ebx - 4]

    inc dword [ebx - 8] ; 7BF0 (EBP)-8 = 7BE8: 01
    inc dword [ebx - 8]
    dec dword [ebx - 8]

    sub esp, 8 ; 7BF0 (EBP) - 8 = 7BE8
    push dword [ebx - 8] ; FF/6 ; 7BE4: 01
    
    jmp 0