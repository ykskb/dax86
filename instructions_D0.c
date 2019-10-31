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

static void rol_rm8_once(Emulator *emu, ModRM *modrm, uint8_t use_cl)
{
    uint8_t n = use_cl ? get_register8(emu, CL) : 1;
    set_rm8(emu, modrm, rol8(get_rm8(emu, modrm), n));
}

static void ror_rm8_once(Emulator *emu, ModRM *modrm, uint8_t use_cl)
{
    uint8_t n = use_cl ? get_register8(emu, CL) : 1;
    set_rm8(emu, modrm, ror8(get_rm8(emu, modrm), n));
}

static void rcl_rm8_once(Emulator *emu, ModRM *modrm, uint8_t use_cl)
{
    uint8_t n = use_cl ? get_register8(emu, CL) : 1;
    ShiftRes8 res = rcl8(get_rm8(emu, modrm), n, (uint8_t)is_carry(emu));
    set_rm8(emu, modrm, res.result);
    set_carry_flag(emu, res.carry);
}

static void rcr_rm8_once(Emulator *emu, ModRM *modrm, uint8_t use_cl)
{
    uint8_t n = use_cl ? get_register8(emu, CL) : 1;
    ShiftRes8 res = rcr8(get_rm8(emu, modrm), n, (uint8_t)is_carry(emu));
    set_rm8(emu, modrm, res.result);
    set_carry_flag(emu, res.carry);
}

static void shl_rm8_once(Emulator *emu, ModRM *modrm, uint8_t use_cl)
{
    uint8_t n = use_cl ? get_register8(emu, CL) : 1;
    ShiftRes8 res = shl8(get_rm8(emu, modrm), n);
    set_rm8(emu, modrm, res.result);
    set_carry_flag(emu, res.carry);
}

static void shr_rm8_once(Emulator *emu, ModRM *modrm, uint8_t use_cl)
{
    uint8_t n = use_cl ? get_register8(emu, CL) : 1;
    ShiftRes8 res = shr8(get_rm8(emu, modrm), n, (uint8_t)is_carry(emu));
    set_rm8(emu, modrm, res.result);
    set_carry_flag(emu, res.carry);
}

static void sar_rm8_once(Emulator *emu, ModRM *modrm, uint8_t use_cl)
{
    uint8_t n = use_cl ? get_register8(emu, CL) : 1;
    ShiftRes8 res = sar8(get_rm8(emu, modrm), n, (uint8_t)is_carry(emu));
    set_rm8(emu, modrm, res.result);
    set_carry_flag(emu, res.carry);
}

void code_d0(Emulator *emu)
{
    /* Proceed 1 byte for op code D0. */
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);

    switch (modrm.opcode)
    {
    case 0:
        rol_rm8_once(emu, &modrm, 0);
        break;
    case 1:
        ror_rm8_once(emu, &modrm, 0);
        break;
    case 2:
        rcl_rm8_once(emu, &modrm, 0);
        break;
    case 3:
        rcr_rm8_once(emu, &modrm, 0);
        break;
    case 4:
        shl_rm8_once(emu, &modrm, 0);
        break;
    case 5:
        shr_rm8_once(emu, &modrm, 0);
        break;
    case 6:
        shr_rm8_once(emu, &modrm, 0);
        break;
    case 7:
        sar_rm8_once(emu, &modrm, 0);
        break;
    default:
        printf("Not implemented: Op: D0 with ModR/M Op: %d\n", modrm.opcode);
        exit(1);
    }
}

static void rol_rm32_once(Emulator *emu, ModRM *modrm, uint8_t use_cl)
{
    uint8_t n = use_cl ? get_register8(emu, CL) : 1;
    set_rm32(emu, modrm, rol32(get_rm32(emu, modrm), n));
}

static void ror_rm32_once(Emulator *emu, ModRM *modrm, uint8_t use_cl)
{
    uint8_t n = use_cl ? get_register8(emu, CL) : 1;
    set_rm32(emu, modrm, ror32(get_rm32(emu, modrm), n));
}

static void rcl_rm32_once(Emulator *emu, ModRM *modrm, uint8_t use_cl)
{
    uint8_t n = use_cl ? get_register8(emu, CL) : 1;
    ShiftRes32 res = rcl32(get_rm32(emu, modrm), n, (uint8_t)is_carry(emu));
    set_rm32(emu, modrm, res.result);
    set_carry_flag(emu, res.carry);
}

