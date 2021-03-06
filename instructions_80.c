#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "instruction_defs.h"
#include "emulator_functions.h"
#include "modrm.h"
#include "io.h"
#include "util.h"

/*
 * add rm8 imm8: 3|4 bytes
 * Adds imm8 to ModR/M(8 bit). Op code 80 and ModR/M op code: 0 execute this.
 * 1 byte: shared op (80)
 * 1|2 byte: ModR/M
 * 1 byte: imm8 to add
 */
static void add_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t rm8 = get_rm8(emu, modrm);
    uint8_t imm8 = (int8_t)get_sign_code8(emu, 0);
    emu->eip += 1;

    set_rm8(emu, modrm, rm8 + imm8);
    uint16_t result = (uint16_t)rm8 + (uint16_t)imm8;
    update_eflags_add_8bit(emu, rm8, imm8, result);
}

/*
 * or rm8 imm8: 3|4 bytes
 * Logical inclusive OR between rm8 and imm8, storing result to destination.
 * 1 byte: shared op (80)
 * 1|2 byte: ModR/M
 * 1 byte: imm8 to or
 */
static void or_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t rm8 = get_rm8(emu, modrm);
    uint8_t imm8 = get_sign_code8(emu, 0);
    emu->eip += 1;

    uint8_t result = rm8 | imm8;
    set_rm8(emu, modrm, result);
    update_eflags_logical_ops_8bit(emu, result);
}

/*
 * adc rm8 imm8: 3|4 bytes
 * Adds imm8 to ModR/M(8 bit) with carry flag. Op code 80 and ModR/M op code: 2 execute this.
 * 1 byte: shared op (80)
 * 1|2 byte: ModR/M
 * 1 byte: imm8 to add
 */
static void adc_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t rm8 = get_rm8(emu, modrm);
    uint8_t imm8 = (int8_t)get_sign_code8(emu, 0) + (uint8_t)is_carry(emu);
    emu->eip += 1;

    set_rm8(emu, modrm, rm8 + imm8);
    uint16_t result = (uint16_t)rm8 + (uint16_t)imm8;
    update_eflags_add_8bit(emu, rm8, imm8, result);
}

/*
 * sbb rm8 imm8: 3 bytes
 * Subtracts imm8 and carry flag from rm8. Op code 80 and ModR/M op code: 3 execute this.
 * 1 byte: shared op (80)
 * 1 byte: ModR/M
 * 1 byte: imm8 to subtract
 */
static void sbb_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t rm8 = get_rm8(emu, modrm);
    uint8_t imm8 = get_sign_code8(emu, 0) + (uint8_t)is_carry(emu);
    emu->eip += 1;

    uint16_t result = (uint16_t)rm8 - (uint16_t)imm8;
    set_rm8(emu, modrm, result);
    update_eflags_sub_8bit(emu, rm8, imm8, result);
}

/*
 * and rm8 imm8: 3|4 bytes
 * Logical AND between rm8 and imm8, storing the result to destination.
 * 1 byte: shared op (80)
 * 1|2 bytes: ModR/M
 * 1 byte: imm8 to AND
 */
static void and_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t rm8 = get_rm8(emu, modrm);
    uint8_t imm8 = get_sign_code8(emu, 0);
    emu->eip += 1;

    uint8_t result = rm8 & imm8;
    set_rm8(emu, modrm, result);
    update_eflags_logical_ops_8bit(emu, result);
}

/*
 * sub rm8 imm8: 3 bytes
 * Subtracts imm8 from rm8. Op code 80 and ModR/M op code: 101 execute this.
 * 1 byte: shared op (80)
 * 1 byte: ModR/M
 * 1 byte: imm8 to subtract
 */
static void sub_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t rm8 = get_rm8(emu, modrm);
    uint8_t imm8 = get_sign_code8(emu, 0);
    emu->eip += 1;

    uint16_t result = (uint16_t)rm8 - (uint16_t)imm8;
    set_rm8(emu, modrm, result);
    update_eflags_sub_8bit(emu, rm8, imm8, result);
}

/*
 * xor rm8 imm8: 3 bytes
 * Logical XOR between rm8 and imm8, storing result to destination.
 * 1 byte: shared op (80)
 * 1 byte: ModR/M
 * 1 byte: imm8 to xor
 */
