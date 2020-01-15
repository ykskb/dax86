BITS 32
    org 0x7c00
    sub esp, 16 ; 7BF0
    mov ebp, esp ; EBP: 7BF0
    
    mov eax, 0x3 ; EAX: 1
    and al, 0x2 ; 24 ; EAX: 2
    push eax ; ESP -> 7BEC: 0x2 <Assert>
    
    or byte [ebp], al ; 7BF0: 0x2
    and byte [ebp], al ; 20 ; 7BF0: 0x2
    
    mov eax, 0x7 ; EAX: 7
    and byte al, [ebp] ; 22 ; EAX: 0x2
    push eax ; ESP -> 7BE8: 0x2 <Assert>
    
    or dword [ebp+4], 0x7 ; 7BF4: 0x7 (0111)
    and dword [ebp+4], 0x5 ; 83 ; 7BF4: 0x5 (0101)
    mov eax, [ebp+4]
    push eax ; 7BE4: 0x5 <Assert>
    
    mov eax, 0x1
    and dword [ebp+4], eax ; 21 ; 7BF4: 0x1 <Assert>
    
    mov eax, 0x4
    and dword eax, [ebp+4] ; 23 ; EAX: 0x0
    push eax ; ESP -> 7BE0: 0x0 <Assert>
    
    mov eax, 0x1001
    and dword eax, 0x1000 ; 25 ; EAX: 0x1000
    jmp 0