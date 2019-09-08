BITS 32
    org 0x7c00
    sub esp, 16 ; 83
    mov ebp, esp
    mov eax, 2
    mov dword [ebp+4], 5 ; C7
    
    mov esi, [ebp+4] ; 8B
    inc dword [ebp+4] ; FF
    mov edi, [ebp+4] ; 8B
    add word [ebp+4], 5 ; 66 (prefix) 83 rm32 imm8
    add byte [ebp], 5 ; 80 rm8 imm8

    ; start of lists ;
    
    add byte [ebp], ah; 00 add rm8 r8
    add dword [ebp+4], eax ; 01 add rm32 r32
    add byte ah, [ebp] ; 02 add r8 rm8
    add dword eax, [ebp+4] ; 03 add r32, rm32
    add byte al, 3 ; 04 add al imm8
    add dword eax, 0x100000 ; 05 add eax imm32
    
    push es ; 06
    pop es ; 07

    or byte [ebp], ah; 08 or rm8 r8
    or dword [ebp+4], eax ; 09 or rm32 r32
    or byte ah, [ebp] ; 0A or r8 rm8
    or dword eax, [ebp+4] ; 0B or r32, rm32
    or byte al, 3 ; 0C or al imm8
    or dword eax, 0x100000 ; 0D or eax imm32

    push cs ; 0E

    ; 0f: 2-byte instructions

    adc byte [ebp], ah; 10 abc rm8 r8
    adc dword [ebp], eax ; 11 adc rm32 r32
    adc byte ah, [ebp] ; 12 adc r8 rm8
    adc dword eax, [ebp] ; 13 adc r32, rm32
    adc byte al, 3 ; 14 adc al imm8
    adc dword eax, 0x100000 ; 15 adc eax imm32

    push ss ; 16
    pop ss ; 17

    ; <standard idiom in optimized code to check if a edx is zero>
    ; sbb edx, edx ; dst = dst - (src + CF) ; Compiler idiom to isolate CF for branchless code (some prefer setc)
    ; neg edx ; edx = -edx ; -CF: 0 or -1 -> 0 or 1 after neg
    ; test edx, edx ; checks AND and updates flags without affecting any
    ; jz 0x810f31c ; jmp if zero 

    sbb byte [ebp], ah; 18 sbb rm8 r8
    sbb dword [ebp], eax ; 19 sbb rm32 r32
    sbb byte ah, [ebp] ; 1A sbb r8 rm8
    sbb dword eax, [ebp] ; 1B sbb r32, rm32
    sbb byte al, 3 ; 1C sbb al imm8
    sbb dword eax, 0x100000 ; 1D sbb eax imm32

    push ds ; 1E
    pop ds ; 1F

    and byte [ebp], ah; 20 and rm8 r8
    and dword [ebp], eax ; 21 and rm32 r32
    and byte ah, [ebp] ; 22 and r8 rm8
    and dword eax, [ebp] ; 23 and r32, rm32
    and byte al, 3 ; 24 and al imm8
    and dword eax, 0x100000 ; 25 and eax imm32

    ; 26 ES override prefix

    ; 27 daa al ; ?

    sub byte [ebp], ah; 28 sub rm8 r8
    sub dword [ebp], eax ; 29 sub rm32 r32
    sub byte ah, [ebp] ; 2A sub r8 rm8
    sub dword eax, [ebp] ; 2B sub r32, rm32
    sub byte al, 3 ; 2C sub al imm8
    sub dword eax, 0x100000 ; 2D sub eax imm32

    ; 2E CS override prefix

    ; 2F as al ; ?

    xor byte [ebp], ah; 30 xor rm8 r8
    xor dword [ebp], eax ; 31 xor rm32 r32
    xor byte ah, [ebp] ; 32 xor r8 rm8
    xor dword eax, [ebp] ; 33 xor r32, rm32
    xor byte al, 3 ; 34 xor al imm8
    xor dword eax, 0x100000 ; 35 xor eax imm32

    ; 36 SS override prefix

    ; 37 aaa al ah ;  ?

    cmp byte [ebp], ah; 38 cmp rm8 r8
    cmp dword [ebp], eax ; 39 cmp rm32 r32
    cmp byte ah, [ebp] ; 3A cmp r8 rm8
    cmp dword eax, [ebp] ; 3B cmp r32, rm32
    cmp byte al, 3 ; 3C cmp al imm8
    cmp dword eax, 0x100000 ; 3D cmp eax imm32

    ; 3E DS override prefix

    ; 3F aas al ah ; ?

    ; 40 + r inc r32 ;

    ; 48 + r dec r32 ;

    ; 50 + r push r32 ;

    ; 58 + r pop r32 ;

    ; 60 pusha ; push all general register to stack
    ; 61 pushad ;
    ; 62 popa ;
    ; 63 popad ;

    ; 64 FS override prefix ;
    ; 65 GS override prefix ;

    ; 66 operand/precision-size override prefix ;
    ; 67 address-size override prefix ;

    ; 68 push imm32 ; implemented ;

    ; imul https://c9x.me/x86/html/file_module_x86_id_138.html ;
    imul byte [ebp] ; F6/5 imul rm8 ; AX = AL * r/m byte.
    imul word [ebp] ; 66 F7/5 imul rm16 ; DX:AX = AX * r/m word.
    imul dword [ebp] ; 66 F7/5 imul rm32 ; EDX:EAX = EAX * r/m doubleword.
    imul dword eax, [ebp] ; 0F doubleword register = doubleword register * r/m doubleword.
    imul eax, [ebp], 0x10000 ; 69 imul r16 rm16 imm32
    imul eax, [ebp], 3 ; 6B r16 rm16 imm8

    ; 6A push imm8 ; implemented ;

    ; https://c9x.me/x86/html/file_module_x86_id_141.html ;
    insb ; 6C ; input byte from port specified in DX into ES:DI
    insw ; 66 6D ; input word from port specified in DX into ES:DI
    insd ; 6D ; input double word from port specified in DX into ES:DI

    ; https://www.felixcloutier.com/x86/outs:outsb:outsw:outsd;
    outsb ; 6E ; output byte from ES:DI to port specified in DX
    outsw ; 66 6F ; output word from ES:DI to port specified in DX
    outsd ; 6F ; output double word from ES:DI to port specified in DX

    ; 70 ~ 79 implemented ;
    ; 7A

    jmp 0