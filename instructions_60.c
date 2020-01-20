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
 * pushad: 1 bytes
 * Pushes contents of general-purpose registers into stack.
 * 1 byte: op (60)
 */
void pushad(Emulator *emu)
{
    uint32_t original_esp = get_register32(emu, ESP);
    push32(emu, get_register32(emu, EAX));
    push32(emu, get_register32(emu, ECX));
    push32(emu, get_register32(emu, EDX));
    push32(emu, get_register32(emu, EBX));
    push32(emu, original_esp);
    push32(emu, get_register32(emu, EBP));
    push32(emu, get_register32(emu, ESI));
    push32(emu, get_register32(emu, EDI));
    emu->eip += 1;
}

/*
 * popad: 1 bytes
 * Pops from stack into general-purpose registers.
 * 1 byte: op (61)
 */
void popad(Emulator *emu)
{
    set_register32(emu, EDI, pop32(emu));
    set_register32(emu, ESI, pop32(emu));
    set_register32(emu, EBP, pop32(emu));
    /* Skips original ESP value by incrementing ESP by 4 bytes. */
    set_register32(emu, ESP, get_register32(emu, ESP) + 4);
    set_register32(emu, EBX, pop32(emu));
    set_register32(emu, EDX, pop32(emu));
    set_register32(emu, ECX, pop32(emu));
    set_register32(emu, EAX, pop32(emu));
    emu->eip += 1;
}

/*
 * push imm32: 5 bytes
 * Pushes 32-bit immediate value into stack.
 * 1 byte: op (68)
 * 4 bytes: immediate 32-bit value
 */
void push_imm32(Emulator *emu)
{
    uint32_t value = get_code32(emu, 1);
    push32(emu, value);
    emu->eip += 5;
}

static void imul_r32_rm32_imm(Emulator *emu, ModRM *modrm, int32_t imm_val)
{
    uint64_t result;
    if (modrm->mod == 3)
    {
        /* r32 = r32 * signed imm */
        int32_t r32_val = get_register32(emu, modrm->reg_index);
        result = (int64_t)r32_val * (int64_t)imm_val;
    }
    else
    {
        /* r32 = rm32 * signed imm */
        int32_t rm32_val = get_rm32(emu, modrm);
        result = (int64_t)rm32_val * (int64_t)imm_val;
    }
    uint32_t res_upper_half = result >> 32;
    uint32_t res_lower_half = (uint32_t)result;
    update_eflags_mul(emu, res_upper_half);
    set_register32(emu, modrm->reg_index, res_lower_half);
}

/*
 * imul r32 imm8 | imul r32 rm32 imm32: 6|7 bytes
 * Performs signed multiplication (r32 = (r32|rm32) * imm32).
 * 1 byte: op (69)
 * 1|2 bytes: ModRm
 * 4 bytes: imm32 value
 */
void imul_r32_rm32_imm32(Emulator *emu)
{
    /* Proceed 1 byte for op code 6B. */
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    int32_t imm32_val = get_code32(emu, 0);
    emu->eip += 4;
    imul_r32_rm32_imm(emu, &modrm, imm32_val);
}

/*
 * push imm8: 2 bytes
 * Pushes 8-bit immediate value into stack.
 * 1 byte: op (6A)
 * 1 byte: immediate 8-bit value
 */
void push_imm8(Emulator *emu)
{
    uint8_t value = get_code8(emu, 1);
    push32(emu, (uint32_t)value);
    emu->eip += 2;
}

/*
 * imul r32 imm8 | imul r32 rm32 imm8: 3|4 bytes
 * Performs signed multiplication (r32 = (r32|rm32) * imm8).
 * 1 byte: op (6B)
 * 1|2 bytes: ModRm
 * 1 bytes: imm8 value
 */
void imul_r32_rm32_imm8(Emulator *emu)
{
    /* Proceed 1 byte for op code 6B. */
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    int8_t imm8_val = get_code8(emu, 0);
    emu->eip += 1;
    imul_r32_rm32_imm(emu, &modrm, imm8_val);
}
