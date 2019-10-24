#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "instruction_defs.h"
#include "emulator_functions.h"
#include "modrm.h"
#include "io.h"

static uint8_t adjust_imm8_for_rm8(uint8_t val)
{
    while (val > 8)
    {
        val -= 8;
    }
    return val;
}

/*
 * 1100 1100 rol 2 => 0011 0011
 * (rm8_val << 2):       0011 0000
 * (rm8_val >> (8 - 2)): 0000 0011
 * 0011 0000 | 0000 0011 = Result: 0011 0011
 */
static void rol_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t rm8_val = get_rm8(emu, modrm);
    uint8_t imm8_val = adjust_imm8_for_rm8(get_code8(emu, 0));
    emu->eip += 1;

    uint8_t result = (rm8_val << imm8_val) | (rm8_val >> (8 - imm8_val));
    set_rm8(emu, modrm, result);
}

static void ror_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t rm8_val = get_rm8(emu, modrm);
    uint8_t imm8_val = adjust_imm8_for_rm8(get_code8(emu, 0));
    emu->eip += 1;

    uint8_t result = (rm8_val >> imm8_val) | (rm8_val << (8 - imm8_val));
    set_rm8(emu, modrm, result);
}

/*
 * 1100 1100 1 rcl 3 => 0110 0111 0 (1001 1001 1 -> 0011 0011 1 -> 0110 0111 0)
 * (rm8_val << 3):      0110 0100 0
 * (rm_val >> (9 - 3)): 0000 0011 0
 * 0110 0100 0 | 0000 0011 0 = Result: 0110 0111 CF: 0
 */
static void rcl_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    uint16_t rm8_val = (uint16_t)get_rm8(emu, modrm);
    uint8_t imm8_val = adjust_imm8_for_rm8(get_code8(emu, 0));
    emu->eip += 1;
    uint16_t carry_bit = (uint16_t)is_carry(emu);

    uint16_t cf_added = (rm8_val << 1) | carry_bit;
    uint16_t result = (cf_added << imm8_val) | (cf_added >> (9 - imm8_val));
    set_rm8(emu, modrm, (uint8_t)(result >> 1));
    set_carry_flag(emu, result & 1);
}

/*
 * 1 1100 1100 rcl 3 => 1 0011 1001 (0 1110 0110 -> 0 0111 0011 -> 1 0011 1001)
 * (rm8_val >> 3):      0 0011 1001
 * (rm_val << (9 - 3)): 1 0000 0000
 * 0 0011 1001 | 1 0000 0000 = CF: 1 Result: 0011 1001
 */
static void rcr_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    uint16_t rm8_val = (uint16_t)get_rm8(emu, modrm);
    uint8_t imm8_val = adjust_imm8_for_rm8(get_code8(emu, 0));
    emu->eip += 1;
    uint16_t carry_bit = (uint16_t)is_carry(emu);

    uint16_t cf_added = rm8_val | (carry_bit << 8);
    uint16_t result = (cf_added >> imm8_val) | (cf_added << (9 - imm8_val));
    set_rm8(emu, modrm, (uint8_t)(result));
    set_carry_flag(emu, result & 0x100);
}

/*
 * shl rm8 imm8: 2|3 bytes
 * Shifts left, setting the carry flag accordingly.
 * 1 byte: op code()
 * 1|2 bytes: rm8
 * 1 byte: imm8
 * 1 1100 1100 << 3
 * 1 1001 1000
 * 1 0011 0000
 * 0 0110 0000
 */
static void shl_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    uint16_t rm8_val = (uint16_t)get_rm8(emu, modrm);
    uint8_t imm8_val = adjust_imm8_for_rm8(get_code8(emu, 0));
    emu->eip += 1;

    uint16_t result = rm8_val << imm8_val;
    set_rm8(emu, modrm, (uint8_t)(result));
    if (imm8_val > 0)
    {
        set_carry_flag(emu, result & 0x100);
    }
}

