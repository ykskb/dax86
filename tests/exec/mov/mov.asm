BITS 32
    org 0x7c00
    ; mov eax, ds ; Tested in seg ;
    mov dword eax, esp
    mov dword edx, 0xF
    ; 7C00 + 0xF * 4 = 7C3C
    lea eax, [eax + edx * 4] ; R: EAX ModRM: 0490 04: [-][-] SIB: 90 = 1001 0000: scale: 2 index: EDX base: EAX
    push eax
    mov dword eax, esp
    mov dword edx, 0xF
    lea eax, [edx + 4] ; ModRM: C2: RM: EDX R: EAX
    push eax
    ; mov es, edx ; Tested in seg ;
    jmp 0