static void xor_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t rm8 = get_rm8(emu, modrm);
    uint8_t imm8 = get_sign_code8(emu, 0);
    emu->eip += 1;

    uint8_t result = rm8 ^ imm8;
    set_rm8(emu, modrm, result);
    update_eflags_logical_ops_8bit(emu, result);
}

/*
 * cmp rm8 imm8: 3 bytes
 * Compares RM8 value and imm8 value by subtracting in order.
 * Op code 80 and ModR/M op code: 111 execute this.
 * 1 byte: shared op (80)
 * 1 byte: ModR/M
 * 1 byte: imm8 to subtract
 */
static void cmp_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t rm8 = get_rm8(emu, modrm);
    uint8_t imm8 = get_sign_code8(emu, 0);
    emu->eip += 1;

    uint16_t result = (uint16_t)rm8 - (uint16_t)imm8;
    update_eflags_sub_8bit(emu, rm8, imm8, result);
}

void code_80(Emulator *emu)
{
    /* Proceed 1 byte for op code 80. */
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);

    switch (modrm.opcode)
    {
    case 0:
        add_rm8_imm8(emu, &modrm);
        break;
    case 1:
        or_rm8_imm8(emu, &modrm);
        break;
    case 2:
        adc_rm8_imm8(emu, &modrm);
        break;
    case 3:
        sbb_rm8_imm8(emu, &modrm);
        break;
    case 4:
        and_rm8_imm8(emu, &modrm);
        break;
    case 5:
        sub_rm8_imm8(emu, &modrm);
        break;
    case 6:
        xor_rm8_imm8(emu, &modrm);
        break;
    case 7:
        cmp_rm8_imm8(emu, &modrm);
        break;
    default:
        printf("Not implemented: Op: 80 with ModR/M Op: %d\n", modrm.opcode);
        panic_exit(emu);
    }
}

/*
 * add rm32 imm32: 6|7 bytes
 * Adds imm32 to ModR/M(32 bit). Op code 81 and ModR/M op code: 0 execute this.
 * 1 byte: shared op (81)
 * 1|2 bytes: ModR/M
 * 4 bytes: imm32 to add
 */
static void add_rm32_imm32(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32 = get_rm32(emu, modrm);
    uint32_t imm32 = get_sign_code32(emu, 0);
    emu->eip += 4;

    set_rm32(emu, modrm, rm32 + imm32);
    uint64_t result = (uint64_t)rm32 + (uint64_t)imm32;
    update_eflags_add(emu, rm32, imm32, result);
}

/*
 * or rm32 imm32: 6|7 bytes
 * Logical inclusive OR between rm32 and imm32, storing result to destination.
 * 1 byte: shared op (81)
 * 1|2 bytes: ModR/M
 * 4 bytes: imm32 to or
 */
static void or_rm32_imm32(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32 = get_rm32(emu, modrm);
    uint32_t imm32 = get_sign_code32(emu, 0);
    emu->eip += 4;

    uint32_t result = rm32 | imm32;
    set_rm32(emu, modrm, result);
    update_eflags_logical_ops(emu, result);
}

/*
 * adc rm32 imm32: 6|7 bytes
 * Adds imm32 to ModR/M(32 bit) with carry flag. Op code 81 and ModR/M op code: 2 execute this.
 * 1 byte: shared op (81)
 * 1|2 bytes: ModR/M
 * 4 byte: imm32 to add
 */
static void adc_rm32_imm32(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32 = get_rm32(emu, modrm);
    uint32_t imm32 = get_sign_code32(emu, 0) + (uint32_t)is_carry(emu);
    emu->eip += 4;

    set_rm32(emu, modrm, rm32 + imm32);
    uint64_t result = (uint64_t)rm32 + (uint64_t)imm32;
    update_eflags_add(emu, rm32, imm32, result);
}

/*
 * sbb rm32 imm32: 6|7 bytes
 * Subtracts imm32 and carry flag from rm32. Op code 81 and ModR/M op code: 3 execute this.
 * 1 byte: shared op (81)
 * 1|2 bytes: ModR/M
 * 4 bytes: imm32 to subtract
 */
static void sbb_rm32_imm32(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32 = get_rm32(emu, modrm);
    uint32_t imm32 = get_sign_code32(emu, 0) + (uint32_t)is_carry(emu);
    emu->eip += 4;

    uint64_t result = (uint64_t)rm32 - (uint64_t)imm32;
    set_rm32(emu, modrm, result);
    update_eflags_sub(emu, rm32, imm32, result);
}