static void shr_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    uint16_t rm8_val = (uint16_t)get_rm8(emu, modrm);
    uint8_t imm8_val = adjust_imm8_for_rm8(get_code8(emu, 0));
    emu->eip += 1;
    uint16_t carry_bit = (uint16_t)is_carry(emu);

    uint16_t cf_added = (rm8_val << 1) | carry_bit;
    uint16_t result = cf_added >> imm8_val;
    set_rm8(emu, modrm, (uint8_t)(result >> 1));
    if (imm8_val > 0)
    {
        set_carry_flag(emu, result & 1);
    }
}

static void sar_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    /* convert to signed and shift with cf */
    int16_t rm8_val = (int8_t)get_rm8(emu, modrm);
    uint8_t imm8_val = adjust_imm8_for_rm8(get_code8(emu, 0));
    emu->eip += 1;
    int16_t carry_bit = (int16_t)is_carry(emu);

    int16_t cf_added = (rm8_val << 1) | carry_bit;
    int16_t result = cf_added >> imm8_val;
    set_rm8(emu, modrm, (uint8_t)(result >> 1));
    if (imm8_val > 0)
    {
        set_carry_flag(emu, result & 1);
    }
}

void code_c0(Emulator *emu)
{
    /* Proceed 1 byte for op code C0. */
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);

    switch (modrm.opcode)
    {
    case 0:
        rol_rm8_imm8(emu, &modrm);
        break;
    case 1:
        ror_rm8_imm8(emu, &modrm);
        break;
    case 2:
        rcl_rm8_imm8(emu, &modrm);
        break;
    case 3:
        rcr_rm8_imm8(emu, &modrm);
        break;
    case 4:
        shl_rm8_imm8(emu, &modrm); /* shl or sal: technically the same. */
        break;
    case 5:
        shr_rm8_imm8(emu, &modrm);
        break;
    case 6:
        shr_rm8_imm8(emu, &modrm); /* shr or sar: undefined? */
        break;
    case 7:
        sar_rm8_imm8(emu, &modrm);
        break;
    default:
        printf("Not implemented: Op: C0 with ModR/M Op: %d\n", modrm.opcode);
        exit(1);
    }
}

static uint8_t adjust_imm8_for_rm32(uint8_t val)
{
    while (val > 32)
    {
        val -= 32;
    }
    return val;
}

static void rol_rm32_imm8(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32_val = get_rm32(emu, modrm);
    uint8_t imm8_val = adjust_imm8_for_rm32(get_code8(emu, 0));
    emu->eip += 1;

    uint32_t result = (rm32_val << imm8_val) | (rm32_val >> (32 - imm8_val));
    set_rm32(emu, modrm, result);
}

static void ror_rm32_imm8(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32_val = get_rm32(emu, modrm);
    uint8_t imm8_val = adjust_imm8_for_rm32(get_code8(emu, 0));
    emu->eip += 1;

    uint32_t result = (rm32_val >> imm8_val) | (rm32_val << (32 - imm8_val));
    set_rm32(emu, modrm, result);
}

static void rcl_rm32_imm8(Emulator *emu, ModRM *modrm)
{
    uint64_t rm32_val = (uint64_t)get_rm32(emu, modrm);
    uint8_t imm8_val = adjust_imm8_for_rm32(get_code8(emu, 0));
    emu->eip += 1;
    uint64_t carry_bit = (uint64_t)is_carry(emu);

    uint64_t cf_added = (rm32_val << 1) | carry_bit;
    uint64_t result = (cf_added << imm8_val) | (cf_added >> (33 - imm8_val));
    set_rm32(emu, modrm, (uint32_t)(result >> 1));
    set_carry_flag(emu, result & 1);
}

static void rcr_rm32_imm8(Emulator *emu, ModRM *modrm)
{
    uint64_t rm32_val = (uint64_t)get_rm32(emu, modrm);
    uint8_t imm8_val = adjust_imm8_for_rm32(get_code8(emu, 0));
    emu->eip += 1;
    uint64_t carry_bit = (uint64_t)is_carry(emu);

    uint64_t cf_added = rm32_val | (carry_bit << 32);
    uint64_t result = (cf_added >> imm8_val) | (cf_added << (33 - imm8_val));
    set_rm32(emu, modrm, (uint32_t)(result));
    set_carry_flag(emu, ((result & 0x100000000) != 0));
}

