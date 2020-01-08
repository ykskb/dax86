BITS 32
    org 0x7c00

    ; strncomp ;
    
    ; check the length of msghello
    mov edx, 32
    mov ecx, edx
    mov edi, str_marilyn
    repne scasd ; EAX - ES:EDI -> update flags
    sub edx, ecx
    inc edx
    push edx ; 0x02 (length of marilyn by 4 bytes)

    ; compare same chars
    mov ecx, edx
    mov edi, str_marilyn
    mov esi, str_marilyn
    rep cmpsd ; DS:ESI - ES:EDI -> update flags
    push ecx ; 0x00 (rep went through all letters: equal)

    ; compare diff chars
    mov ecx, edx
    mov edi, str_marilyn
    mov esi, str_mariah
    rep cmpsd
    push ecx ; 0x01 (rep stops at difference: not equal)

    ; write strings ;

    mov ecx, 1
    mov esi, str_cup
    mov edi, str_abc
    rep movsd ; DS:ESI to ES:EDI

    ; initialize buffer ;

    mov eax, 0xFFFFFFFF
    mov edi, str_def
    mov ecx, 1
    rep stosd ; AL to ES:DI/EDI

    ; load a letter to AL ;

    mov esi, str_cup
    lodsd ; DS:SI/ESI to AL ; EAX: 0x68

    jmp 0

str_marilyn:
    db "marilyn", 0x0a, 0, 0, 0, 0; 7C5D: 6d 61 72 69 6c 79 6e 0a 00 00 00 00

str_mariah:
    db "mariah", 0x0a, 0x0a, 0, 0, 0, 0 ; 7C69: 6d 61 72 69 61 68 0a 0a 00 00 00 00

str_cup:
    db "cup", 0 ; 7C75: 63 75 70 00

str_abc:
    db "abc", 0 ; 7C79: 61 62 63 00 -> 63 75 70 00

str_def:
    db "def", 0 ; 7C7D: 64 65 66 00 -> 01 01 01 00