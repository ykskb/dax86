BITS 32
    org 0x7c00
start:
    mov edx, 0x03f8
mainloop:
    mov al, '>'
    out dx, al
input:
    in al, dx  
    cmp al, 'h'
    je puthello
    cmp al, 'w'
    je putworld
    cmp al, 'q'
    je fin
    jmp input
puthello:
    mov esi, msghello
    call puts
    jmp mainloop
putworld:
    mov esi, msgworld
    call puts
    jmp mainloop
fin:
    jmp 0
puts:
    mov al, [esi]
    inc esi
    cmp al, 0
    je putsend
    out dx, al
    jmp puts
putsend:
    ret
msghello:
    db "hello", 0x0d, 0x0a, 0
msgworld:
    db "world", 0x0d, 0x0a, 0