static void shl_rm32_imm8(Emulator *emu, ModRM *modrm)
{
    uint64_t rm32_val = (uint64_t)get_rm32(emu, modrm);
    uint8_t imm8_val = adjust_imm8_for_rm32(get_code8(emu, 0));
    emu->eip += 1;

    uint64_t result = rm32_val << imm8_val;
    set_rm32(emu, modrm, (uint32_t)result);
    if (imm8_val > 0)
    {
        set_carry_flag(emu, ((result & 0x100000000) != 0));
    }
}

static void shr_rm32_imm8(Emulator *emu, ModRM *modrm)
{
    uint64_t rm32_val = (uint64_t)get_rm32(emu, modrm);
    uint8_t imm8_val = adjust_imm8_for_rm32(get_code8(emu, 0));
    emu->eip += 1;
    uint64_t carry_bit = (uint64_t)is_carry(emu);

    uint64_t cf_added = (rm32_val << 1) | carry_bit;
    uint64_t result = cf_added >> imm8_val;
    set_rm32(emu, modrm, (uint32_t)(result >> 1));
    if (imm8_val > 0)
    {
        set_carry_flag(emu, result & 1);
    }
}

static void sar_rm32_imm8(Emulator *emu, ModRM *modrm)
{
    /* convert to signed and shift with cf */
    int64_t rm32_val = (int32_t)get_rm32(emu, modrm);
    uint8_t imm8_val = adjust_imm8_for_rm32(get_code8(emu, 0));
    emu->eip += 1;
    int64_t carry_bit = (int64_t)is_carry(emu);

    int64_t cf_added = (rm32_val << 1) | carry_bit;
    int64_t result = cf_added >> imm8_val;
    set_rm32(emu, modrm, (uint32_t)(result >> 1));
    if (imm8_val > 0)
    {
        set_carry_flag(emu, result & 1);
    }
}

void code_c01(Emulator *emu)
{
    /* Proceed 1 byte for op code C1. */
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);

    switch (modrm.opcode)
    {
    case 0:
        rol_rm32_imm8(emu, &modrm);
        break;
    case 1:
        ror_rm32_imm8(emu, &modrm);
        break;
    case 2:
        rcl_rm32_imm8(emu, &modrm);
        break;
    case 3:
        rcr_rm32_imm8(emu, &modrm);
        break;
    case 4:
        shl_rm32_imm8(emu, &modrm); /* shl or sal: technically the same. */
        break;
    case 5:
        shr_rm32_imm8(emu, &modrm);
        break;
    case 6:
        shr_rm32_imm8(emu, &modrm); /* shr or sar: undefined? */
        break;
    case 7:
        sar_rm32_imm8(emu, &modrm);
        break;
    default:
        printf("Not implemented: Op: C1 with ModR/M Op: %d\n", modrm.opcode);
        exit(1);
    }
}

/* 
 * ret: 1 byte
 * Jumps to the address pushed by call (address after call instruction).
 * 1 byte: op (C3)
 */
void ret(Emulator *emu)
{
    emu->eip = pop32(emu);
}

/*
 * mov rm8 imm8: 3/4 bytes
 * Copies imm8 value to register or memory specified by ModR/M(rm8).
 * 1 byte: op (C6)
 * 1/2 byte: ModR/M
 * 1 byte: imm8 value
 */
void mov_rm8_imm8(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint8_t value = get_code8(emu, 0);
    emu->eip += 1;
    set_rm8(emu, &modrm, value);
}

/*
 * mov rm32 imm32: 5 bytes
 * Copies imm value to register or memory specified by ModR/M (rm32).
 * 1 byte: op (C7)
 * 1 byte: ModR/M
 * 4 bytes: value (32)
 */
void mov_rm32_imm32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t value = get_code32(emu, 0);
    emu->eip += 4;
    set_rm32(emu, &modrm, value);
}

/*
 * leave: 1byte
 * Set of mov esp, ebp & pop ebp.
 * 1 byte: op (C9)
 */
void leave(Emulator *emu)
{
    uint32_t ebp_val = get_register32(emu, EBP);
    /* Update ESP with current EBP value. */
    set_register32(emu, ESP, ebp_val);
    /* Pop from stack and set it on EBP. */
    set_register32(emu, EBP, pop32(emu));
    emu->eip += 1;
}
