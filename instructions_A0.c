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
 * mov al moffs8: 5 bytes
 * Copies moffs value to AL. (8 bit)
 * 1 byte: op (A0)
 * 4 byte: moffs
 */
void mov_al_moffs8(Emulator *emu)
{
    uint32_t offset = get_code32(emu, 1);
    uint8_t mem_val = get_memory8(emu, offset);
    set_register8(emu, AL, mem_val);
    emu->eip += 5;
}

/*
 * mov eax moffs16/32: 5 bytes
 * Copies moffs value to AX/EAX. (16/32 bit)
 * 1 byte: op (A1)
 * 4 byte: moffs
 */
void mov_eax_moffs32(Emulator *emu)
{
    uint32_t offset = get_code32(emu, 1);
    uint32_t mem_val = get_memory32(emu, offset);
    set_register32(emu, EAX, mem_val);
    emu->eip += 5;
}

/*
 * mov moffs8 al: 5 bytes
 * Copies AL value to moffs. (8 bit)
 * 1 byte: op (A2)
 * 4 byte: moffs
 */
void mov_moffs8_al(Emulator *emu)
{
    uint32_t offset = get_code32(emu, 1);
    uint8_t al_val = get_register8(emu, AL);
    set_memory8(emu, offset, al_val);
    emu->eip += 5;
}

/*
 * mov moffs16/32 eax: 5 bytes
 * Copies AX/EAX value to moffs. (16/32 bit)
 * 1 byte: op (A3)
 * 4 byte: moffs
 */
void mov_moffs32_eax(Emulator *emu)
{
    uint32_t offset = get_code32(emu, 1);
    uint32_t eax_val = get_register32(emu, EAX);
    set_memory32(emu, offset, eax_val);
    emu->eip += 5;
}

/*
 * test al imm8: 2 bytes
 * Performs logical AND of operands and updates flags. No result store.
 * 1 byte: op (A8)
 * 1 byte: imm8 value
 */
void test_al_imm8(Emulator *emu)
{
    uint8_t imm8_val = get_code8(emu, 1);
    uint8_t al_val = get_register8(emu, AL);

    uint8_t result = al_val & imm8_val;
    update_eflags_logical_ops_8bit(emu, result);
    emu->eip += 2;
}

/*
 * test eax imm32: 5 bytes
 * Performs logical AND of operands and updates flags. No result store.
 * 1 byte: op (A9)
 * 4 bytes: imm32 value
 */
void test_eax_imm32(Emulator *emu)
{
    uint32_t eax_val = get_register32(emu, EAX);
    uint32_t imm32_val = get_code32(emu, 1);

    uint32_t result = eax_val & imm32_val;
    update_eflags_logical_ops(emu, result);
    emu->eip += 5;
}