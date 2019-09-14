BITS 32
label0:
    org 0x7c00
    ; 03 add r32, rm32 ;
    ; 0C mod: 00 rm: 100 r32: 001 ; [-][-][ECX] ;
    ; BB ; 10: 4 (scale) 111: EDI(index) 011: EBX(base)
    mov eax, 0
    mov ebx, esp
    mov edi, 4
    mov dword [ebx + edi * 8], 5
    add dword eax, [ebx + edi * 8] ; 03 0C BB
    jmp 0;
    hlt ; F4 hlt ;