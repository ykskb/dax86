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

instruction_func_t *two_byte_instructions[256];
instruction_func_t *instructions[256];
uint8_t quiet;

/*
 * Executes 2-byte instruction: 1 byte
 * 1 byte: op (0F)
 */
static void two_byte_inst(Emulator *emu)
{
    uint8_t op = get_code8(emu, 1);
    if (two_byte_instructions[op] == NULL)
    {
        printf("EIP: %08x Op: 0f %x not implemented.\n", emu->eip, op);
        exit(1);
    }
    two_byte_instructions[op](emu);
}

static void lock_prefix(Emulator *emu)
{
    emu->eip += 1;
    uint8_t op = get_code8(emu, 0);
    if (!quiet)
        printf("Ignoring lock prefix.\n");
    instructions[op](emu);
}

static void operand_override(Emulator *emu)
{
    emu->eip += 1;
    uint8_t op = get_code8(emu, 0);
    if (emu->is_pe)
    {
        switch (op)
        {
        case 0x85:
            test_rm16_r16(emu);
            break;
        case 0x89:
            mov_rm16_r16(emu);
            break;
        case 0xA3:
            mov_moffs16_ax(emu);
            break;
        case 0xB8:
            mov_r16_imm16(emu);
            break;
        case 0xC7:
            mov_rm16_imm16(emu);
            break;
        default:
            printf("EIP: %08x Op: 66 %x not implemented.\n", emu->eip, op);
            exit(1);
            break;
        }
    }
    else
    {
        if (op >= 0xB8 && op <= 0xBF)
        {
            mov_r32_imm32(emu);
            return;
        }
        switch (op)
        {
        case 0x85:
            test_rm32_r32(emu);
            break;
        case 0x01:
            add_rm32_r32(emu);
            break;
        case 0x83:
            code_83(emu);
            break;
        case 0x89:
            mov_rm32_r32(emu);
            break;
        default:
            printf("EIP: %08x Op: 66 %x not implemented.\n", emu->eip, op);
            exit(1);
            break;
        }
    }
}

/* 
 * REP/REPE/REPZ/REPNE/REPNZ
 * 
 * Repeats string operations.
 *
 * REP(F3): terminates if ECS=0
 *     INS(6C/6D)
 *     MOVS(A4/A5)
 *     OUTS(6E/6F)
 *     LODS(AC/AD)
 *     STOS(AA/AB)
 * REPE/REPZ(F3): terminates if ECX=0 or ZF=0:
 *     CMPS(A6/A7)
 *     SCAS(AE/AF)
 * REPNE/REPNZ(F2): terminates if ECX=0 or ZF=1:
 *     CMPS(A6/A7)
 *     SCAS(AE/AF)
 */
static void rep(Emulator *emu)
{
    emu->eip += 1;
    uint32_t ecx_value = get_register32(emu, ECX);
    uint8_t op = get_code8(emu, 0);
    uint32_t op_eip = emu->eip;
    if (instructions[op] == NULL)
    {
        printf("Op: f3 %x not implemented.\n", op);
        exit(1);
    }
    int i;
    for (i = 0; i < ecx_value; i++)
    {
        emu->eip = op_eip;
        if (!quiet)
            printf("CS: %04X EIP: %08X Op: %02X\n", get_seg_register16(emu, CS), emu->eip, op);
        instructions[op](emu);
        if ((op == 0xA6 || op == 0xA7 || op == 0xAE || op == 0xAF) && !is_zero(emu))
        {
            break;
        }
    }
    set_register32(emu, ECX, ecx_value - i);
}

static void repne(Emulator *emu)
{
    emu->eip += 1;
    uint32_t ecx_value = get_register32(emu, ECX);
    uint8_t op = get_code8(emu, 0);
    uint32_t op_eip = emu->eip;
    if (instructions[op] == NULL)
    {
        printf("Op: f2 %x not implemented.\n", op);
        exit(1);
    }
    int i;
    for (i = 0; i < ecx_value; i++)
    {
        emu->eip = op_eip;
        if (!quiet)
            printf("CS: %04X EIP: %08X Op: %02X\n", get_seg_register16(emu, CS), emu->eip, op);
        instructions[op](emu);
        if ((op == 0xA6 || op == 0xA7 || op == 0xAE || op == 0xAF) && is_zero(emu))
        {
            break;
        }
    }
    set_register32(emu, ECX, ecx_value - i);
}

