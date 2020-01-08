BITS 32
    org 0x7c00

    ; strncomp ;
    
    ; check the length of msghello
    mov edx, 32
    mov ecx, edx
    les dword edi, [addr_to_str_hello]
    repne scasb ; AL - ES:EDI -> update flags
    sub edx, ecx
    inc edx
    push edx ; 0x08 (length of str_hello)

    ; compare same chars
    mov ecx, edx
    les dword edi, [addr_to_str_hello]
    lds dword esi, [addr_to_str_hello]
    rep cmpsb ; DS:ESI - ES:EDI -> update flags
    push ecx ; 0x00 (rep went through all letters: equal)

    ; compare diff chars
    mov ecx, edx
    les dword edi, [addr_to_str_hello]
    lds dword esi, [addr_to_str_hell]
    rep cmpsb
    push ecx ; 0x04 (rep stops at difference: not equal)

    ; write strings ;

    mov ecx, 4
    mov esi, str_hell
    mov edi, str_abcd
    rep movsb ; DS:ESI to ES:EDI

    ; initialize buffer ;

    mov al, 1
    mov edi, str_defg
    mov ecx, 4
    rep stosb ; AL to ES:DI/EDI

    ; load a letter to AL ;

    mov esi, str_hell
    lodsb ; DS:SI/ESI to AL ; EAX: 0x68

    jmp 0

str_hello:
    db "hello", 0x0d, 0x0a, 0 ; 7C57: 68 65 6C 6C 6F 0D 0A 00

str_hell:
    db "hell", 0 ; 7C67: 68 65 6c 6c 00

str_abcd:
    db "abcd", 0 ; 7C6C: 61 62 63 64 00 -> 68 65 6c 6c 00

str_defg:
    db "defg", 0 ; 7C71: 64 65 66 67 00 -> 01 01 01 01 00

addr_to_str_hello:
    dw 0x0    ; Seg
    dd 0x7C5F ; Offset

addr_to_str_hell:
    dw 0x0    ; Seg
    dd 0x7C67 ; Offset