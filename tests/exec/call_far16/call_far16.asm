BITS 16
    org 0x7c00
start:
    mov eax, 0
    mov ebx, 0x10
    mov ecx, 0x11
    call 0x7C0:0x2D ; eax: 0x10 after addEbx
    mov edx, 0x12
    call [indirect] ; eax: 0x21 after addEcx
    add eax, edx ; eax: 0x33
    call far [farAddr] ; eax: 43 after addEbx
    jmp 0:0
addEbx:
    add eax, ebx
    retf
addEcx:
    add eax, ecx
    ret
indirect:
    dd addEcx
farAddr:
    dw 0x7C0  ; Seg
    dd 0x2D   ; Offset