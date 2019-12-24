#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "instruction_defs.h"
#include "emulator_functions.h"
#include "modrm.h"
#include "io.h"
#include "shift.h"

static uint8_t adjust_imm8_for_rm8(uint8_t val)
{
    while (val > 8)
    {
        val -= 8;
    }
    return val;
}

static void rol_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t imm8_val = adjust_imm8_for_rm8(get_code8(emu, 0));
    emu->eip += 1;

    set_rm8(emu, modrm, rol8(get_rm8(emu, modrm), imm8_val));
}

static void ror_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t imm8_val = adjust_imm8_for_rm8(get_code8(emu, 0));
    emu->eip += 1;

    set_rm8(emu, modrm, ror8(get_rm8(emu, modrm), imm8_val));
}

static void rcl_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t imm8_val = adjust_imm8_for_rm8(get_code8(emu, 0));
    emu->eip += 1;

    ShiftRes8 res = rcl8(get_rm8(emu, modrm), imm8_val, (uint8_t)is_carry(emu));
    set_rm8(emu, modrm, res.result);
    set_carry_flag(emu, res.carry);
}

static void rcr_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t imm8_val = adjust_imm8_for_rm8(get_code8(emu, 0));
    emu->eip += 1;

    ShiftRes8 res = rcr8(get_rm8(emu, modrm), imm8_val, (uint8_t)is_carry(emu));
    set_rm8(emu, modrm, res.result);
    set_carry_flag(emu, res.carry);
}

static void shl_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t imm8_val = adjust_imm8_for_rm8(get_code8(emu, 0));
    emu->eip += 1;

    ShiftRes8 res = shl8(get_rm8(emu, modrm), imm8_val);
    set_rm8(emu, modrm, res.result);
    set_carry_flag(emu, res.carry);
}

static void shr_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t imm8_val = adjust_imm8_for_rm8(get_code8(emu, 0));
    emu->eip += 1;

    ShiftRes8 res = shr8(get_rm8(emu, modrm), imm8_val, (uint8_t)is_carry(emu));
    set_rm8(emu, modrm, res.result);
    set_carry_flag(emu, res.carry);
}

static void sar_rm8_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t imm8_val = adjust_imm8_for_rm8(get_code8(emu, 0));
    emu->eip += 1;

    ShiftRes8 res = sar8(get_rm8(emu, modrm), imm8_val, (uint8_t)is_carry(emu));
    set_rm8(emu, modrm, res.result);
    set_carry_flag(emu, res.carry);
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
    uint8_t imm8_val = adjust_imm8_for_rm32(get_code8(emu, 0));
    emu->eip += 1;

    set_rm32(emu, modrm, rol32(get_rm32(emu, modrm), imm8_val));
}

static void ror_rm32_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t imm8_val = adjust_imm8_for_rm32(get_code8(emu, 0));
    emu->eip += 1;

    set_rm32(emu, modrm, ror32(get_rm32(emu, modrm), imm8_val));
}

static void rcl_rm32_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t imm8_val = adjust_imm8_for_rm32(get_code8(emu, 0));
    emu->eip += 1;

    ShiftRes32 res = rcl32(get_rm32(emu, modrm), imm8_val, (uint8_t)is_carry(emu));
    set_rm32(emu, modrm, res.result);
    set_carry_flag(emu, res.carry);
}

static void rcr_rm32_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t imm8_val = adjust_imm8_for_rm32(get_code8(emu, 0));
    emu->eip += 1;

    ShiftRes32 res = rcr32(get_rm32(emu, modrm), imm8_val, (uint8_t)is_carry(emu));
    set_rm32(emu, modrm, res.result);
    set_carry_flag(emu, res.carry);
}

static void shl_rm32_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t imm8_val = adjust_imm8_for_rm32(get_code8(emu, 0));
    emu->eip += 1;

    ShiftRes32 res = shl32(get_rm32(emu, modrm), imm8_val);
    set_rm32(emu, modrm, res.result);
    set_carry_flag(emu, res.carry);
}

static void shr_rm32_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t imm8_val = adjust_imm8_for_rm32(get_code8(emu, 0));
    emu->eip += 1;

    ShiftRes32 res = shr32(get_rm32(emu, modrm), imm8_val, (uint8_t)is_carry(emu));
    set_rm32(emu, modrm, res.result);
    set_carry_flag(emu, res.carry);
}

static void sar_rm32_imm8(Emulator *emu, ModRM *modrm)
{
    uint8_t imm8_val = adjust_imm8_for_rm32(get_code8(emu, 0));
    emu->eip += 1;

    ShiftRes32 res = sar32(get_rm32(emu, modrm), imm8_val, (uint8_t)is_carry(emu));
    set_rm32(emu, modrm, res.result);
    set_carry_flag(emu, res.carry);
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

/*
 * ret far: 1 byte
 * Jumps to the address pushed by call (EIP & CS).
 * 1 byte: op (CB)
 */
void ret_far(Emulator *emu)
{
    emu->eip = pop32(emu);
    set_seg_register16(emu, CS, pop16(emu));
}