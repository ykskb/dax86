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
 * and rm8 r8: 2/3 bytes (and byte [eax], ah;)
 * Logical AND between value of ModR/M and REG, storing result to destination.
 * 1 byte: op (20)
 * 1/2 byte: ModR/M
 */
void and_rm8_r8(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint8_t rm8_val = get_rm8(emu, &modrm);
    uint8_t r8_val = get_r8(emu, &modrm);
    uint8_t result = rm8_val & r8_val;
    set_rm8(emu, &modrm, result);
    update_eflags_logical_ops_8bit(emu, result);
}

/*
 * and rm32 r32: 2/3 bytes
 * Logical AND between value of REG and ModR/M, storing result to destination.
 * 1 byte: op (21)
 * 1/2 byte: ModR/M
 */
void and_rm32_r32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t rm32_val = get_rm32(emu, &modrm);
    uint32_t r32_val = get_r32(emu, &modrm);
    uint32_t result = rm32_val & r32_val;
    set_rm32(emu, &modrm, result);
    update_eflags_logical_ops(emu, result);
}

/*
 * and r8 rm8: 2/3 bytes
 * Logical AND between value of ModR/M and r8, storing result of destination.
 * 1 byte: op (22)
 * 1/2 byte: ModR/M
 */
void and_r8_rm8(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint8_t rm8_val = get_rm8(emu, &modrm);
    uint8_t r8_val = get_r8(emu, &modrm);
    uint8_t result = r8_val & rm8_val;
    set_r8(emu, &modrm, result);
    update_eflags_logical_ops_8bit(emu, result);
}

/*
 * and r32 rm32: 2/3 bytes
 * Logical AND between value of ModR/M and r32, storing result of destination.
 * 1 byte: op (23)
 * 1/2 byte: ModR/M
 */
void and_r32_rm32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t r32_val = get_r32(emu, &modrm);
    uint32_t rm32_val = get_rm32(emu, &modrm);
    uint32_t result = r32_val & rm32_val;
    set_r32(emu, &modrm, result);
    update_eflags_logical_ops(emu, result);
}

/*
 * and al imm8: 2 bytes
 * Logical AND between imm8 and al, storing result to destination.
 * 1 byte: op (24)
 * 1 byte: imm8
 */
void and_al_imm8(Emulator *emu)
{
    uint8_t imm8_val = get_code8(emu, 1);
    uint8_t al_val = get_register8(emu, AL);
    uint8_t result = al_val & imm8_val;
    set_register8(emu, AL, result);
    update_eflags_logical_ops_8bit(emu, result);
    emu->eip += 2;
}

/*
 * and al imm32: 5 bytes
 * Logical AND between imm32 to eax, storing result to destination.
 * 1 byte: op (25)
 * 4 byte: imm32
 */
void and_eax_imm32(Emulator *emu)
{
    uint32_t eax_val = get_register32(emu, EAX);
    uint32_t imm32_val = get_code32(emu, 1);
    uint32_t result = eax_val & imm32_val;
    set_register32(emu, EAX, result);
    update_eflags_logical_ops(emu, result);
    emu->eip += 5;
}

/* 26 ES override prefix */
/* 27 DAA Adjusts two packed BCD values. */

/*
 * sub rm8 r8: 2 bytes (sub byte [eax], ah;)
 * Subtracts value of ModR/M from REG.
 * 1 byte: op (28)
 * 1 byte: ModR/M
 */
void sub_rm8_r8(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint8_t rm8_val = get_rm8(emu, &modrm);
    uint8_t r8_val = get_r8(emu, &modrm);
    uint16_t result = (uint16_t)rm8_val - (uint16_t)r8_val;
    set_rm8(emu, &modrm, result);
    update_eflags_sub_8bit(emu, rm8_val, r8_val, result);
}

/*
 * sub rm32 r32: 2 bytes
 * Subtracts value of REG from ModR/M.
 * 1 byte: op (29)
 * 1 byte: ModR/M
 */
void sub_rm32_r32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t rm32_val = get_rm32(emu, &modrm);
    uint32_t r32_val = get_r32(emu, &modrm);
    uint64_t result = (uint64_t)rm32_val - (uint64_t)r32_val;
    set_rm32(emu, &modrm, result);
    update_eflags_sub(emu, rm32_val, r32_val, result);
}

/*
 * sub r8 rm8: 2 bytes
 * Subtracts value of ModR/M from r8.
 * 1 byte: op (2A)
 * 1/2 byte: ModR/M
 */
void sub_r8_rm8(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint8_t rm8_val = get_rm8(emu, &modrm);
    uint8_t r8_val = get_r8(emu, &modrm);
    uint16_t result = (uint16_t)r8_val - (uint16_t)rm8_val;
    set_r8(emu, &modrm, result);
    update_eflags_sub_8bit(emu, r8_val, rm8_val, result);
}

/*
 * sub r32 rm32: 2 - 3 bytes
 * Subtracts value of ModR/M to r32.
 * 1 byte: op (2B)
 * 1 - 2 byte: ModR/M
 */
void sub_r32_rm32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t r32_val = get_r32(emu, &modrm);
    uint32_t rm32_val = get_rm32(emu, &modrm);
    uint64_t result = (uint64_t)r32_val - (uint64_t)rm32_val;
    set_r32(emu, &modrm, result);
    update_eflags_sub(emu, r32_val, rm32_val, result);
}

/*
 * sub al imm8: 2 bytes
 * Subtracts imm8 from al.
 * 1 byte: op (2C)
 * 1 byte: imm8
 */
void sub_al_imm8(Emulator *emu)
{
    uint8_t imm8_val = (int8_t)get_sign_code8(emu, 1);
    uint8_t al_val = get_register8(emu, AL);
    uint16_t result = (uint16_t)al_val - (uint16_t)imm8_val;
    set_register8(emu, AL, result);
    update_eflags_sub_8bit(emu, al_val, imm8_val, result);
    emu->eip += 2;
}

/*
 * sub eax imm32: 5 bytes
 * Subtracts imm32 from eax.
 * 1 byte: op (2D)
 * 4 byte: imm32
 */
void sub_eax_imm32(Emulator *emu)
{
    uint32_t eax_val = get_register32(emu, EAX);
    uint32_t imm32_val = (int32_t)get_sign_code32(emu, 1);
    uint64_t result = (uint64_t)eax_val - (uint64_t)imm32_val;
    set_register32(emu, EAX, result);
    update_eflags_sub(emu, eax_val, imm32_val, result);
    emu->eip += 5;
}

/* 2E CS override prefix */