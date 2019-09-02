#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "emulator_functions.h"
#include "modrm.h"

/*
 * Mod R/M: 1 byte
 * 0 1 | 0 0 0 | 1 0 1
 * Mod |  REG  |  R/M
 *
 *                        |  REG  |EAX |ECX |EDX |EBX |ESP |EBP |ESI |EDI |
 *                        | (r32) |000 |001 |010 |011 |100 |101 |110 |111 |
 * | Mod | R/M |    Addressing    |          Mod R/M Value in Hex         |
 * | 00  | 000 | [eax]            | 00 | 08 | 10 | 18 | 20 | 28 | 30 | 38 |
 * |     | 001 | [ecx]            | 01 | 09 | 11 | 19 | 21 | 29 | 31 | 39 |
 * |     | 010 | [edx]            | 02 | 0A | 12 | 1A | 22 | 2A | 32 | 3A |
 * |     | 011 | [ebx]            | 03 | 0B | 13 | 1B | 23 | 2B | 33 | 3B |
 * |     | 100 | [-][-]           | 04 | 0C | 14 | 1C | 24 | 2C | 34 | 3C |
 * |     | 101 | disp32           | 05 | 0D | 15 | 1D | 25 | 2D | 35 | 3D |
 * |     | 110 | [esi]            | 06 | 0E | 16 | 1E | 26 | 2E | 36 | 3E |
 * |     | 111 | [edi]            | 07 | 0F | 17 | 1F | 27 | 2F | 37 | 3F |
 * | 01  | 000 | [eax] + disp8    | 40 | 48 | 50 | 58 | 60 | 68 | 70 | 78 |
 * ...
 * | 10  | 000 | [eax] + disp32   | 80 | 88 | 90 | 98 | A0 | A8 | B0 | B8 |
 * ...
 * | 11  | 000 | eax              | C0 | C8 | D0 | D8 | E0 | E8 | F0 | F8 |
 */

void parse_modrm(Emulator *emu, ModRM *modrm)
{
    uint8_t code;

    memset(modrm, 0, sizeof(ModRM));

    code = get_code8(emu, 0);

    /* Mod (2 bits) Reg (3 bits) R/M (3 bits) */
    modrm->mod = ((code & 0xC0) >> 6);
    modrm->opcode = ((code & 0x38) >> 3);
    modrm->rm = code & 0x07;

    emu->eip += 1;

    /*
     * SIB after ModR/M Byte
     * mod/RM:
     * 00/100: [-][-]
     * 01/100: [-][-] + disp8
     * 10/100: [-][-] + disp32
     */
    if (modrm->mod != 3 && modrm->rm == 4)
    {
        modrm->sib = get_code8(emu, 0);
        emu->eip += 1;
    }

    /*
     * Stores disp32.
     * mod/RM:
     * 00/101: disp32
     * 10/[000 - 111]: [reg] + disp32
     */
    if ((modrm->mod == 0 && modrm->rm == 5) || modrm->mod == 2)
    {
        modrm->disp32 = get_sign_code32(emu, 0);
        emu->eip += 4;
    }
    else if (modrm->mod == 1)
    {
        /*
         * Stores disp8.
         * mod/RM:
         * 01/[000 - 111]: [reg] + disp8
         */
        modrm->disp8 = get_sign_code8(emu, 0);
        emu->eip += 1;
    }
}

uint32_t calc_memory_address(Emulator *emu, ModRM *modrm)
{
    /* Mod:0 [reg] */
    if (modrm->mod == 0)
    {
        /* SIB not implemented (Mod: 00 R/M: 100) */
        if (modrm->rm == 4)
        {
            printf("Not implemented: ModRM with Mod: 00 and RM: 100\n");
            exit(0);
        }
        /* Mod:0 R/M: 101 uses disp32 as memory address. */
        else if (modrm->rm == 5)
        {
            return modrm->disp32;
        }
        /* Mod:0 R/M: 000 - 011, 110, 111 uses [reg] */
        else
        {
            return get_register32(emu, modrm->rm);
        }
    }
    /* Mod: 01 [reg] + disp8 */
    else if (modrm->mod == 1)
    {
        /* SIB not implemented (Mod: 01 R/M: 100) */
        if (modrm->rm == 4)
        {
            printf("Not implemented: ModRM with Mod: 01 and RM: 100\n");
            exit(0);
        }
        /* Mod: 1 R/M: 000 - 011, 101 - 111 uses [reg] + disp8 */
        else
        {
            return get_register32(emu, modrm->rm) + modrm->disp8;
        }
    }
    /* Mod: 10 [reg] + disp32 */
    else if (modrm->mod == 2)
    {
        /* SIB not implemented (MOD: 10 R/M: 100) */
        if (modrm->rm == 4)
        {
            printf("Not implemented: ModRM with Mod: 10 and RM: 100\n");
            exit(0);
        }
        /* Mod: 10 R/M: 000 - 011, 101 - 111 uses [reg] + disp32 */
        else
        {
            return get_register32(emu, modrm->rm) + modrm->disp32;
        }
    }
    else
    {
        printf("ModRM with mod: 00 - 10 are only implemented.");
        exit(0);
    }
}

void set_rm8(Emulator *emu, ModRM *modrm, uint8_t value)
{
    /* Mod:11 is for registers directly. */
    if (modrm->mod == 3)
    {
        set_register8(emu, modrm->rm, value);
    }
    else
    {
        /*
         * Mod: Pattern 
         * 00: [eax], [ecx]...
         * 01: [eax] + disp8, [ecx] + disp8...
         * 10: [eax] + disp32 etc, [ecx] + disp32...
         */
        uint32_t address = calc_memory_address(emu, modrm);
        set_memory8(emu, address, value);
    }
}

uint8_t get_rm8(Emulator *emu, ModRM *modrm)
{
    if (modrm->mod == 3)
    {
        return get_register8(emu, modrm->rm);
    }
    else
    {
        uint32_t address = calc_memory_address(emu, modrm);
        return get_memory8(emu, address);
    }
}

void set_rm32(Emulator *emu, ModRM *modrm, uint32_t value)
{
    /* Mod:11 is for registers directly. */
    if (modrm->mod == 3)
    {
        set_register32(emu, modrm->rm, value);
    }
    else
    {
        /*
         * Mod: Pattern 
         * 00: [eax], [ecx]...
         * 01: [eax] + disp8, [ecx] + disp8...
         * 10: [eax] + disp32 etc, [ecx] + disp32...
         */
        uint32_t address = calc_memory_address(emu, modrm);
        set_memory32(emu, address, value);
    }
}

uint32_t get_rm32(Emulator *emu, ModRM *modrm)
{
    if (modrm->mod == 3)
    {
        return get_register32(emu, modrm->rm);
    }
    else
    {
        uint32_t address = calc_memory_address(emu, modrm);
        return get_memory32(emu, address);
    }
}

void set_r8(Emulator *emu, ModRM *modrm, uint8_t value)
{
    set_register8(emu, modrm->reg_index, value);
}

uint8_t get_r8(Emulator *emu, ModRM *modrm)
{
    return get_register8(emu, modrm->reg_index);
}

void set_r32(Emulator *emu, ModRM *modrm, uint32_t value)
{
    set_register32(emu, modrm->reg_index, value);
}

uint32_t get_r32(Emulator *emu, ModRM *modrm)
{
    return get_register32(emu, modrm->reg_index);
}