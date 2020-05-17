BITS 16
    org 0x7c00
    ; jmp
    ; EB:   jmp rel8      short, displacement relative to next instruction
    ; E9:   jmp rel8/16   near, displacement relative to next instruction
    ; FF/4: jmp r/m16/32  near, absolute
    ; EA:   jmp ptr16/32  far, absolute from operand
    ; FF/5: jmp m16/32    far, absolute indirect

    ; near: within current code segment
    ; far in real mode: code seg + offset (ptr16:16/32 or m16:16/32)
    ; (far address is loaded directly into CS & EIP)
    
    ; absolute: absoslute offset from current from base of CS
    ;           usually register or r/m16/32
    ; relative: signed offset from currrent EIP
    ;           usually a label in asm -> signed value after linking

    ; direct: address in operand
    ; indirect: memory location of 4/6 byte far address (m16:16/32)

    ; not sure SEG:OFFSET or OFFSET:SEG on memory ;
    ; push SEG -> push OFFSET &
    ; pop EIP -> pop SEG according to far return instruction (CB)

    jmp [NEAR_ADDR1]
    jmp 0x1234:0x5678 ; Will be skipped.
    push cs ; Address: 7C0D

    jmp 0x7C1:0x4 ; EA 04 00 00 00 7C 00 ; 0x7C00 + 0xE = 0x7C0E
    jmp 0x2345:0x6789 ; EA jmp ptr16:16/32 ; EA BC 9A 78 56 34 12 ; Will be skipped.
    push cs ; ADDRESS: 0x7C14

    jmp far [FAR_ADDR1] ; FF 2D (same as [0x7C25]) ; FF 2D (0010 1011) 23 7C 00 00 ;
    push cs ; Will be skipped.
    jmp 0x3456:0x789A ; Will be skipped.
    push cs ; ADDRESS: 0x7C2B

    jmp 0:0

FAR_ADDR1:
    dw 0x7C0  ; Seg
    dw 0x1F   ; Offset

NEAR_ADDR1:
    dd 0x7C09