static void init_two_byte_instructions(void)
{
    memset(two_byte_instructions, 0, sizeof(two_byte_instructions));
    two_byte_instructions[0x01] = code_0f_01;
    two_byte_instructions[0x20] = mov_r32_cr;
    two_byte_instructions[0x22] = mov_cr_r32;
    two_byte_instructions[0x82] = jc32;
    two_byte_instructions[0x83] = jnc32;
    two_byte_instructions[0x84] = jz32;
    two_byte_instructions[0x85] = jnz32;
    two_byte_instructions[0x86] = jna32;
    two_byte_instructions[0x87] = ja32;
    two_byte_instructions[0x8F] = jg32;
    two_byte_instructions[0x94] = sete;
    two_byte_instructions[0xB6] = movzx_r32_rm8;
    two_byte_instructions[0xB7] = movzx_r32_rm16;
    two_byte_instructions[0xBE] = movsx_r32_rm8;
}

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

    instructions[0x0F] = two_byte_inst;

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
    instructions[0x66] = operand_override;
    instructions[0x68] = push_imm32;
    instructions[0x69] = imul_r32_rm32_imm32;
    instructions[0x6A] = push_imm8;
    instructions[0x6B] = imul_r32_rm32_imm8;
    instructions[0x6D] = ins_m32_dx;

    instructions[0x70] = jo;
    instructions[0x71] = jno;
    instructions[0x72] = jc;
    instructions[0x73] = jnc;
    instructions[0x74] = jz;
    instructions[0x75] = jnz;
    instructions[0x76] = jna;
    instructions[0x77] = ja;
    instructions[0x78] = js;
    instructions[0x79] = jns;
    instructions[0x7C] = jl;
    instructions[0x7E] = jle;
    instructions[0x7F] = jg;

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
    instructions[0x9A] = ptr_call;
    instructions[0x9C] = pushfd;
    instructions[0x9D] = popfd;
    instructions[0x9E] = sahf;
    instructions[0x9F] = lahf;

    instructions[0xA0] = mov_al_moffs8;
    instructions[0xA1] = mov_eax_moffs32;
    instructions[0xA2] = mov_moffs8_al;
    instructions[0xA3] = mov_moffs32_eax;
    instructions[0xA4] = movsb;
    instructions[0xA5] = movsd;
    instructions[0xA6] = cmpsb;
    instructions[0xA7] = cmpsd;
    instructions[0xA8] = test_al_imm8;
    instructions[0xA9] = test_eax_imm32;
    instructions[0xAA] = stosb;
    instructions[0xAB] = stosd;
    instructions[0xAC] = lodsb;
    instructions[0xAD] = lodsd;
    instructions[0xAE] = scasb;
    instructions[0xAF] = scasd;

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

    instructions[0xC0] = code_c0;
    instructions[0xC1] = code_c01;
    instructions[0xC3] = ret;
    instructions[0xC4] = les;
    instructions[0xC5] = lds;
    instructions[0xC6] = mov_rm8_imm8;
    instructions[0xC7] = mov_rm32_imm32;
    instructions[0xC9] = leave;
    instructions[0xCB] = ret_far;

    instructions[0xE0] = loopnz;
    instructions[0xE1] = loopz;
    instructions[0xE2] = loop;
    instructions[0xE3] = jecxz;
    instructions[0xE4] = in_al_imm8;
    instructions[0xE5] = in_eax_imm8;
    instructions[0xE6] = out_imm8_al;
    instructions[0xE7] = out_imm8_eax;
    instructions[0xE8] = call_rel32;
    instructions[0xE9] = near_jump;
    instructions[0xEA] = ptr_jump;
    instructions[0xEB] = short_jump;
    instructions[0xEC] = in_al_dx;
    instructions[0xED] = in_eax_dx;
    instructions[0xEE] = out_dx_al;
    instructions[0xEF] = out_dx_eax;

    instructions[0xF0] = lock_prefix;
    instructions[0xF2] = repne;
    instructions[0xF3] = rep;
    instructions[0xF5] = cmc;
    instructions[0xF6] = code_f6;
    instructions[0xF7] = code_f7;
    instructions[0xF8] = clc;
    instructions[0xF9] = stc;
    instructions[0xFA] = cli;
    instructions[0xFB] = sti;
    instructions[0xFC] = cld;
    instructions[0xFD] = std;
    instructions[0xFE] = code_fe;
    instructions[0xFF] = code_ff;

    init_two_byte_instructions();
}