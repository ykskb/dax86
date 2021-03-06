BITS 32
label0:
    org 0x7c00
    
    ; implemented ;
    add byte [eax], ah; 00 add rm8 r8
    add dword [ebp+4], eax ; 01 add rm32 r32
    add byte ah, [ebp] ; 02 add r8 rm8
    add dword eax, [ebp+4] ; 03 add r32, rm32
    add byte al, 3 ; 04 add al imm8
    add dword eax, 0x100000 ; 05 add eax imm32
    
    push es ; 06 ; implemented ;
    pop es ; 07 ; implemented ;

    ; implemented ;
    or byte [ebp], ah; 08 or rm8 r8
    or dword [ebp+4], eax ; 09 or rm32 r32
    or byte ah, [ebp] ; 0A or r8 rm8
    or dword eax, [ebp+4] ; 0B or r32, rm32
    or byte al, 3 ; 0C or al imm8
    or dword eax, 0x100000 ; 0D or eax imm32

    push cs ; 0E

    ; 0f: 2-byte instructions

    ; implemented ;
    adc byte [ebp], ah; 10 adc rm8 r8
    adc dword [ebp], eax ; 11 adc rm32 r32
    adc byte ah, [ebp] ; 12 adc r8 rm8
    adc dword eax, [ebp] ; 13 adc r32, rm32
    adc byte al, 3 ; 14 adc al imm8
    adc dword eax, 0x100000 ; 15 adc eax imm32

    push ss ; 16 ; implemented ;
    pop ss ; 17 ; implemented ;

    ; <standard idiom in optimized code to check if a edx is zero>
    ; sbb edx, edx ; dst = dst - (src + CF) ; Compiler idiom to isolate CF for branchless code (some prefer setc)
    ; neg edx ; edx = -edx ; -CF: 0 or -1 -> 0 or 1 after neg
    ; test edx, edx ; checks AND and updates flags without affecting any
    ; jz 0x810f31c ; jmp if zero 

    ; implemented ;
    sbb byte [ebp], ah; 18 sbb rm8 r8
    sbb dword [ebp], eax ; 19 sbb rm32 r32
    sbb byte ah, [ebp] ; 1A sbb r8 rm8
    sbb dword eax, [ebp] ; 1B sbb r32, rm32
    sbb byte al, 3 ; 1C sbb al imm8
    sbb dword eax, 0x100000 ; 1D sbb eax imm32

    push ds ; 1E ; implemented ;
    pop ds ; 1F ; implemented ;

    ; implemented ;
    and byte [ebp], ah; 20 and rm8 r8
    and dword [ebp], eax ; 21 and rm32 r32
    and byte ah, [ebp] ; 22 and r8 rm8
    and dword eax, [ebp] ; 23 and r32, rm32
    and byte al, 3 ; 24 and al imm8
    and dword eax, 0x100000 ; 25 and eax imm32

    ; 26 ES override prefix

    ; 27 daa al ; ?

    ; implemented ;
    sub byte [ebp], ah; 28 sub rm8 r8
    sub dword [ebp], eax ; 29 sub rm32 r32
    sub byte ah, [ebp] ; 2A sub r8 rm8
    sub dword eax, [ebp] ; 2B sub r32, rm32
    sub byte al, 3 ; 2C sub al imm8
    sub dword eax, 0x100000 ; 2D sub eax imm32

    ; 2E CS override prefix

    ; 2F as al ; ?

    ; implemented ;
    xor byte [ebp], ah; 30 xor rm8 r8
    xor dword [ebp], eax ; 31 xor rm32 r32
    xor byte ah, [ebp] ; 32 xor r8 rm8
    xor dword eax, [ebp] ; 33 xor r32, rm32
    xor byte al, 3 ; 34 xor al imm8
    xor dword eax, 0x100000 ; 35 xor eax imm32

    ; 36 SS override prefix

    ; 37 aaa al ah ;  ?

    ; implemented ;
    cmp byte [ebp], ah; 38 cmp rm8 r8
    cmp dword [ebp], eax ; 39 cmp rm32 r32
    cmp byte ah, [ebp] ; 3A cmp r8 rm8
    cmp dword eax, [ebp] ; 3B cmp r32, rm32
    cmp byte al, 3 ; 3C cmp al imm8
    cmp dword eax, 0x100000 ; 3D cmp eax imm32

    ; 3E DS override prefix

    ; 3F aas al ah ; ?

    ; 40 + r inc r32 ; implemented ;

    ; 48 + r dec r32 ; implemented ;

    ; 50 + r push r32 ; implemented ;

    ; 58 + r pop r32 ; implemented ;

    pusha ; 60 pusha ; push all general register to stack ; implemented ;
    pushad ; 61 pushad ; implemented ;
    popa ; 62 popa ;
    popad ; 63 popad ;

    ; 64 FS override prefix ;
    ; 65 GS override prefix ;

    ; 66 operand/precision-size override prefix ;
    ; 67 address-size override prefix ;

    ; 68 push imm32 ; implemented ;

    ; imul https://c9x.me/x86/html/file_module_x86_id_138.html ;
    imul byte [ebp] ; F6/5 imul rm8 ; AX = al * r/m byte. ; implemented ;
    imul word [ebp] ; 66 F7/5 imul rm16 ; DX:AX = AX * r/m word.
    imul dword [ebp] ; 66 F7/5 imul rm32 ; EDX:EAX = EAX * r/m doubleword. ; implemented ;
    imul dword eax, [ebp] ; 0F doubleword register = doubleword register * r/m doubleword.
    imul eax, [ebp], 0x10000 ; 69 imul r16 rm16 imm32 ; implemented ;
    imul eax, [ebp], 3 ; 6B r16 rm16 imm8 ; implemented ;

    ; 6A push imm8 ; implemented ;

    ; https://c9x.me/x86/html/file_module_x86_id_141.html ;
    insb ; 6C ; input byte from port specified in DX into ES:DI
    insw ; 66 6D ; input word from port specified in DX into ES:DI
    insd ; 6D ; input double word from port specified in DX into ES:DI

    ; https://www.felixcloutier.com/x86/outs:outsb:outsw:outsd;
    outsb ; 6E ; output byte from ES:DI to port specified in DX
    outsw ; 66 6F ; output word from ES:DI to port specified in DX
    outsd ; 6F ; output double word from ES:DI to port specified in DX

    ; 70 ~ 79, 7C, 7E implemented ;
    ; 7A JP/JPE jump short if parity is even (PF=1)
    ; 7B JNP/JPO jump short if parity is odd (PF=0)
    ; 7D JNL/JGE jump short if not less/greater or equal (SF=OF)
    ; 7F JNLE/JG jump short if not less nor equal/greater ((ZF=0) AND (SF=OF))

    ; 80 + n ; rm8 imm8 pattern ; implemented (test needed) ;
    add byte [eax], 3
    or byte [eax], 3
    adc byte [eax], 3
    sbb byte [eax], 3
    and byte [eax], 3
    sub byte [eax], 3
    xor byte [eax], 3
    cmp byte [eax], 3

    ; 81 + n ; rm32 imm32 pattern ; implemented (test needed) ;
    add dword [eax], 0x1000
    or dword [eax], 0x1000
    adc dword [eax], 0x1000
    sbb dword [eax], 0x1000
    and dword [eax], 0x1000
    sub dword [eax], 0x1000
    xor dword [eax], 0x1000
    cmp dword [eax], 0x1000

    ; 82 aliases to 80 pattern ;
    ; https://stackoverflow.com/questions/41791470/assembler-what-is-seventh-bit-in-sub-instruction-used-for ;

    ; 83 + n ; rm32 imm8 pattern ; implemented (test needed) ;
    add dword [eax], 3
    or dword [eax], 3
    adc dword [eax], 3
    sbb dword [eax], 3
    and dword [eax], 3
    sub dword [eax], 3
    xor dword [eax], 3
    cmp dword [eax], 3

    test byte [eax], al ; 84 test rm8 r8 ; implemented ;
    test dword [eax], eax ; 85 test rm32 r32 ; implemented ;

    xchg byte [eax], al ; 86 xchg rm8 r8 ; implemented ;
    xchg dword [eax], eax ; 87 xchg rm32 r32 ; implemented ;

    mov byte [eax], al ; 88 mov rm8 r8 ; implemented ;
    mov dword [eax], eax ; 89 mov rm32 r32 ; implemented ;
    mov byte al, [eax] ; 8A mov r8 rm8 ; implemented ;
    mov dword eax, [eax] ; 8B mov r32 rm32 ; implemented ;

    mov eax, ds ; 8C mov rm32 sreg ; implemented ;

    lea eax, [eax] ; 8D lea r32 m ; implemented ;

    mov ds, eax ; 8E mov sreg rm32 ; implemented ;

    pop dword [eax] ; 8F pop rm32 ; implemented ;

    xchg dword eax, ebp ; 90 + r ; xchg r32 eax ; implemented ;

    ; F3 90 ; Pause ?

    cbw ; 66 98 cbw ax, al
    cwde ; 98 cwde eax, ax ; implemented ;
    cwd ; 66 99 cwd dx, ax
    cdq ; 99 cdq edx, eax ; implemented ;

    ; https://stackoverflow.com/questions/20264452/x86-function-call-types ;
    call dword 0x10:0x1000 ; 9A call far ptr16:16/32 (cs: addr); absolute far jump ; implemented ;

    ; https://www.felixcloutier.com/x86/wait:fwait ;
    fwait ; prefix 9B checks for and handle pending, unmasked, floating-point exceptions before proceeding. ;

    pushf ; 9C pushf ; pushes flags register to stack ; implemented ;
    pushfd ; 9C pushfd ; pushes eflags register to stack ; implemented ;

    popf ; 9D popf ; pops stack to flags register ; implemented ;
    popfd ; 9D popfd ; pops stack to eflags register ; implemented ;

    sahf ; 9E sahf ; stores ah into flags ; implemented ;
    lahf ; 9F lahf ; load flags to ah register ; implemented ;

    ; offset to segment base ;
    mov byte al, [0x10] ; A0 mov al moffs8 ; implemented ;
    mov dword eax, [0x10] ; A1 mov eax moffs32 ; implemented ;
    mov byte [0x10], al ; A2 mov moffs8 al ; implemented ;
    mov dword [0x10], eax ; A3 mov moffs32 eax ; implemented ;

    ; move data at address DS:(E)SI to address ES:(E)DI ;
    movsb ; A4 movsb ; implemented ;
    movsw ; 66 A5 movsw ;
    movsd ; A5 movsd ; implemented ;

    ; compares data at address DS:(E)SI with data at address ES:(E)DI and sets the status flags ;
    cmpsb ; A6 cmpsb ; implemented ;
    cmpsw ; 66 A7 cmpsw ;
    cmpsd ; A7 cmpsd ; implemented ;

    test al, 0x1 ; A8 test al imm8 ; implemented ;
    test eax, 0x1 ; A9 test eax imm32 ; implemented ;

    ; stores al/AX/EAX at address ES:(E)DI ;
    stosb ; AA stosb ; implemented ;
    stosw ; 66 AB stosw ;
    stosd ; AB stosd ; implemented ;

    ; loads data at address DS:(E)SI into AL/AX/EAX ;
    lodsb ; AC lodsb ; implemented ;
    lodsw ; 66 AD lodsw ;
    lodsd ; AD lodsd ; implemented ;

    ; compare AL/AX/EAX with byte at ES:(E)DI and set status flags ;
    scasb ; AE scasb ; implemented ;
    scasw ; 66 AF scasw ;
    scasd ; AF scasd ; implemented ;

    ; 0xB0 ~ 0xB7 mov r8 imm8 implemented ; op code includes 8 registers in 1 byte ;
    ; 0xB8 ~ 0xBF mov r32 imm32 implemented ; op code includes 8 registers in 1 byte ;

    ; implemented ;
    ; https://stackoverflow.com/questions/10395071/what-is-the-difference-between-rcr-and-ror ;
    rol byte [eax], 0x10 ; C0 /0 rotate rm8 left imm8 times ;
    ror byte [eax], 0x10 ; C0 /1 rotate rm8 right imm8 times ;
    rcl byte [eax], 0x10 ; C0 /2 rotate rm8 left imm8 times including CF ;
    rcr byte [eax], 0x10 ; C0 /3 rotate rm8 right imm8 times including CF ;

    shl byte [eax], 0x10 ; C0 /4 logical shift rm8 left imm8 times ;
    shr byte [eax], 0x10 ; C0 /5 logical shift rm8 right imm8 times ;
    sar byte [eax], 0x10 ; C0 /7 arithmetic shift rm8 left imm8 times ;

    ; implemented ;
    rol dword [eax], 0x10 ; C1 /0 rotate rm32 left imm8 times ;
    ror dword [eax], 0x10 ; C1 /1 rotate rm32 right imm8 times ;
    rcl dword [eax], 0x10 ; C1 /2 rotate rm32 left imm8 times including CF ;
    rcr dword [eax], 0x10 ; C1 /3 rotate rm32 right imm8 times including CF ;

    shl dword [eax], 0x10 ; C1 /4 logical shift rm32 left imm8 times ;
    shr dword [eax], 0x10 ; C1 /5 logical shift rm32 right imm8 times ;
    sar dword [eax], 0x10 ; C1 /7 arithmetic shift rm32 left imm8 times ;

    ret 0x1000 ; C2 ret imm16 Near return to calling procedure and pop imm16 bytes from stack ; used?
    ; C3 ret implemented. ;

    ; implemented ;
    les dword eax, [0x1000] ; C4 les r32 imm16/32 ; load ES:r32 with far pointer at memory ;
    les word ax, [0x1000] ; 66 C4 less r16 imm16 ; load ES:r16 with far pointer at memory ;

    ; implemented ;
    lds dword eax, [0x1000] ; C5 les r32 imm16/32 ; load DS:r32 with far pointer at memory ;
    lds word ax, [0x1000] ; 66 C5 less r16 imm16 ; load DS:r16 with far pointer at memory ;

    mov byte [eax], 0x1 ; C6 ; mov rm8 imm8 ; implemented ;
    ; C7 mov rm32 imm32 ; implemented ;

    enter 0x1000, 0x10 ; C8 enter imm16 imm8 ; make stack frame https://www.felixcloutier.com/x86/enter ; obsolete
    ; C9 leave ; implemented ;

    ; far return: inter-segment return ;
    retf 0x1000 ; CA retf imm16; far return from procedure ; used?
    retf ; CB retf ; far return from procedure ; implemented ;

    int3 ; CC int3 ; for use by debuggers to temporarily replace an instruction in order to set a code breakpoint
    int 0x10 ; CD int ; calls interrupt or exception handler specified with the destination operand: interrupt vector 0~255
    into ; CE into ; calls overflow exception (#OF), vector number 4 which checks the OF of EFLAGS and calls the overflow interrupt handler if the OF 1

    ; returns program control from an exception or interrupt handler to a program ;
    iret ; CF iret ; interrupt return with operand size of 16bit
    iretd ; CF iretd ; interrupt return with operand size of 32bit

    ; implemented ;
    rol byte [eax], 0x1 ; D0 /0 rotate rm8 left once ;
    ror byte [eax], 0x1 ; D0 /1 rotate rm8 right once ;
    rcl byte [eax], 0x1 ; D0 /2 rotate rm8 left once including CF ;
    rcr byte [eax], 0x1 ; D0 /3 rotate rm8 right once including CF ;

    shl byte [eax], 0x1 ; D0 /4 logical shift rm8 left once ;
    shr byte [eax], 0x1 ; D0 /5 logical shift rm8 right once ;
    sar byte [eax], 0x1 ; D0 /7 arithmetic shift rm8 left once ;

    ; implemented ;
    rol dword [eax], 0x1 ; D1 /0 rotate rm32 left once ;
    ror dword [eax], 0x1 ; D1 /1 rotate rm32 right once ;
    rcl dword [eax], 0x1 ; D1 /2 rotate rm32 left once including CF ;
    rcr dword [eax], 0x1 ; D1 /3 rotate rm32 right once including CF ;

    shl dword [eax], 0x1 ; D1 /4 logical shift rm32 left once ;
    shr dword [eax], 0x1 ; D1 /5 logical shift rm32 right once ;
    sar dword [eax], 0x1 ; D1 /7 arithmetic shift rm32 left once ;

    ; implemented ;
    rol byte [eax], cl ; D2 /0 rotate rm8 left cl times ;
    ror byte [eax], cl ; D2 /1 rotate rm8 right cl times ;
    rcl byte [eax], cl ; D2 /2 rotate rm8 left cl times including CF ;
    rcr byte [eax], cl ; D2 /3 rotate rm8 right cl times including CF ;

    shl byte [eax], cl ; D2 /4 logical shift rm8 left cl times ;
    shr byte [eax], cl ; D2 /5 logical shift rm8 right cl times ;
    sar byte [eax], cl ; D2 /7 arithmetic shift rm8 left cl times ;

    ; implemented ;
    rol dword [eax], cl ; D3 /0 rotate rm32 cl times ;
    ror dword [eax], cl ; D3 /1 rotate rm32 right cl times ;
    rcl dword [eax], cl ; D3 /2 rotate rm32 left cl times including CF ;
    rcr dword [eax], cl ; D3 /3 rotate rm32 right cl times including CF ;

    shl dword [eax], cl ; D3 /4 logical shift rm32 left cl times ;
    shr dword [eax], cl ; D3 /5 logical shift rm32 right cl times ;
    sar dword [eax], cl ; D3 /7 arithmetic shift rm32 left cl times ;

    ; D4 AAM ;
    ; D4 AMX ;
    ; D5 AAD ;
    ; D5 ADX ;

    ; D6 undefined no UD ;

    salc ; D6 salc ; set al if carry ;

    xlat ; D7 xlat m8 ; sets AL to memory byte DS:[(E)BX + unsigned AL]
    xlatb ; D7 xlatb ; sets AL to memory byte DS:[(E)BX + unsigned AL]

    ; D8 ~ DF ; floating point instructions
label1:
    loopnz label1 ; E0 loopnz rel8 ; Decrement count; jump short if count != 0 and ZF=0. ; implemented ;
    loopne label1 ; E0 loopne rel8; identical to loopnz ;

    loopz label1 ; E1 loopz rel8 ; Decrement count; jump short if count != 0 and ZF=1. ; implemented ;
    loope label1 ; E1 loope rel8 ; idential to loopz

    loop label1 ; E2 loop rel8 ; Decrement count; jump short if count != 0. ; implemented ;

    jecxz label1 ; E3 jecsx rel8 ; jumps short if ECX is 0 ; implemented ;
    jcxz label1 ; 67 E3 jcxz rel8 ; jumps short if CX is 0

    in al, 0x10 ; E4 in al imm8 ; inputs byte from imm8 I/O port address into AL ; used?
    in eax, 0x10 ; E5 in eax imm8 ; inputs byte from imm8 I/O port address into EAX ; used?

    out 0x10, al ; E6 out imm8 al ; outputs byte from AL to imm8 I/O port ; used?
    out 0x10, eax ; E7 out imm8 eax ; outputs byte from EAX to imm8 I/O port ; used?

    call label0 ; E8 call rel32 ; implemented ;
    jmp label0 ; E9 jmp rel32 ; implemented ;
    jmp 0x10:0x1000 ; EA jmp ptr16:16/32 ; implemented ;
    jmp label1 ; EB jmp rel8 ; implemented

    in al, dx ; EC in al dx ; inputs byte from dx I/O port address into AL ; implemented ;
    in eax, dx ; ED in eax dx ; inputs byte from dx I/O port address into EAX ; implemented ;

    out dx, al ; EE out dx al ; outputs byte from AL to dx I/O port ; implemented ;
    out dx, eax ; EF out dx eax ; outputs byte from EAX to dx I/O port ; implemented ;

    ; F0 lock prefix ;

    int1 ; F1 int1 ; generates debug trap ;
    icebp ; identical to int1

    ; rep string ops https://c9x.me/x86/html/file_module_x86_id_279.html ;
    
    ; terminates if ECX=0 or ZF=1 ;
    repnz insb ; F3 repnz prefix ; implemented (insb) ;
    repne insb ; F2 repne ; identical to repnz ; implemented (insb) ;
    
    ; terminates if ECX=0 or ZF=0 ;
    repz insb ; F3 repz prefix ; implemented (insb) ;
    repe insb ; F3 repe prefix ; implemented (insb);
    
    ; terminates if ECX=0 ;
    rep insb ; F3 rep preifx implemented ; inputs (E)CX bytes from port DX into ES:[(E)DI] ;

    cmc ; F5 cmc ; reverses carry flag ; implemented ;

    test byte [eax], 0x10 ; F6 0|1 test rm8 imm8 ; implemented ;
    not byte [eax], 0x10 ; F6 2 not rm8 imm8 ; implemented ;
    neg byte [eax], 0x10 ; F6 3 neg rm8 imm8 ; implemented ;
    mul byte [eax] ; F6 4 mul ax al rm8 ; implemented ;
    imul byte [eax] ; F6 5 imul ax al rm8 ; implemented ;
    div byte [eax] ; F6 6 div al ah ax rm8 ; implemented ;
    idiv byte [eax] ; F6 7 idiv al ah ax rm8 ; implemented ;

    test dword [eax], 0x1000 ; F7 0|1 test rm32 imm32 ; implemented ;
    not dword [eax] ; F7 not rm32 ; implemented ;
    neg dword [eax] ; F7 neg rm32 ; implemented ;
    mul dword [eax] ; F7 mul edx dax rm32 ; implemented ;
    imul dword [eax] ; F7 imul edx dax rm32 ; implemented ;
    div dword [eax] ; F7 div edx dax rm32 ; implemented ;
    idiv dword [eax] ; F7 idiv edx dax rm32 ; implemented ;

    clc ; F8 clc ; clears carry flag ; implemented ;
    stc ; F9 stc ; sets carry flag ; implemented ;

    ; FA ~ FD interrupt & direction flags ; implemented ;

    inc byte [eax] ; FE inc rm8 ; implemented ;
    dec byte [eax] ; FE dec rm8 ; implemented ;
    
    ; FF 0 inc rm32 implemented ;
    dec dword [eax] ; FF /1 dec rm32 ; implemented ;
    call [eax]; FF /2 call rm32 ; implemented ;
    call [0x1000] ; FF /3 call m16:16/32 ; implemented ;
    jmp [eax] ; FF /4 jmp rm32 ; implemented ;
    jmp far [0x1000] ; FF /5 jmp m16:16/32 ; impemented ;
    push dword [eax] ; FF /6 push rm32 ; implemented ;

    hlt ; F4 hlt ;