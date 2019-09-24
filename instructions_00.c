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
 * add rm8 r8: 2 bytes (add byte [eax], ah;)
 * Adds value of ModR/M to REG.
 * 1 byte: op (00)
 * 1 byte: ModR/M
 */
void add_rm8_r8(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm;
    parse_modrm(emu, &modrm);
    uint32_t rm8_val = get_rm8(emu, &modrm);
    uint8_t r8_val = get_r8(emu, &modrm);
    uint64_t result = (uint64_t)rm8_val + (uint64_t)r8_val;
    set_rm8(emu, &modrm, result);
    update_eflags_add(emu, rm8_val, r8_val, result);
}

/*
 * add rm32 r32: 2 bytes
 * Adds value of REG to ModR/M.
 * 1 byte: op (01)
 * 1 byte: ModR/M
 */
void add_rm32_r32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm;
    parse_modrm(emu, &modrm);
    uint32_t rm32_val = get_rm32(emu, &modrm);
    uint32_t r32_val = get_r32(emu, &modrm);
    uint64_t result = (uint64_t)rm32_val + (uint64_t)r32_val;
    set_rm32(emu, &modrm, result);
    update_eflags_add(emu, rm32_val, r32_val, result);
}

/*
 * add r8 rm8: 2 bytes
 * Adds value of ModR/M to r8.
 * 1 byte: op (02)
 * 1/2 byte: ModR/M
 */
void add_r8_rm8(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm;
    parse_modrm(emu, &modrm);
    uint32_t rm8_val = get_rm8(emu, &modrm);
    uint8_t r8_val = get_r8(emu, &modrm);
    uint64_t result = (uint64_t)r8_val + (uint64_t)rm8_val;
    set_r8(emu, &modrm, result);
    update_eflags_add(emu, r8_val, rm8_val, result);
}

/*
 * add r32 rm32: 2 - 3 bytes
 * Adds value of ModR/M to r32.
 * 1 byte: op (03)
 * 1 - 2 byte: ModR/M
 */
void add_r32_rm32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm;
    parse_modrm(emu, &modrm);
    uint32_t r32_val = get_r32(emu, &modrm);
    uint32_t rm32_val = get_rm32(emu, &modrm);
    uint64_t result = (uint64_t)r32_val + (uint64_t)rm32_val;
    set_r32(emu, &modrm, result);
    update_eflags_add(emu, r32_val, rm32_val, result);
}

/*
 * add al imm8: 2 bytes
 * Adds imm8 to al.
 * 1 byte: op (04)
 * 1 byte: imm8
 */
void add_al_imm8(Emulator *emu)
{
    uint8_t imm8_val = get_code8(emu, 1);
    uint8_t al_val = get_register8(emu, AL);
    uint64_t result = (uint64_t)al_val + (uint64_t)imm8_val;
    set_register8(emu, AL, result);
    update_eflags_add(emu, al_val, imm8_val, result);
    emu->eip += 2;
}

/*
 * add al imm32: 5 bytes
 * Adds imm32 to eax.
 * 1 byte: op (05)
 * 4 byte: imm32
 */
void add_eax_imm32(Emulator *emu)
{
    uint32_t eax_val = get_register32(emu, EAX);
    uint32_t imm32_val = get_code32(emu, 1);
    uint64_t result = (uint64_t)eax_val + (uint64_t)imm32_val;
    set_register32(emu, EAX, result);
    update_eflags_add(emu, eax_val, imm32_val, result);
    emu->eip += 5;
}

/*
 * push es: 1 byte
 * Pushes value of ES to stack.
 * 1 byte: op (06)
 */
void push_es(Emulator *emu)
{
    push_segment_register(emu, ES);
    emu->eip += 1;
}

/*
 * pop es: 1 byte
 * Pops value from stack to ES.
 * 1 byte: op (07)
 */
void pop_es(Emulator *emu)
{
    pop_segment_register(emu, ES);
    emu->eip += 1;
}

/*
 * or rm8 r8: 2/3 bytes (or byte [eax], ah;)
 * Logical inclusive OR between value of ModR/M and REG, storing result to destination.
 * 1 byte: op (08)
 * 1/2 byte: ModR/M
 */
void or_rm8_r8(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm;
    parse_modrm(emu, &modrm);
    uint32_t rm8_val = get_rm8(emu, &modrm);
    uint8_t r8_val = get_r8(emu, &modrm);
    uint32_t result = rm8_val | (uint32_t)r8_val;
    set_rm8(emu, &modrm, result);
    update_eflags_or(emu, result);
}

/*
 * or rm32 r32: 2/3 bytes
 * Logical inclusive OR between value of REG and ModR/M, storing result to destination.
 * 1 byte: op (09)
 * 1/2 byte: ModR/M
 */
void or_rm32_r32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm;
    parse_modrm(emu, &modrm);
    uint32_t rm32_val = get_rm32(emu, &modrm);
    uint32_t r32_val = get_r32(emu, &modrm);
    uint32_t result = rm32_val | r32_val;
    set_rm32(emu, &modrm, result);
    update_eflags_or(emu, result);
}

/*
 * or r8 rm8: 2/3 bytes
 * Logical inclusive OR between value of ModR/M and r8, storing result of destination.
 * 1 byte: op (0A)
 * 1/2 byte: ModR/M
 */
void or_r8_rm8(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm;
    parse_modrm(emu, &modrm);
    uint32_t rm8_val = get_rm8(emu, &modrm);
    uint8_t r8_val = get_r8(emu, &modrm);
    uint32_t result = (uint32_t)r8_val | rm8_val;
    set_r8(emu, &modrm, result);
    update_eflags_or(emu, result);
}

/*
 * or r32 rm32: 2/3 bytes
 * Logical inclusive OR between value of ModR/M and r32, storing result of destination.
 * 1 byte: op (0B)
 * 1/2 byte: ModR/M
 */
void or_r32_rm32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm;
    parse_modrm(emu, &modrm);
    uint32_t r32_val = get_r32(emu, &modrm);
    uint32_t rm32_val = get_rm32(emu, &modrm);
    uint32_t result = r32_val | rm32_val;
    set_r32(emu, &modrm, result);
    update_eflags_or(emu, result);
}

/*
 * add al imm8: 2 bytes
 * Logical inclusive OR between imm8 and al, storing result to destination.
 * 1 byte: op (0C)
 * 1 byte: imm8
 */
void or_al_imm8(Emulator *emu)
{
    uint8_t imm8_val = get_code8(emu, 1);
    uint8_t al_val = get_register8(emu, AL);
    uint32_t result = (uint32_t)al_val | (uint32_t)imm8_val;
    set_register8(emu, AL, result);
    update_eflags_or(emu, result);
    emu->eip += 2;
}

/*
 * or al imm32: 5 bytes
 * Logical inclusive OR between imm32 to eax, storing result to destination.
 * 1 byte: op (0D)
 * 4 byte: imm32
 */
void or_eax_imm32(Emulator *emu)
{
    uint32_t eax_val = get_register32(emu, EAX);
    uint32_t imm32_val = get_code32(emu, 1);
    uint32_t result = eax_val | imm32_val;
    set_register32(emu, EAX, result);
    update_eflags_or(emu, result);
    emu->eip += 5;
}

/*
 * push cs: 1 byte
 * Pushes value of CS to stack.
 * 1 byte: op (0E)
 */
void push_cs(Emulator *emu)
{
    push_segment_register(emu, CS);
    emu->eip += 1;
}