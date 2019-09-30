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
 * ret: 1 byte
 * Jumps to the address pushed by call (address after call instruction).
 * 1 byte: op (C3)
 */
void ret(Emulator *emu)
{
    emu->eip = pop32(emu);
}

/*
 * mov rm8 imm8: 3/4 bytes
 * Copies imm8 value to register or memory specified by ModR/M(rm8).
 * 1 byte: op (C6)
 * 1/2 byte: ModR/M
 * 1 byte: imm8 value
 */
void mov_rm8_imm8(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm;
    parse_modrm(emu, &modrm);
    uint8_t value = get_code8(emu, 0);
    emu->eip += 1;
    set_rm8(emu, &modrm, value);
}

/*
 * mov rm32 imm32: 5 bytes
 * Copies imm value to register or memory specified by ModR/M (rm32).
 * 1 byte: op (C7)
 * 1 byte: ModR/M
 * 4 bytes: value (32)
 */
void mov_rm32_imm32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm;
    parse_modrm(emu, &modrm);
    uint32_t value = get_code32(emu, 0);
    emu->eip += 4;
    set_rm32(emu, &modrm, value);
}

/*
 * leave: 1byte
 * Set of mov esp, ebp & pop ebp.
 * 1 byte: op (C9)
 */
void leave(Emulator *emu)
{
    uint32_t ebp_val = get_register32(emu, EBP);
    /* Update ESP with current EBP value. */
    set_register32(emu, ESP, ebp_val);
    /* Pop from stack and set it on EBP. */
    set_register32(emu, EBP, pop32(emu));
    emu->eip += 1;
}