/*
 * and rm32 imm32: 6|7 bytes
 * Logical AND between rm32 and imm32, storing the result to destination.
 * 1 byte: shared op (81)
 * 1|2 bytes: ModR/M
 * 4 bytes: imm32 to AND
 */
static void and_rm32_imm32(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32 = get_rm32(emu, modrm);
    uint32_t imm32 = get_sign_code32(emu, 0);
    emu->eip += 4;

    uint32_t result = rm32 & imm32;
    set_rm32(emu, modrm, result);
    update_eflags_logical_ops(emu, result);
}

/*
 * sub rm32 imm32: 6|7 bytes
 * Subtracts imm32 from rm32. Op code 81 and ModR/M op code: 101 execute this.
 * 1 byte: shared op (81)
 * 1|2 bytes: ModR/M
 * 4 bytes: imm32 to subtract
 */
static void sub_rm32_imm32(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32 = get_rm32(emu, modrm);
    uint32_t imm32 = get_sign_code32(emu, 0);
    emu->eip += 4;

    uint64_t result = (uint64_t)rm32 - (uint64_t)imm32;
    set_rm32(emu, modrm, result);
    update_eflags_sub(emu, rm32, imm32, result);
}

/*
 * xor rm32 imm32: 6|7 bytes
 * Logical XOR between rm32 and imm32, storing result to destination.
 * 1 byte: shared op (81)
 * 1|2 byte: ModR/M
 * 4 byte: imm32 to xor
 */
static void xor_rm32_imm32(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32 = get_rm32(emu, modrm);
    uint32_t imm32 = get_sign_code32(emu, 0);
    emu->eip += 4;

    uint32_t result = rm32 ^ imm32;
    set_rm32(emu, modrm, result);
    update_eflags_logical_ops(emu, result);
}

/*
 * cmp rm32 imm32: 6|7 bytes
 * Compares RM32 value and imm32 value by subtracting in order.
 * Op code 81 and ModR/M op code: 111 execute this.
 * 1 byte: shared op (81)
 * 1|2 byte: ModR/M
 * 4 byte: imm32 to subtract
 */
static void cmp_rm32_imm32(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32 = get_rm32(emu, modrm);
    uint32_t imm32 = get_sign_code32(emu, 0);
    emu->eip += 4;

    uint64_t result = (uint64_t)rm32 - (uint64_t)imm32;
    update_eflags_sub(emu, rm32, imm32, result);
}

void code_81(Emulator *emu)
{
    /* Proceed 1 byte for op code 81. */
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);

    switch (modrm.opcode)
    {
    case 0:
        add_rm32_imm32(emu, &modrm);
        break;
    case 1:
        or_rm32_imm32(emu, &modrm);
        break;
    case 2:
        adc_rm32_imm32(emu, &modrm);
        break;
    case 3:
        sbb_rm32_imm32(emu, &modrm);
        break;
    case 4:
        and_rm32_imm32(emu, &modrm);
        break;
    case 5:
        sub_rm32_imm32(emu, &modrm);
        break;
    case 6:
        xor_rm32_imm32(emu, &modrm);
        break;
    case 7:
        cmp_rm32_imm32(emu, &modrm);
        break;
    default:
        printf("Not implemented: Op: 81 with ModR/M Op: %d\n", modrm.opcode);
        panic_exit(emu);
    }
}

/*
 * add rm32 imm8: 3|4 bytes
 * Adds imm8 to RM32. Op code 83 and ModR/M op code: 0 execute this.
 * 1 byte: shared op (83)
 * 1|2 byte: ModR/M
 * 1 byte: imm8 to add
 */
static void add_rm32_imm8(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32 = get_rm32(emu, modrm);
    uint32_t imm8 = (int32_t)get_sign_code8(emu, 0);
    emu->eip += 1;

    uint64_t result = (uint64_t)rm32 + (uint64_t)imm8;
    set_rm32(emu, modrm, result);
    update_eflags_add(emu, rm32, imm8, result);
}

/*
 * or rm32 imm8: 3|4 bytes
 * Logical inclusive OR between rm32 and imm8, storing result to destination.
 * 1 byte: shared op (83)
 * 1|2 byte: ModR/M
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
 * adc rm32 imm8: 3|4 bytes
 * Adds imm8 to ModR/M(32 bit) with carry flag. Op code 83 and ModR/M op code: 2 execute this.
 * 1 byte: shared op (83)
 * 1|2 bytes: ModR/M
 * 1 byte: imm8 to add
 */
