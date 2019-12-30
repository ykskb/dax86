BITS 32
    org 0x7c00
    ; moffs8
    mov eax, 0x7BA
    mov ds, eax
    mov eax, 0
    mov al, 0xFF
    mov byte [0x0], al
    mov byte al, 0x11 ; to be overwritten
    push eax
    mov byte al, [0x0]
    push eax ; 0x7BFC: 0xFF

    ; moff32

    mov eax, 0xEEEEEEEE
    mov dword [0x2], eax
    mov dword eax, 0x11111111
    push eax
    mov dword eax, [0x2]
    push eax
    jmp 0