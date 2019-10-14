#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "instructions.h"
#include "instruction_defs.h"
#include "emulator_functions.h"
#include "modrm.h"
#include "io.h"

/*
 * rep: 1 byte prefix
 * Repeats string operation.
 * 1 byte: prefix (F2/F3)
 */
static void rep(Emulator *emu)
{
    emu->eip += 1;
    uint32_t ecx_value = emu->registers[ECX];
    uint8_t op = get_code8(emu, 0);
    /* TODO: Add check on supported types: INS, LODS, MOVS, OUTS and STOS */
    uint32_t op_eip = emu->eip;
    int i;
    for (i = 0; i < ecx_value; i++)
    {
        emu->eip = op_eip;
        instructions[op](emu);
    }
}

instruction_func_t *instructions[256];

void init_instructions(void)
{
    int i;
    memset(instructions, 0, sizeof(instructions));

    instructions[0x00] = add_rm8_r8;
    instructions[0x01] = add_rm32_r32;
    instructions[0x02] = add_r8_rm8;
    instructions[0x03] = add_r32_rm32;
    instructions[0x04] = add_al_imm8;
    instructions[0x05] = add_eax_imm32;

    instructions[0x06] = push_es;
    instructions[0x07] = pop_es;

    instructions[0x08] = or_rm8_r8;
    instructions[0x09] = or_rm32_r32;
    instructions[0x0A] = or_r8_rm8;
    instructions[0x0B] = or_r32_rm32;
    instructions[0x0C] = or_al_imm8;
    instructions[0x0D] = or_eax_imm32;

    instructions[0x0E] = push_cs;

    instructions[0x10] = adc_rm8_r8;
    instructions[0x11] = adc_rm32_r32;
    instructions[0x12] = adc_r8_rm8;
    instructions[0x13] = adc_r32_rm32;
    instructions[0x14] = adc_al_imm8;
    instructions[0x15] = adc_eax_imm32;

    instructions[0x16] = push_ss;
    instructions[0x17] = pop_ss;

    instructions[0x18] = sbb_rm8_r8;
    instructions[0x19] = sbb_rm32_r32;
    instructions[0x1A] = sbb_r8_rm8;
    instructions[0x1B] = sbb_r32_rm32;
    instructions[0x1C] = sbb_al_imm8;
    instructions[0x1D] = sbb_eax_imm32;

    instructions[0x1E] = push_ds;
    instructions[0x1F] = pop_ds;

    instructions[0x20] = and_rm8_r8;
    instructions[0x21] = and_rm32_r32;
    instructions[0x22] = and_r8_rm8;
    instructions[0x23] = and_r32_rm32;
    instructions[0x24] = and_al_imm8;
    instructions[0x25] = and_eax_imm32;

    /* instructions[0x26] = prefix_override; */

    instructions[0x28] = sub_rm8_r8;
    instructions[0x29] = sub_rm32_r32;
    instructions[0x2A] = sub_r8_rm8;
    instructions[0x2B] = sub_r32_rm32;
    instructions[0x2C] = sub_al_imm8;
    instructions[0x2D] = sub_eax_imm32;

    /* instructions[0x2E] = prefix_override; */

    instructions[0x30] = xor_rm8_r8;
    instructions[0x31] = xor_rm32_r32;
    instructions[0x32] = xor_r8_rm8;
    instructions[0x33] = xor_r32_rm32;
    instructions[0x34] = xor_al_imm8;
    instructions[0x35] = xor_eax_imm32;

    instructions[0x38] = cmp_rm8_r8;
    instructions[0x39] = cmp_rm32_r32;
    instructions[0x3A] = cmp_r8_rm8;
    instructions[0x3B] = cmp_r32_rm32;
    instructions[0x3C] = cmp_al_imm8;
    instructions[0x3D] = cmp_eax_imm32;

    /* op code includes 8 registers in 1 byte: 0x40 ~ 0x47*/
    for (i = 0; i < 8; i++)
    {
        instructions[0x40 + i] = inc_r32;
    }

    /* op code includes 8 registers in 1 byte: 0x48 ~ 0x4F*/
    for (i = 0; i < 8; i++)
    {
        instructions[0x48 + i] = dec_r32;
    }

    /* op code includes 8 registers in 1 byte: 0x50 ~ 0x57*/
    for (i = 0; i < 8; i++)
    {
        instructions[0x50 + i] = push_r32;
    }

    /* op code includes 8 registers in 1 byte: 0x58 ~ 0x5F */
    for (i = 0; i < 8; i++)
    {
        instructions[0x58 + i] = pop_r32;
    }

    instructions[0x60] = pushad;
    instructions[0x61] = popad;
    instructions[0x68] = push_imm32;
    instructions[0x6A] = push_imm8;

    instructions[0x70] = jo;
    instructions[0x71] = jno;
    instructions[0x72] = jc;
    instructions[0x73] = jnc;
    instructions[0x74] = jz;
    instructions[0x75] = jnz;
    instructions[0x78] = js;
    instructions[0x79] = jns;
    instructions[0x7C] = jl;
    instructions[0x7E] = jle;

    instructions[0x80] = code_80;
    instructions[0x81] = code_81;
    instructions[0x83] = code_83;
    instructions[0x84] = test_rm8_r8;
    instructions[0x85] = test_rm32_r32;
    instructions[0x86] = xchg_rm8_r8;
    instructions[0x87] = xchg_rm32_r32;
    instructions[0x88] = mov_rm8_r8;
    instructions[0x89] = mov_rm32_r32;
    instructions[0x8A] = mov_r8_rm8;
    instructions[0x8B] = mov_r32_rm32;
    instructions[0x8C] = mov_rm32_seg;
    instructions[0x8D] = lea_r32_m;
    instructions[0x8E] = mov_seg_rm32;
    instructions[0x8F] = pop_rm32;

    /* op code includes 8 registers in 1 byte: 0x90 ~ 0x97 */
    for (i = 0; i < 8; i++)
    {
        instructions[0x90 + i] = xchg_r32_r32;
    }

    instructions[0x98] = cwde;
    instructions[0x99] = cdq;
    instructions[0x9C] = pushfd;
    instructions[0x9D] = popfd;
    instructions[0x9E] = sahf;
    instructions[0x9F] = lahf;

    /* op code includes 8 registers in 1 byte: 0xB0 ~ 0xB7*/
    for (i = 0; i < 8; i++)
    {
        instructions[0xB0 + i] = mov_r8_imm8;
    }

    /* op code includes 8 registers in 1 byte: 0xB8 ~ 0xBF */
    for (i = 0; i < 8; i++)
    {
        instructions[0xB8 + i] = mov_r32_imm32;
    }

    instructions[0xC3] = ret;
    instructions[0xC6] = mov_rm8_imm8;
    instructions[0xC7] = mov_rm32_imm32;
    instructions[0xC9] = leave;

    instructions[0xE9] = near_jump;
    instructions[0xEB] = short_jump;
    instructions[0xEC] = in_al_dx;
    instructions[0xEE] = out_dx_al;

    instructions[0xF3] = rep;
    instructions[0xFA] = cli;
    instructions[0xFB] = sti;
    instructions[0xFC] = cld;
    instructions[0xFD] = std;
    instructions[0xFF] = code_ff;

    instructions[0xE8] = call_rel32;
}