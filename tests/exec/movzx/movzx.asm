BITS 32
    org 0x7c00
    mov ebp, esp
    mov dword [ebp-0x8], 0xFFFFFFFF
    movzx eax, byte [ebp-0x8] ; eax should have 0xFF
    mov dword [ebp-0x4], 0x11111111
    movzx edx, word [ebp-0x4] ; edx should have 0x1111
    jmp 0