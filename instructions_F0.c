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
 * cmc: 1 byte
 * Flips carry flag.
 * 1 byte: op (F5)
 */
void cmc(Emulator *emu)
{
    set_carry_flag(emu, !is_carry(emu));
    emu->eip += 1;
}

/*
 * test rm8 imm8: 3|4 bytes
 * Performs logical AND of operands and updates flags. No result store.
 * 1 byte: op (F6: 0|1)
 * 1|2 byte: ModRM
 * 1 byte: imm8
 */
static void test_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t rm8_val = get_rm8(emu, modrm);
    uint8_t imm8_val = get_code8(emu, 0);
    uint8_t result = rm8_val & imm8_val;
    update_eflags_logical_ops_8bit(emu, result);
    emu->eip += 1;
}

/*
 * not rm8: 2|3 bytes
 * One's complement negation.
 * 1 byte: op (F6: 2)
 * 1|2 byte: ModRM
 */
static void not_rm8(Emulator *emu, ModRM *modrm)
{
    set_rm8(emu, modrm, ~(get_rm8(emu, modrm)));
}

/*
 * neg rm8: 2|3 bytes
 * Two's complement negation.
 * 1 byte: op (F6: 3)
 * 1|2 byte: ModRM
 */
static void neg_rm8(Emulator *emu, ModRM *modrm)
{
    int8_t rm8_val = (int8_t)get_rm8(emu, modrm);
    int8_t result = 0 - rm8_val;
    set_rm8(emu, modrm, (uint8_t)result);
}

/*
 * mul rm8: 2|3 
 * Performs unsigned multiplication (AX = AL * r/m8).
 * 1 byte: op (F6: 4)
 * 1|2 bytes: ModRM
 */
static void mul_rm8(Emulator *emu, ModRM *modrm)
{
    uint8_t rm8_val = get_rm8(emu, modrm);
    uint8_t al_val = get_register8(emu, AL);
    uint16_t result = al_val * rm8_val;
    uint8_t res_upper_half = result >> 8;
    uint8_t res_lower_half = (uint8_t)result;
    set_register8(emu, AH, res_upper_half);
    set_register8(emu, AL, res_lower_half);
    update_eflags_mul(emu, res_upper_half);
}

void code_f6(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);

    switch (modrm.opcode)
    {
    case 0:
        test_rm8_imm8(emu, &modrm);
        break;
    case 1:
        test_rm8_imm8(emu, &modrm);
        break;
    case 2:
        not_rm8(emu, &modrm);
        break;
    case 3:
        neg_rm8(emu, &modrm);
        break;
    case 4:
        mul_rm8(emu, &modrm);
        break;
    default:
        printf("Not implemented: Op: F6 with ModR/M Op: %d\n", modrm.opcode);
        exit(1);
    }
}

/*
 * test rm32 imm32: 6|7 bytes
 * Performs logical AND of operands and updates flags. No result store.
 * 1 byte: op (F7: 0|1)
 * 1|2 byte: ModRM
 * 4 byte: imm32
 */
static void test_rm32_imm32(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32_val = get_rm32(emu, modrm);
    uint32_t imm32_val = get_code32(emu, 0);
    emu->eip += 4;
    uint32_t result = rm32_val & imm32_val;
    update_eflags_logical_ops_8bit(emu, result);
}

/*
 * not rm32: 2|3 bytes
 * One's complement negation.
 * 1 byte: op (F7: 2)
 * 1|2 byte: ModRM
 */
static void not_rm32(Emulator *emu, ModRM *modrm)
{
    set_rm32(emu, modrm, ~(get_rm32(emu, modrm)));
}

/*
 * neg rm32: 2|3 bytes
 * Two's complement negation.
 * 1 byte: op (F7: 3)
 * 1|2 byte: ModRM
 */
static void neg_rm32(Emulator *emu, ModRM *modrm)
{
    int32_t rm32_val = (int32_t)get_rm32(emu, modrm);
    int32_t result = 0 - rm32_val;
    set_rm32(emu, modrm, (uint32_t)result);
}

