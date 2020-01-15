BITS 32
    org 0x7c00
    sub esp, 16 ; 7BF0
    mov ebp, esp ; EBP: 7BF0
    
    mov eax, 0x1 ; EAX: 1
    xor al, 0x9 ; op: 34 EAX: 8
    push eax ; ESP -> 7BEC: 0x8 <Assert>
    
    mov byte [ebp], 0xF
    xor byte [ebp], al ; op: 30 ; 7BF0: 0x7 <Assert>
    
    mov eax, 0x2 ; EAX: 2
    xor byte al, [ebp] ; op: 32 ; EAX: 0x5
    push eax ; ESP -> 7BE8: 0x5 <Assert>
    
    mov byte [ebp+4], 0x3
    xor dword [ebp+4], 0x1 ; op: 83 ; 7BF4: 0x2 
    
    mov eax, 0x3
    xor dword [ebp+4], eax ; op: 31 ; 7BF4: 0x1 <Assert>
    
    mov eax, 0x5
    xor dword eax, [ebp+4] ; op: 33 ; EAX: 0x4
    push eax ; ESP -> 7BE4: 0x4 <Assert>
    
    xor dword eax, 0x1004 ; op: 35 ; EAX: 0x1000
    jmp 0