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
 * 1 byte: op (F6: 2)
 * 1|2 byte: ModRM
 */
static void neg_rm8(Emulator *emu, ModRM *modrm)
{
    int8_t rm8_val = (int8_t)get_rm8(emu, modrm);
    int8_t result = 0 - rm8_val;
    set_rm8(emu, modrm, (uint8_t)result);
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
 * 1 byte: op (F7: 2)
 * 1|2 byte: ModRM
 */
static void neg_rm32(Emulator *emu, ModRM *modrm)
{
    int32_t rm32_val = (int32_t)get_rm32(emu, modrm);
    printf("rm32: %d\n", rm32_val);
    int32_t result = 0 - rm32_val;
    printf("result: %d\n", result);
    set_rm32(emu, modrm, (uint32_t)result);
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
    default:
        printf("Not implemented: Op: F7 with ModR/M Op: %d\n", modrm.opcode);
        exit(1);
    }
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
 * inc rm32: 2 bytes
 * Increments ModR/M. Op code 83 and ModR/M op code: 000 execute this.
 * 1 byte: shared op (FF)
 */
static void inc_rm32(Emulator *emu, ModRM *modrm)
{
    uint32_t value = get_rm32(emu, modrm);
    set_rm32(emu, modrm, value + 1);
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

    default:
        printf("Not implemented: Op: FF with ModR/M Op: %d\n", modrm.opcode);
        exit(1);
    }
}