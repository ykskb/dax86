BITS 32
    org 0x7c00
    sub esp, 16 ; 7BF0
    mov ebp, esp ; EBP: 7BF0
    
    mov eax, 0x1 ; EAX: 1
    or al, 0x8 ; op: 0C EAX: 9
    push eax ; ESP -> 7BEC: 0x9 <Assert>
    
    or byte [ebp], al ; op: 08 ; 7BF0: 0x9 <Assert>
    
    mov eax, 0x2 ; EAX: 2
    or byte al, [ebp] ; op: 0A ; EAX: 0xB
    push eax ; ESP -> 7BE8: 0xB <Assert>
    
    or dword [ebp+4], 0x1 ; op: 83 ; 7BF4: 0x1 <Assert>
    
    mov eax, 0x2
    or dword [ebp+4], eax ; op: 09 ; 7BF4: 0x3 <Assert>
    
    mov eax, 0x4
    or dword eax, [ebp+4] ; op: 0B ; EAX: 0x7
    push eax ; ESP -> 7BE4: 0x7 <Assert>
    
    or dword eax, 0x1000 ; op: 0D ; EAX: 0x1007
    jmp 0