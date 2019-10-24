BITS 32
    org 0x7c00
    push 131 ; 0x7BFC: 0x83: 1000 0011 (CF: 0)
    mov eax, esp
    rol byte [eax], 0x2 ; C0 /0 rotate rm8 left imm8 times ; 0000 1110 (0x0E)
    pushfd ; 0x7BF8: 0x0

    push 131 ; 0x7BF4: 1000 0011 (CF: 0)
    mov eax, esp
    rol byte [eax], 0xA ; C0 /0 rotate rm8 left imm8 times ; 0000 1110 (0x0E)
    pushfd ; 0x7BF0: 0x0

    push 131 ; 0x7BEC: 1000 0011 (CF: 0)
    mov eax, esp
    ror byte [eax], 0x2 ; C0 /1 rotate rm8 right imm8 times ; 1110 0000 (0xE0)
    pushfd ; 0x7BE8: 0

    push 163 ; 0x7BE4: 1010 0011 (CF: 0)
    mov eax, esp
    rcl byte [eax], 0x3 ; C0 /2 rotate rm8 left imm8 times including CF ; 0001 1010 (0x1A)
    pushfd ; 0x7BE0: 0x1

    push 131 ; 0x7BDC: 1 1000 0011 (CF: 1)
    mov eax, esp
    rcr byte [eax], 0x3 ; C0 /3 rotate rm8 right imm8 times including CF ; 1111 0000 (0xF0)
    pushfd ; 0x7BD8: 0x0

    push 131; 0x7BD4: 1000 0011 (CF: 0)
    mov eax, esp
    shl byte [eax], 0x2 ; C0 /4 logical shift rm8 left imm8 times ; 0000 1100 (0x0C)
    pushfd ; 0x7BD0: 0x0

    push 131; 0x7BCC: 1000 0011 (CF: 0)
    mov eax, esp
    shr byte [eax], 0x2 ; C0 /5 logical shift rm8 right imm8 times ; 0010 0000 (0x20)
    pushfd ; 0x7BC8: 0x1

    push 131; 0x7BC4: 1000 0011 (CF: 1)
    mov eax, esp
    sar byte [eax], 0x2 ; C0 /7 arithmetic shift rm8 left imm8 times ; 1110 0000 (0xE0)
    pushfd ; 0x7BC0: 0x1

    ; code_c1 ;

    push 131 ; 0x7BBC: 0x83: 0x0 0x0 0x0 1000 0011 (CF: 1)
    mov eax, esp
    rol dword [eax], 25 ; C1/0 ; 0000 0110 0x0 0x0 0000 0001 (0x6000001)
    pushfd ; 0x7BB8: 0x1 (from previous condition)

    push 131 ; 0x7BB4: 0x0 0x0 0x0 1000 0011 (CF: 1)
    mov eax, esp
    rol dword [eax], 34 ; C1/0 ; 0x0 0x0 0000 0010 0000 1100 (0x20C)
    pushfd ; 0x7BB0: 0x1 (from previous condition)

    push 131 ; 0x7BAC: 0x0 0x0 0x0 1000 0011 (CF: 1)
    mov eax, esp
    ror dword [eax], 34 ; C1/ 1 ; 1100 0000 0x0 0x0 0010 0000 (0xC0000020)
    pushfd ; 0x7BA8: 0x1 (from previous condition)

    push 0xA3000000 ; 0x7BA4: 1010 0011 0x0 0x0 0x0 (CF: 1)
    mov eax, esp
    rcl dword [eax], 3 ; C1/2 ; 0001 1000 0x0 0x0 0000 1100 (0x18000006)
    pushfd ; 0x7BA0: 0x1

    push 131 ; 0x7B9C: 0x0 0x0 0x0 1000 0011 (CF: 1)
    mov eax, esp
    rcr dword [eax], 3 ; C1/3 ; 1110 0000 0x0 0x0 0001 0000 (0xE0000010)
    pushfd ; 0x7B98: 0x0

    push 131; 0x7B94: 0x0 0x0 0x0 1000 0011 (CF: 0)
    mov eax, esp
    shl dword [eax], 25 ; C1/4 ; 0000 0110 0x0 0x0 0x0 (0x6000000)
    pushfd ; 0x7B90: 0x1

    push 0x83000000; 0x7B8C: 1000 0011 0x0 0x0 0x0 (CF: 1)
    mov eax, esp
    shr dword [eax], 25 ; C1/5 ; 0x0 0x0 0x0 0100 0001 (0x41)
    pushfd ; 0x7B88: 0x1

    push 0x83000000; 0x7B84: 1000 0011 0x0 0x0 0x0 (CF: 1)
    mov eax, esp
    sar dword [eax], 25 ; C1/7 ; 0xFF 0xFF 0xFF 1100 0001 (0xFFFFFFC1)
    pushfd ; 0x7B80: 0x1

    jmp 0