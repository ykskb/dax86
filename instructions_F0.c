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
void inc_rm32(Emulator *emu, ModRM *modrm)
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