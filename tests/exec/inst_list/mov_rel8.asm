BITS 32
    org 0x7c00
    
str1:
    db 'Hello', 10, 0
    mov eax, str1
