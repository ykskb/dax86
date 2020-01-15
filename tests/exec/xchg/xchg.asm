BITS 32
    org 0x7C00
    mov dword eax, 3
    push 1
    mov edx, esp
    xchg byte [edx], al ; 0x7BFC: 3

    mov dword eax, 4
    push 2 ; ESP: 0x7BFC
    mov edx, esp
    xchg dword [edx], eax ; 0x7BF8: 4

    mov dword eax, 5
    mov dword ecx, 6
    xchg dword ecx, eax
    push eax ; 0x7BF4: 6
    push ecx ; 0x7BF0: 5

    mov dword eax, 7
    mov dword edx, 8
    xchg dword edx, eax
    push eax ; 0x7BFC: 8
    push edx ; 0x7BE8: 7

    mov dword eax, 9
    mov dword ebx, 10
    xchg dword eax, ebx
    push eax ; 0x7BE4: 10
    push ebx ; 0x7BE0: 9

    ; xchg dword eax, eax ; 90: nop ;

    mov dword eax, 11
    mov dword ebx, 12
    xchg eax, ebx
    push eax ; 0x7BDC: 12
    push ebx ; 0x7BD8: 11

    mov dword eax, 13
    mov dword ecx, 14
    xchg eax, ecx
    push eax ; 0x7BD4: 14
    push ecx ; 0x7BD0: 13

    mov dword eax, 15
    mov dword edx, 16
    xchg eax, edx
    push eax ; 0x7BCC: 16
    push edx ; 0x7BC8: 15

    mov dword eax, 17
    mov dword edi, 18
    xchg eax, edi
    push eax ; 0x7BC4: 18
    push edi ; 0x7BC0: 17

    mov dword eax, 19
    mov dword esi, 20
    xchg eax, esi
    push eax ; 0x7BBC: 20
    push edi ; 0x7BB8: 19

    mov dword eax, 21
    mov dword ebp, 22
    xchg eax, ebp
    push eax ; 0x7BB4: 22
    push ebp ; 0x7BB0: 21

    mov dword eax, 0x7BB0
    mov dword esp, 24
    xchg eax, esp
    push eax ; 0x7BAC: 24
    push esp ; 0x7BA8: 7BAC

    ; mov ax, 9 ; 66 B8
    ; mov bx, 10 ; 66 B8
    ; xchg bx, ax ; 66 93
    ; push ax ; 66 50
    ; push bx ; 66 53
    jmp 0

