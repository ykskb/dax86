BITS 32
    org 0x7c00
    sub esp, 16 ; 83 ; ESP: 7BF0
    mov ebp, esp ; EBP: 7BF0
    mov eax, 10 ; EAX: 10
    sub al, 2 ; 2C ; EAX: 8
    mov byte [ebp], 10
    sub byte [ebp], al ; 28 ; 7BF0: 2
    sub byte al, [ebp] ; 2A ; EAX: 6
    push eax ; 7BEC: 6
    mov dword [ebp+4], 10 ; 7BF4: 10
    sub dword [ebp+4], eax ; 29 ; 7BF4: 4
    sub dword eax, [ebp+4] ; 2B ; EAX : 2
    sub dword eax, 0x1000 ; 2D ; 2 - 0x1000 = 4096 ; EAX: -4094
    jmp 0