/*
 * mul rm32: 2|3 
 * Performs unsigned multiplication (EDX:EAX = EAX * r/m32).
 * 1 byte: op (F7: 4)
 * 1|2 bytes: ModRM
 */
static void mul_rm32(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32_val = get_rm32(emu, modrm);
    uint32_t eax_val = get_register32(emu, EAX);
    uint64_t result = (uint64_t)eax_val * (uint64_t)rm32_val;
    uint32_t res_upper_half = result >> 32;
    uint32_t res_lower_half = (uint32_t)result;
    set_register32(emu, EDX, res_upper_half);
    set_register32(emu, EAX, res_lower_half);
    update_eflags_mul(emu, res_upper_half);
}

void code_f7(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);

    switch (modrm.opcode)
    {
    case 0:
        test_rm32_imm32(emu, &modrm);
        break;
    case 1:
        test_rm32_imm32(emu, &modrm);
        break;
    case 2:
        not_rm32(emu, &modrm);
        break;
    case 3:
        neg_rm32(emu, &modrm);
        break;
    case 4:
        mul_rm32(emu, &modrm);
        break;
    default:
        printf("Not implemented: Op: F7 with ModR/M Op: %d\n", modrm.opcode);
        exit(1);
    }
}

/*
 * clc: 1 byte
 * Clears carry flag.
 * 1 byte: op (F8)
 */
void clc(Emulator *emu)
{
    set_carry_flag(emu, 0);
    emu->eip += 1;
}

/*
 * stc: 1 byte
 * Sets carry flag.
 * 1 byte: op (F9)
 */
void stc(Emulator *emu)
{
    set_carry_flag(emu, 1);
    emu->eip += 1;
}

/*
 * cli: 1 byte
 * Clears int flag on eflags.
 * 1 byte: op (FA)
 */
void cli(Emulator *emu)
{
    set_int_flag(emu, 0);
    emu->eip += 1;
}

/*
 * sti: 1 byte
 * Sets int flag on eflags.
 * 1 byte: op (FB)
 */
void sti(Emulator *emu)
{
    set_int_flag(emu, 1);
    emu->eip += 1;
}

/*
 * cld: 1 byte
 * Clears directional on eflags. (0: up)
 * 1 byte: op (FC)
 */
void cld(Emulator *emu)
{
    set_direction_flag(emu, 0);
    emu->eip += 1;
}

/*
 * std: 1 byte
 * Sets directional on eflags. (1: down)
 * 1 byte: op (FD)
 */
void std(Emulator *emu)
{
    set_direction_flag(emu, 1);
    emu->eip += 1;
}

/*
 * inc rm8: 2|3 bytes
 * Increments ModR/M. Op code FE and ModR/M op code: 000 execute this.
 * 1 byte: shared op (FE)
 * 1|2 byte: rm8
 */
static void inc_rm8(Emulator *emu, ModRM *modrm)
{
    uint8_t rm8_val = get_rm8(emu, modrm);
    uint16_t result = rm8_val + 1;
    set_rm8(emu, modrm, (uint8_t)result);
    update_eflags_add_8bit(emu, rm8_val, 1, result);
}

/*
 * dec rm8: 2|3 bytes
 * Decrements ModR/M. Op code FE and ModR/M op code: 001 execute this.
 * 1 byte: shared op (FE)
 * 1|2 byte: rm8
 */
static void dec_rm8(Emulator *emu, ModRM *modrm)
{
    uint8_t rm8_val = get_rm8(emu, modrm);
    uint16_t result = rm8_val - 1;
    set_rm8(emu, modrm, (uint8_t)result);
    update_eflags_sub_8bit(emu, rm8_val, 1, result);
}

