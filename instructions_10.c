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
 * adc rm8 r8: 2|3 bytes (adc byte [eax], ah;)
 * Adds values of ModR/M and carry flag to REG.
 * 1 byte: op (10)
 * 1|2 byte: ModR/M
 */
void adc_rm8_r8(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint8_t rm8_val = get_rm8(emu, &modrm);
    uint8_t r8_val = get_r8(emu, &modrm) + (uint8_t)is_carry(emu);
    uint16_t result = (uint16_t)rm8_val + (uint16_t)r8_val;
    set_rm8(emu, &modrm, result);
    update_eflags_add_8bit(emu, rm8_val, r8_val, result);
}

/*
 * adc rm32 r32: 2|3 bytes
 * Adds values of REG and carry flag to ModR/M.
 * 1 byte: op (11)
 * 1|2 byte: ModR/M
 */
void adc_rm32_r32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t rm32_val = get_rm32(emu, &modrm);
    uint32_t r32_val = get_r32(emu, &modrm) + is_carry(emu);
    uint64_t result = (uint64_t)rm32_val + (uint64_t)r32_val;
    set_rm32(emu, &modrm, result);
    update_eflags_add(emu, rm32_val, r32_val, result);
}

/*
 * adc r8 rm8: 2|3 bytes
 * Adds values of ModR/M and carry flag to r8.
 * 1 byte: op (12)
 * 1|2 byte: ModR/M
 */
void adc_r8_rm8(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint8_t rm8_val = get_rm8(emu, &modrm);
    uint8_t r8_val = get_r8(emu, &modrm) + (uint8_t)is_carry(emu);
    uint16_t result = (uint16_t)r8_val + (uint16_t)rm8_val;
    set_r8(emu, &modrm, result);
    update_eflags_add_8bit(emu, r8_val, rm8_val, result);
}

/*
 * adc r32 rm32: 2|3 bytes
 * Adds values of ModR/M and carry flag to r32.
 * 1 byte: op (13)
 * 1|2 byte: ModR/M
 */
void adc_r32_rm32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t r32_val = get_r32(emu, &modrm);
    uint32_t rm32_val = get_rm32(emu, &modrm) + is_carry(emu);
    uint64_t result = (uint64_t)r32_val + (uint64_t)rm32_val;
    set_r32(emu, &modrm, result);
    update_eflags_add(emu, r32_val, rm32_val, result);
}

/*
 * adc al imm8: 2 bytes
 * Adds imm8 and carry flag to al.
 * 1 byte: op (14)
 * 1 byte: imm8
 */
void adc_al_imm8(Emulator *emu)
{
    uint8_t imm8_val = (int8_t)get_sign_code8(emu, 1);
    uint8_t al_val = get_register8(emu, AL) + (uint8_t)is_carry(emu);
    uint16_t result = (uint16_t)al_val + (uint16_t)imm8_val;
    set_register8(emu, AL, result);
    update_eflags_add_8bit(emu, al_val, imm8_val, result);
    emu->eip += 2;
}

/*
 * adc al imm32: 5 bytes
 * Adds imm32 and carry flag to eax.
 * 1 byte: op (15)
 * 4 byte: imm32
 */
void adc_eax_imm32(Emulator *emu)
{
    uint32_t eax_val = get_register32(emu, EAX);
    uint32_t imm32_val = (int32_t)get_sign_code32(emu, 1) + is_carry(emu);
    uint64_t result = (uint64_t)eax_val + (uint64_t)imm32_val;
    set_register32(emu, EAX, result);
    update_eflags_add(emu, eax_val, imm32_val, result);
    emu->eip += 5;
}

/*
 * push ss: 1 byte
 * Pushes value of SS to stack.
 * 1 byte: op (16)
 */
void push_ss(Emulator *emu)
{
    push_segment_register(emu, SS);
    emu->eip += 1;
}

/*
 * pop es: 1 byte
 * Pops value from stack to SS.
 * 1 byte: op (17)
 */
void pop_ss(Emulator *emu)
{
    pop_segment_register(emu, SS);
    emu->eip += 1;
}