static void adc_rm32_imm8(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32 = get_rm32(emu, modrm);
    uint32_t imm8 = (int32_t)get_sign_code8(emu, 0) + (uint32_t)is_carry(emu);
    emu->eip += 1;

    set_rm32(emu, modrm, rm32 + imm8);
    uint64_t result = (uint64_t)rm32 + (uint64_t)imm8;
    update_eflags_add(emu, rm32, imm8, result);
}

/*
 * sbb rm32 imm8: 3|4 bytes
 * Subtracts imm8 and carry flag from rm32. Op code 83 and ModR/M op code: 3 execute this.
 * 1 byte: shared op (83)
 * 1|2 bytes: ModR/M
 * 1 bytes: imm8 to subtract
 */
static void sbb_rm32_imm8(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32 = get_rm32(emu, modrm);
    uint32_t imm8 = (int32_t)get_sign_code8(emu, 0) + (uint32_t)is_carry(emu);
    emu->eip += 1;

    uint64_t result = (uint64_t)rm32 - (uint64_t)imm8;
    set_rm32(emu, modrm, result);
    update_eflags_sub(emu, rm32, imm8, result);
}

/*
 * and rm32 imm8: 3|4 bytes
 * Logical AND between rm32 and imm8, storing the result to destination.
 * 1 byte: shared op (83)
 * 1|2 bytes: ModR/M
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

/*
 * xor rm32 imm8: 3 bytes
 * Logical XOR between rm32 and imm8, storing result to destination.
 * 1 byte: shared op (83)
 * 1 byte: ModR/M
 * 1 byte: imm8 to xor
 */
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
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);

    switch (modrm.opcode)
    {
    case 0:
        add_rm32_imm8(emu, &modrm);
        break;
    case 1:
        or_rm32_imm8(emu, &modrm);
        break;
    case 2:
        adc_rm32_imm8(emu, &modrm);
        break;
    case 3:
        sbb_rm32_imm8(emu, &modrm);
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
        panic_exit(emu);
    }
}

/*
 * add rm16 imm8: 3|4 bytes
 * Adds imm8 to RM16. Op code 83 and ModR/M op code: 0 execute this.
 * 1 byte: shared op (83)
 * 1|2 byte: ModR/M
 * 1 byte: imm8 to add
 */
static void add_rm16_imm8(Emulator *emu, ModRM *modrm)
{
    uint16_t rm16 = get_rm16(emu, modrm);
    uint16_t imm8 = (uint16_t)get_sign_code8(emu, 0);
    emu->eip += 1;

    uint32_t result = (uint32_t)rm16 + (uint32_t)imm8;
    set_rm16(emu, modrm, result);
    update_eflags_add_16bit(emu, rm16, imm8, result);
}

/*
 * cmp rm16 imm8: 3 bytes
 * Compares rm16 value and imm8 value by subtracting in order.
 * Op code 83 and ModR/M op code: 111 execute this.
 * 1 byte: shared op (83)
 * 1 byte: ModR/M
 * 1 byte: imm8 to subtract
 */
static void cmp_rm16_imm8(Emulator *emu, ModRM *modrm)
{
    uint16_t rm16 = get_rm16(emu, modrm);
    uint16_t imm8 = (int16_t)get_sign_code8(emu, 0);
    emu->eip += 1;

    uint32_t result = (uint32_t)rm16 - (uint32_t)imm8;
    update_eflags_sub_16bit(emu, rm16, imm8, result);
}

void code_83_rm16(Emulator *emu)
{
    /* Proceed 1 byte for op code 83. */
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);

    switch (modrm.opcode)
    {
    case 0:
        add_rm16_imm8(emu, &modrm);
        break;
    // case 1:
    //     or_rm16_imm8(emu, &modrm);
    //     break;
    // case 2:
    //     adc_rm16_imm8(emu, &modrm);
    //     break;
    // case 3:
    //     sbb_rm16_imm8(emu, &modrm);
    //     break;
    // case 4:
    //     and_rm16_imm8(emu, &modrm);
    //     break;
    // case 5:
    //     sub_rm16_imm8(emu, &modrm);
    //     break;
    // case 6:
    //     xor_rm16_imm8(emu, &modrm);
    //     break;
    case 7:
        cmp_rm16_imm8(emu, &modrm);
        break;
    default:
        printf("Not implemented: Op: 83 with ModR/M Op: %d\n", modrm.opcode);
        panic_exit(emu);
    }
}

