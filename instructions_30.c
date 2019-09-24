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
 * cmp al imm8: 2 bytes
 * Compares AL value and 8-bit imm value by subtracting in order.
 * 1 byte: op (3C)
 * 1 byte: 8-bit imm value
 */
void cmp_al_imm8(Emulator *emu)
{
    uint8_t value = get_code8(emu, 1);
    uint8_t al = get_register8(emu, AL);
    uint64_t result = (uint64_t)al - (uint64_t)value;
    update_eflags_sub(emu, al, value, result);
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
    uint32_t value = get_code32(emu, 1);
    uint32_t eax = get_register32(emu, EAX);
    uint64_t result = (uint64_t)eax - (uint64_t)value;
    update_eflags_sub(emu, eax, value, result);
    emu->eip += 5;
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
    ModRM modrm;
    parse_modrm(emu, &modrm);
    uint32_t r32 = get_r32(emu, &modrm);
    uint32_t rm32 = get_rm32(emu, &modrm);
    uint64_t result = (uint64_t)r32 - (uint64_t)rm32;
    update_eflags_sub(emu, r32, rm32, result);
}