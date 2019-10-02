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
 * add rm32 imm8: 3 bytes
 * Adds imm8 to RM32. Op code 83 and ModR/M op code: 0 execute this.
 * 1 byte: shared op (83)
 * 1 byte: ModR/M
 * 1 byte: imm8 to add
 */
static void add_rm32_imm8(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32 = get_rm32(emu, modrm);
    uint32_t imm8 = (int32_t)get_sign_code8(emu, 0);
    emu->eip += 1;
    set_rm32(emu, modrm, rm32 + imm8);
}

/*
 * or rm32 imm8: 3 bytes
 * Logical inclusive OR between rm32 and imm8, storing result to destination.
 * 1 byte: shared op (83)
 * 1 byte: ModR/M
 * 1 byte: imm8 to or
 */
static void or_rm32_imm8(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32 = get_rm32(emu, modrm);
    uint32_t imm8 = (int32_t)get_sign_code8(emu, 0);
    emu->eip += 1;
    uint32_t result = rm32 | imm8;

    set_rm32(emu, modrm, result);
    update_eflags_logical_ops(emu, result);
}

/*
 * and rm32 imm8: 3/4 bytes
 * Logical AND between rm32 and imm8, storing the result to destination.
 * 1 byte: shared op (83)
 * 1/2 bytes: ModR/M
 * 1 byte: imm8 to AND
 */
static void and_rm32_imm8(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32 = get_rm32(emu, modrm);
    uint32_t imm8 = (int32_t)get_sign_code8(emu, 0);
    emu->eip += 1;
    uint32_t result = rm32 & imm8;

    set_rm32(emu, modrm, result);
    update_eflags_logical_ops(emu, result);
}

/*
 * sub rm32 imm8: 3 bytes
 * Subtracts imm8 from RM32. Op code 83 and ModR/M op code: 101 execute this.
 * 1 byte: shared op (83)
 * 1 byte: ModR/M
 * 1 byte: imm8 to subtract
 */
static void sub_rm32_imm8(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32 = get_rm32(emu, modrm);
    uint32_t imm8 = (int32_t)get_sign_code8(emu, 0);
    emu->eip += 1;
    uint64_t result = (uint64_t)rm32 - (uint64_t)imm8;

    set_rm32(emu, modrm, result);
    update_eflags_sub(emu, rm32, imm8, result);
}

static void xor_rm32_imm8(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32 = get_rm32(emu, modrm);
    uint32_t imm8 = (int32_t)get_sign_code8(emu, 0);
    emu->eip += 1;
    uint32_t result = rm32 ^ imm8;

    set_rm32(emu, modrm, result);
    update_eflags_logical_ops(emu, result);
}

/*
 * cmp rm32 imm8: 3 bytes
 * Compares RM32 value and imm8 value by subtracting in order.
 * Op code 83 and ModR/M op code: 111 execute this.
 * 1 byte: shared op (83)
 * 1 byte: ModR/M
 * 1 byte: imm8 to subtract
 */
static void cmp_rm32_imm8(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32 = get_rm32(emu, modrm);
    uint32_t imm8 = (int32_t)get_sign_code8(emu, 0);
    emu->eip += 1;
    uint64_t result = (uint64_t)rm32 - (uint64_t)imm8;

    update_eflags_sub(emu, rm32, imm8, result);
}

void code_83(Emulator *emu)
{
    /* Proceed 1 byte for op code 83. */
    emu->eip += 1;
    ModRM modrm;
    parse_modrm(emu, &modrm);

    switch (modrm.opcode)
    {
    case 0:
        add_rm32_imm8(emu, &modrm);
        break;
    case 1:
        or_rm32_imm8(emu, &modrm);
        break;
    case 4:
        and_rm32_imm8(emu, &modrm);
        break;
    case 5:
        sub_rm32_imm8(emu, &modrm);
        break;
    case 6:
        xor_rm32_imm8(emu, &modrm);
        break;
    case 7:
        cmp_rm32_imm8(emu, &modrm);
        break;
    default:
        printf("Not implemented: Op: 83 with ModR/M Op: %d\n", modrm.opcode);
        exit(1);
    }
}

/*
 * mov rm8 r8: 2 bytes
 * Copies 8-bit value from register specified by ModR/M to the register
 * or memory specified by ModR/M.
 * 1 byte: op (88)
 * 1 byte: ModR/M
 */
void mov_rm8_r8(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t r8 = get_r8(emu, &modrm);
    set_rm8(emu, &modrm, r8);
}

/*
 * mov rm32 r32: 2 bytes
 * Copies value of register specified by REG to ModR/M.
 * 1 byte: op (89)
 * 1 byte: ModR/M
 */
void mov_rm32_r32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm;
    parse_modrm(emu, &modrm);
    /* Reads value from register specified by REG bits. */
    uint32_t r32 = get_r32(emu, &modrm);
    /* Sets value on memory/register specified by ModR/M. */
    set_rm32(emu, &modrm, r32);
}

/*
 * mov r8 rm8: 2 bytes
 * Copies 8-bit value from register or memory specified by ModR/M to the register
 * specified by ModR/M.
 * 1 byte: op (8A)
 * 1 byte: ModR/M
 */
void mov_r8_rm8(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t rm8 = get_rm8(emu, &modrm);
    set_r8(emu, &modrm, rm8);
}

/*
 * mov r32 rm32: 2 bytes
 * Copies value of ModR/M to REG.
 * 1 byte: op (8B)
 * 1 byte: ModR/M
 */
void mov_r32_rm32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm;
    parse_modrm(emu, &modrm);
    /* Reads value from memory/register specified by ModR/M. */
    uint32_t rm32 = get_rm32(emu, &modrm);
    /* Sets value on register specified by REG bits. */
    set_r32(emu, &modrm, rm32);
}

/*
 * mov rm16 seg: 2 bytes
 * Copes rm16 to segment register.
 * 1 byte: op (8E)
 * 1 byte: RodR/M
 */
void mov_seg_rm16(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm;
    parse_modrm(emu, &modrm);
    uint32_t rm32 = get_rm32(emu, &modrm);
    set_seg_r(emu, &modrm, rm32);
}