static void rcr_rm32_once(Emulator *emu, ModRM *modrm, uint8_t use_cl)
{
    uint8_t n = use_cl ? get_register8(emu, CL) : 1;
    ShiftRes32 res = rcr32(get_rm32(emu, modrm), n, (uint8_t)is_carry(emu));
    set_rm32(emu, modrm, res.result);
    set_carry_flag(emu, res.carry);
}

static void shl_rm32_once(Emulator *emu, ModRM *modrm, uint8_t use_cl)
{
    uint8_t n = use_cl ? get_register8(emu, CL) : 1;
    ShiftRes32 res = shl32(get_rm32(emu, modrm), n);
    set_rm32(emu, modrm, res.result);
    set_carry_flag(emu, res.carry);
}

static void shr_rm32_once(Emulator *emu, ModRM *modrm, uint8_t use_cl)
{
    uint8_t n = use_cl ? get_register8(emu, CL) : 1;
    ShiftRes32 res = shr32(get_rm32(emu, modrm), n, (uint8_t)is_carry(emu));
    set_rm32(emu, modrm, res.result);
    set_carry_flag(emu, res.carry);
}

static void sar_rm32_once(Emulator *emu, ModRM *modrm, uint8_t use_cl)
{
    uint8_t n = use_cl ? get_register8(emu, CL) : 1;
    ShiftRes32 res = sar32(get_rm32(emu, modrm), n, (uint8_t)is_carry(emu));
    set_rm32(emu, modrm, res.result);
    set_carry_flag(emu, res.carry);
}

void code_d1(Emulator *emu)
{
    /* Proceed 1 byte for op code D1. */
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);

    switch (modrm.opcode)
    {
    case 0:
        rol_rm32_once(emu, &modrm, 0);
        break;
    case 1:
        ror_rm32_once(emu, &modrm, 0);
        break;
    case 2:
        rcl_rm32_once(emu, &modrm, 0);
        break;
    case 3:
        rcr_rm32_once(emu, &modrm, 0);
        break;
    case 4:
        shl_rm32_once(emu, &modrm, 0);
        break;
    case 5:
        shr_rm32_once(emu, &modrm, 0);
        break;
    case 6:
        shr_rm32_once(emu, &modrm, 0);
        break;
    case 7:
        sar_rm32_once(emu, &modrm, 0);
        break;
    default:
        printf("Not implemented: Op: D1 with ModR/M Op: %d\n", modrm.opcode);
        exit(1);
    }
}

void code_d2(Emulator *emu)
{
    /* Proceed 1 byte for op code D2. */
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);

    switch (modrm.opcode)
    {
    case 0:
        rol_rm8_once(emu, &modrm, 1);
        break;
    case 1:
        ror_rm8_once(emu, &modrm, 1);
        break;
    case 2:
        rcl_rm8_once(emu, &modrm, 1);
        break;
    case 3:
        rcr_rm8_once(emu, &modrm, 1);
        break;
    case 4:
        shl_rm8_once(emu, &modrm, 1);
        break;
    case 5:
        shr_rm8_once(emu, &modrm, 1);
        break;
    case 6:
        shr_rm8_once(emu, &modrm, 1);
        break;
    case 7:
        sar_rm8_once(emu, &modrm, 1);
        break;
    default:
        printf("Not implemented: Op: D2 with ModR/M Op: %d\n", modrm.opcode);
        exit(1);
    }
}

void code_d3(Emulator *emu)
{
    /* Proceed 1 byte for op code D3. */
    emu->eip += 1;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);

    switch (modrm.opcode)
    {
    case 0:
        rol_rm32_once(emu, &modrm, 1);
        break;
    case 1:
        ror_rm32_once(emu, &modrm, 1);
        break;
    case 2:
        rcl_rm32_once(emu, &modrm, 1);
        break;
    case 3:
        rcr_rm32_once(emu, &modrm, 1);
        break;
    case 4:
        shl_rm32_once(emu, &modrm, 1);
        break;
    case 5:
        shr_rm32_once(emu, &modrm, 1);
        break;
    case 6:
        shr_rm32_once(emu, &modrm, 1);
        break;
    case 7:
        sar_rm32_once(emu, &modrm, 1);
        break;
    default:
        printf("Not implemented: Op: D3 with ModR/M Op: %d\n", modrm.opcode);
        exit(1);
    }
}

void set_al_on_c(Emulator *emu)
{
    if (is_carry(emu))
    {
        set_register8(emu, AL, 0xFF);
    }
    else
    {
        set_register8(emu, AL, 0x0);
    }
}