void code_fe(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);

    switch (modrm.opcode)
    {
    case 0:
        inc_rm8(emu, &modrm);
        break;
    case 1:
        dec_rm8(emu, &modrm);
        break;

    default:
        printf("Not implemented: Op: FE with ModR/M Op: %d\n", modrm.opcode);
        exit(1);
    }
}

/*
 * inc rm32: 2|3 bytes
 * Increments ModR/M. Op code FF and ModR/M op code: 000 execute this.
 * 1 byte: shared op (FF/0)
 * 1|2 bytes: ModRM
 */
static void inc_rm32(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32_val = get_rm32(emu, modrm);
    uint64_t result = rm32_val + 1;
    set_rm32(emu, modrm, (uint32_t)result);
    update_eflags_add(emu, rm32_val, 1, result);
}

/*
 * dec rm32: 2|3 bytes
 * Decrements ModR/M. Op code FF and ModR/M op code: 001 execute this.
 * 1 byte: shared op (FF/1)
 * 1|2 bytes: ModRM
 */
static void dec_rm32(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32_val = get_rm32(emu, modrm);
    uint64_t result = rm32_val - 1;
    set_rm32(emu, modrm, (uint32_t)result);
    update_eflags_sub(emu, rm32_val, 1, result);
}

/*
 * call rm32: 2|3 bytes
 * Jumps to 32-bit absolute address after pushing the return address.
 * 1 byte: op (FF/2)
 * 1|2 bytes: ModRM
 */
static void call_rm32(Emulator *emu, ModRM *modrm)
{
    int32_t address = get_rm32(emu, modrm);
    push32(emu, emu->eip);
    emu->eip = address;
}

/*
 * jmp rm32: 2|3 bytes
 * Jumps to 32-bit absolute address.
 * 1 byte: op (FF/4)
 * 1|2 bytes: ModRM
 */
static void jmp_rm32(Emulator *emu, ModRM *modrm)
{
    int32_t address = get_rm32(emu, modrm);
    emu->eip = address;
}

/*
 * jmp m16:32: 2|3 bytes
 * Jumps far to absolute address indirectly specified with m16:16/32.
 * 1 byte: op (FF/5)
 * 1|2 bytes: ModRM
 */
static void jmp_m_ptr(Emulator *emu, ModRM *modrm)
{
    uint32_t address = calc_memory_address(emu, modrm);
    uint16_t cs_val = get_memory16(emu, DS, address);
    uint32_t eip_val = get_memory32(emu, DS, address + 2);

    set_seg_register16(emu, CS, cs_val);
    emu->eip = eip_val;
}

/*
 * call m16:32: 2|3 bytes
 * Jumps with m16:16/32 after pushing CS and EIP.
 * 1 byte: op (FF/3)
 * 1|2 bytes: ModRM
 */
static void call_m_ptr(Emulator *emu, ModRM *modrm)
{
    push_segment_register(emu, CS);
    push32(emu, emu->eip);
    jmp_m_ptr(emu, modrm);
}

/*
 * push rm32: 2|3 bytes
 * Pushes 32-bit value from R/M.
 * 1 byte: op (FF/6)
 * 1|2 bytes: ModRM
 */
static void push_rm32(Emulator *emu, ModRM *modrm)
{
    int32_t rm32_val = get_rm32(emu, modrm);
    push32(emu, rm32_val);
}

void code_ff(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);

    switch (modrm.opcode)
    {
    case 0:
        inc_rm32(emu, &modrm);
        break;
    case 1:
        dec_rm32(emu, &modrm);
        break;
    case 2:
        call_rm32(emu, &modrm);
        break;
    case 3:
        call_m_ptr(emu, &modrm);
        break;
    case 4:
        jmp_rm32(emu, &modrm);
        break;
    case 5:
        jmp_m_ptr(emu, &modrm);
        break;
    case 6:
        push_rm32(emu, &modrm);
        break;

    default:
        printf("Not implemented: Op: FF with ModR/M Op: %d\n", modrm.opcode);
        exit(1);
    }
}