/*
 * test rm8 r8: 2|3 bytes
 * Performs logical AND of operands and updates flags. No result store.
 * 1 byte: op (84)
 * 1|2 byte: ModR/M
 */
void test_rm8_r8(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint8_t rm8 = get_rm8(emu, &modrm);
    /* Reads value from register specified by REG bits. */
    uint8_t r8 = get_r8(emu, &modrm);

    uint8_t result = rm8 & r8;
    update_eflags_logical_ops_8bit(emu, result);
}

/*
 * test rm16 r16: 2|3 bytes
 * Performs logical AND of operands and updates flags. No result store.
 * 1 byte: op (85)
 * 1|2 byte: ModR/M
 */
void test_rm16_r16(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint16_t rm16 = get_rm16(emu, &modrm);
    uint16_t r16 = get_r16(emu, &modrm);

    uint32_t result = rm16 & r16;
    update_eflags_logical_ops_16bit(emu, result);
}

/*
 * test rm32 r32: 2|3 bytes
 * Performs logical AND of operands and updates flags. No result store.
 * 1 byte: op (85)
 * 1|2 byte: ModR/M
 */
void test_rm32_r32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t rm32 = get_rm32(emu, &modrm);
    /* Reads value from register specified by REG bits. */
    uint32_t r32 = get_r32(emu, &modrm);

    uint32_t result = rm32 & r32;
    update_eflags_logical_ops(emu, result);
}

/*
 * xchg rm8 r8: 2|3 bytes
 * Exchanges the contents of operands.
 * 1 byte: op (86)
 * 1|2 byte: ModR/M
 */
void xchg_rm8_r8(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint8_t rm8 = get_rm8(emu, &modrm);
    uint8_t r8 = get_r8(emu, &modrm);

    set_rm8(emu, &modrm, r8);
    set_r8(emu, &modrm, rm8);
}

/*
 * xchg rm32 r32: 2|3 bytes
 * Exchanges the contents of operands.
 * 1 byte: op (87)
 * 1|2 byte: ModR/M
 */
void xchg_rm32_r32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t rm32 = get_rm32(emu, &modrm);
    uint32_t r32 = get_r32(emu, &modrm);

    set_rm32(emu, &modrm, r32);
    set_r32(emu, &modrm, rm32);
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
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    /* Reads value from register specified by REG bits. */
    uint32_t r32 = get_r32(emu, &modrm);
    /* Sets value on memory/register specified by ModR/M. */
    set_rm32(emu, &modrm, r32);
}

/*
 * mov rm16 r16: 2 bytes
 * Copies value of register specified by REG to ModR/M.
 * 1 byte: op (89)
 * 1 byte: ModR/M
 */
void mov_rm16_r16(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint16_t r16 = get_r16(emu, &modrm);
    set_rm16(emu, &modrm, r16);
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
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    /* Reads value from memory/register specified by ModR/M. */
    uint32_t rm32 = get_rm32(emu, &modrm);
    /* Sets value on register specified by REG bits. */
    set_r32(emu, &modrm, rm32);
}

/*
 * mov rm32 seg: 2|3 bytes
 * Copies segment register value to rm32.
 * 1 byte: op (8C)
 * 1|2 byte: ModR/M
 */
void mov_rm32_seg(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t seg_val = get_seg_r(emu, &modrm);
    set_rm32(emu, &modrm, seg_val);
}

/*
 * lea r32 m:
 * Loads the calcurated effective address to r32.
 * 1 byte: op (8D)
 * 1|2 bytes: ModR/M
 */
void lea_r32_m(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t addr = calc_memory_address(emu, &modrm);
    set_r32(emu, &modrm, addr);
}

/*
 * mov seg rm32: 2|3 bytes
 * Copies rm32 to segment register.
 * 1 byte: op (8E)
 * 1|2 byte: RodR/M
 */
void mov_seg_rm32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t rm32 = get_rm32(emu, &modrm);
    set_seg_r(emu, &modrm, rm32);
}

/*
 * pop rm32: 2|3 bytes
 * Pops from stack to rm32.
 * 1 byte: op (8F)
 * 1|2 byte: RodR/M
 */
void pop_rm32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    set_rm32(emu, &modrm, pop32(emu));
}