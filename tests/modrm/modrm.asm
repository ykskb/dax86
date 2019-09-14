BITS 32
    org 0x7c00
    sub esp, 16 ; 7BF0
    mov ebp, esp ; EBP: 7BF0
    mov eax, 2 ; EAX: 2
    add byte [ebp], al ; op: 00 ; 7BF0: 2
    add byte al, [ebp] ; op: 02 ; EAX: 4
    mov dword [ebp+4], 5 ; 7BF4: 5
    add dword [ebp+4], eax ; 7BF4: 9
    add dword eax, [ebp+4] ; EAX : 13
    mov esi, [ebp+4] ; ESI: 9
    inc dword [ebp+4] ; 7BF4: 10
    mov edi, [ebp+4] ; EDI: 10
    ; ModRM: 7c (01: Mod01 111: EDI 100: SIB indicator) ;
    ; SIB: 85 (10: *4, 000: EAX, 101: EBP) ;
    mov edx, 2 ; EDX: 2
    mov [ebp + edx * 4], edi ; 7BF0 + 2 * 4 = 7BF8: 10 ;
    mov [ebp + edx * 4 + 0x1000], edi ; 8BF0: 10 ;
    mov ecx, [ebp + edx * 4 + 0x1000] ; ECX: 10 ;
    jmp 0