/*
 * sbb rm8 r8: 2|3 bytes (sbb byte [eax], ah;)
 * Subtracts values of ModR/M and carry flag from REG.
 * 1 byte: op (18)
 * 1|2 byte: ModR/M
 */
void sbb_rm8_r8(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint8_t rm8_val = get_rm8(emu, &modrm);
    uint8_t r8_val = get_r8(emu, &modrm) + (uint8_t)is_carry(emu);
    uint16_t result = (uint16_t)rm8_val - (uint16_t)r8_val;
    set_rm8(emu, &modrm, result);
    update_eflags_sub_8bit(emu, rm8_val, r8_val, result);
}

/*
 * sbb rm32 r32: 2|3 bytes
 * Subtracts values of REG and carry flag from ModR/M.
 * 1 byte: op (19)
 * 1|2 byte: ModR/M
 */
void sbb_rm32_r32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t rm32_val = get_rm32(emu, &modrm);
    uint32_t r32_val = get_r32(emu, &modrm) + is_carry(emu);
    uint64_t result = (uint64_t)rm32_val - (uint64_t)r32_val;
    set_rm32(emu, &modrm, result);
    update_eflags_sub(emu, rm32_val, r32_val, result);
}

/*
 * sbb r8 rm8: 2|3 bytes
 * Subtracts values of ModR/M and carry flag from r8.
 * 1 byte: op (1A)
 * 1|2 byte: ModR/M
 */
void sbb_r8_rm8(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint8_t rm8_val = get_rm8(emu, &modrm);
    uint8_t r8_val = get_r8(emu, &modrm) + (uint8_t)is_carry(emu);
    uint16_t result = (uint16_t)r8_val - (uint16_t)rm8_val;
    set_r8(emu, &modrm, result);
    update_eflags_sub_8bit(emu, r8_val, rm8_val, result);
}

/*
 * sbb r32 rm32: 2|3 bytes
 * Subtracts values of ModR/M and carry flag from r32.
 * 1 byte: op (13)
 * 1|2 byte: ModR/M
 */
void sbb_r32_rm32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t r32_val = get_r32(emu, &modrm);
    uint32_t rm32_val = get_rm32(emu, &modrm) + is_carry(emu);
    uint64_t result = (uint64_t)r32_val - (uint64_t)rm32_val;
    set_r32(emu, &modrm, result);
    update_eflags_sub(emu, r32_val, rm32_val, result);
}

/*
 * sbb al imm8: 2 bytes
 * Subtracts imm8 and carry flag from al.
 * 1 byte: op (14)
 * 1 byte: imm8
 */
void sbb_al_imm8(Emulator *emu)
{
    uint8_t imm8_val = (int8_t)get_sign_code8(emu, 1);
    uint8_t al_val = get_register8(emu, AL) + (uint8_t)is_carry(emu);
    uint16_t result = (uint16_t)al_val - (uint16_t)imm8_val;
    set_register8(emu, AL, result);
    update_eflags_sub_8bit(emu, al_val, imm8_val, result);
    emu->eip += 2;
}

/*
 * sbb al imm32: 5 bytes
 * Subtracts imm32 and carry flag from eax.
 * 1 byte: op (15)
 * 4 byte: imm32
 */
void sbb_eax_imm32(Emulator *emu)
{
    uint32_t eax_val = get_register32(emu, EAX);
    uint32_t imm32_val = (int32_t)get_sign_code32(emu, 1) + is_carry(emu);
    uint64_t result = (uint64_t)eax_val - (uint64_t)imm32_val;
    set_register32(emu, EAX, result);
    update_eflags_add(emu, eax_val, imm32_val, result);
    emu->eip += 5;
}

/*
 * push ds: 1 byte
 * Pushes value of DS to stack.
 * 1 byte: op (1E)
 */
void push_ds(Emulator *emu)
{
    push_segment_register(emu, DS);
    emu->eip += 1;
}

/*
 * pop ds: 1 byte
 * Pops value from stack to DS.
 * 1 byte: op (1F)
 */
void pop_ds(Emulator *emu)
{
    pop_segment_register(emu, DS);
    emu->eip += 1;
}