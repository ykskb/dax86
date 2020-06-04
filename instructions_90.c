#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "instruction_defs.h"
#include "emulator_functions.h"
#include "modrm.h"
#include "io.h"

/*
 * xchg r32 r32: 1 byte
 * Exchanges values of EAX and r32.
 * 1 byte: op (90 ~ 97)
 */
void xchg_r32_r32(Emulator *emu)
{
    uint8_t reg = get_code8(emu, 0) - 0x90;
    uint32_t original_val = get_register32(emu, reg);
    set_register32(emu, reg, get_register32(emu, EAX));
    set_register32(emu, EAX, original_val);
    emu->eip += 1;
}

/*
 * xchg r16 r16: 1 byte
 * Exchanges values of AX and r16.
 * 1 byte: op (90 ~ 97)
 */
void xchg_r16_r16(Emulator *emu)
{
    uint8_t reg = get_code8(emu, 0) - 0x90;
    uint16_t original_val = get_register16(emu, reg);
    set_register16(emu, reg, get_register16(emu, EAX));
    set_register16(emu, EAX, original_val);
    emu->eip += 1;
}

/*
 * cwde: 1 byte
 * Sign-extend word(AX) to double word(EAX).
 * 1 byte: op (98)
 */
void cwde(Emulator *emu)
{
    uint32_t eax = get_register32(emu, EAX);
    uint16_t ax = (uint16_t)eax;
    uint8_t sign = ax >> 15 & 1;
    if (sign == 1)
    {
        uint32_t sign_extended = 0xFFFF0000 | ax;
        set_register32(emu, EAX, sign_extended);
    }
    else
    {
        set_register32(emu, EAX, (uint32_t)ax);
    }
    emu->eip += 1;
}

/*
 * cdq: 1 byte
 * Sign-extend double word(EAX) to quad word(EDX:EAX).
 * 1 byte: op (99)
 */
void cdq(Emulator *emu)
{
    uint32_t eax = get_register32(emu, EAX);
    uint8_t sign = eax >> 31 & 1;
    if (sign == 1)
        set_register32(emu, EDX, 0xFFFFFFFF);
    else
        set_register32(emu, EDX, 0);
    emu->eip += 1;
}

/*
 * call ptr16:16/32: 7 bytes
 * Pushes CS and EIP in order and jumps to ptr16:32.
 * 1 byte: op (9A)
 * 2 byte: cs value
 * 2|4 byte: eip (ptr16|32)
 */
void ptr_call(Emulator *emu)
{
    push_segment_register(emu, CS);
    if (emu->is_pe)
        push32(emu, emu->eip + 7);
    else
        push32(emu, emu->eip + 5);
    ptr_jump(emu);
}

/*
 * pushf / pushfd: 1 byte
 * Pushes EFLAGS into stack.
 * 1 byte: op (9C)
 */
void pushfd(Emulator *emu)
{
    /*
    if (emu->is_pe)
        push32(emu, emu->eflags);
    else
        push16(emu, (uint16_t)emu->eflags);
    */
    push32(emu, emu->eflags);
    emu->eip += 1;
};

/*
 * popf / popfd: 1 byte
 * Pops stack into EFLAGS.
 * 1 byte: op (9D)
 */
void popfd(Emulator *emu)
{
    /*
    uint32_t value;
    if (emu->is_pe)
        value = pop32(emu);
    else
        value = (uint32_t)pop16(emu);
    emu->eflags = value;
    */
    emu->eflags = pop32(emu);
    emu->eip += 1;
};

/*
 * sahf:
 * Stores AH into FLAGS (Bit 0, 2, 4, 6, 7).
 * 1 byte: op (9E)
 */
void sahf(Emulator *emu)
{
    /* AH value AND 1101 0101  128 64 16 4 1 = 213 */
    uint8_t ah_val = get_register8(emu, AH) & 213;
    /* Higher half of EFLAGS: AND 0xFFFF0000 */
    uint32_t eflags_h = emu->eflags & 0xFFFF0000;

    emu->eflags = eflags_h | ah_val;
    emu->eip += 1;
}

/*
 * lahf:
 * Load FLAGS into AH (0, 2, 4, 6, 7th bits).
 * 1 byte: op (9F)
 */
void lahf(Emulator *emu)
{
    uint8_t eflags_l = emu->eflags & 213;
    set_register8(emu, AH, eflags_l);
    emu->eip += 1;
}