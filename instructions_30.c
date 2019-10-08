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
 * or rm8 r8: 2/3 bytes (or byte [eax], ah;)
 * Logical exclusive OR between value of ModR/M and REG, storing result to destination.
 * 1 byte: op (30)
 * 1/2 byte: ModR/M
 */
void xor_rm8_r8(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint8_t rm8_val = get_rm8(emu, &modrm);
    uint8_t r8_val = get_r8(emu, &modrm);
    uint8_t result = rm8_val ^ r8_val;
    set_rm8(emu, &modrm, result);
    update_eflags_logical_ops_8bit(emu, result);
}

/*
 * or rm32 r32: 2/3 bytes
 * Logical exclusive OR between value of REG and ModR/M, storing result to destination.
 * 1 byte: op (31)
 * 1/2 byte: ModR/M
 */
void xor_rm32_r32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t rm32_val = get_rm32(emu, &modrm);
    uint32_t r32_val = get_r32(emu, &modrm);
    uint32_t result = rm32_val ^ r32_val;
    set_rm32(emu, &modrm, result);
    update_eflags_logical_ops(emu, result);
}

/*
 * or r8 rm8: 2/3 bytes
 * Logical exclusive OR between value of ModR/M and r8, storing result of destination.
 * 1 byte: op (32)
 * 1/2 byte: ModR/M
 */
void xor_r8_rm8(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint8_t rm8_val = get_rm8(emu, &modrm);
    uint8_t r8_val = get_r8(emu, &modrm);
    uint8_t result = (uint8_t)r8_val ^ rm8_val;
    set_r8(emu, &modrm, result);
    update_eflags_logical_ops_8bit(emu, result);
}

/*
 * or r32 rm32: 2/3 bytes
 * Logical exclusive OR between value of ModR/M and r32, storing result of destination.
 * 1 byte: op (33)
 * 1/2 byte: ModR/M
 */
void xor_r32_rm32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t r32_val = get_r32(emu, &modrm);
    uint32_t rm32_val = get_rm32(emu, &modrm);
    uint32_t result = r32_val ^ rm32_val;
    set_r32(emu, &modrm, result);
    update_eflags_logical_ops(emu, result);
}

/*
 * or al imm8: 2 bytes
 * Logical exclusive OR between imm8 and al, storing result to destination.
 * 1 byte: op (34)
 * 1 byte: imm8
 */
void xor_al_imm8(Emulator *emu)
{
    uint8_t imm8_val = get_code8(emu, 1);
    uint8_t al_val = get_register8(emu, AL);
    uint8_t result = (uint8_t)al_val ^ (uint8_t)imm8_val;
    set_register8(emu, AL, result);
    update_eflags_logical_ops_8bit(emu, result);
    emu->eip += 2;
}

/*
 * or al imm32: 5 bytes
 * Logical exclusive OR between imm32 to eax, storing result to destination.
 * 1 byte: op (35)
 * 4 byte: imm32
 */
void xor_eax_imm32(Emulator *emu)
{
    uint32_t eax_val = get_register32(emu, EAX);
    uint32_t imm32_val = get_code32(emu, 1);
    uint32_t result = eax_val ^ imm32_val;
    set_register32(emu, EAX, result);
    update_eflags_logical_ops(emu, result);
    emu->eip += 5;
}

/*
 * cmp rm8 r8: 2|3 bytes
 * Compares register ModR/M(8bit) value and register's 8-bit value by subtracting in order.
 * 1 byte: op (38)
 * 1|2 byte: ModR/M
 */
void cmp_rm8_r8(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint8_t rm8_val = get_rm8(emu, &modrm);
    uint8_t r8_val = get_r8(emu, &modrm);
    uint16_t result = (uint16_t)rm8_val - (uint16_t)r8_val;
    update_eflags_sub_8bit(emu, rm8_val, r8_val, result);
}

/*
 * cmp rm32 r32: 2|3 bytes
 * Compares register ModR/M(32bit) value and register's 32-bit value by subtracting in order.
 * 1 byte: op (39)
 * 1|2 byte: ModR/M
 */
void cmp_rm32_r32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t rm32_val = get_rm32(emu, &modrm);
    uint32_t r32_val = get_r32(emu, &modrm);
    uint64_t result = (uint64_t)rm32_val - (uint64_t)r32_val;
    update_eflags_sub(emu, rm32_val, r32_val, result);
}

/*
 * cmp r8 rm8: 2|3 bytes
 * Compares register register's 8-bit value and ModR/M (8 bit) by subtracting in order.
 * 1 byte: op (3A)
 * 1|2 byte: ModR/M
 */
void cmp_r8_rm8(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint8_t rm8_val = get_rm8(emu, &modrm);
    uint8_t r8_val = get_r8(emu, &modrm);
    uint16_t result = (uint16_t)r8_val - (uint16_t)rm8_val;
    update_eflags_sub_8bit(emu, r8_val, rm8_val, result);
}

/*
 * cmp r32 rm32: 2 bytes
 * Compares register 32-bit value and RM32 value by subtracting in order.
 * 1 byte: op (3B)
 * 1 byte: ModR/M
 */
void cmp_r32_rm32(Emulator *emu)
{
    emu->eip += 1; // op code
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t r32 = get_r32(emu, &modrm);
    uint32_t rm32 = get_rm32(emu, &modrm);
    uint64_t result = (uint64_t)r32 - (uint64_t)rm32;
    update_eflags_sub(emu, r32, rm32, result);
}

/*
 * cmp al imm8: 2 bytes
 * Compares AL value and 8-bit imm value by subtracting in order.
 * 1 byte: op (3C)
 * 1 byte: 8-bit imm value
 */
void cmp_al_imm8(Emulator *emu)
{
    uint8_t value = (int8_t)get_sign_code8(emu, 1);
    uint8_t al = get_register8(emu, AL);
    uint16_t result = (uint16_t)al - (uint16_t)value;
    update_eflags_sub_8bit(emu, al, value, result);
    emu->eip += 2;
}

/*
 * cmp eax imm32: 5 bytes
 * Compares EAX value and 32-bit imm value by subtracting in order.
 * 1 byte: op (3D)
 * 4 bytes: 32-bit imm value
 */
void cmp_eax_imm32(Emulator *emu)
{
    uint32_t value = (int32_t)get_sign_code32(emu, 1);
    uint32_t eax = get_register32(emu, EAX);
    uint64_t result = (uint64_t)eax - (uint64_t)value;
    update_eflags_sub(emu, eax, value, result);
    emu->eip += 5;
}
