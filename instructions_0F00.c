#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "instruction_defs.h"
#include "emulator_functions.h"
#include "modrm.h"
#include "io.h"
#include "gdt.h"
#include "interrupt.h"

void code_0f_00(Emulator *emu)
{
    emu->eip += 2;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);

    switch (modrm.opcode)
    {
    /* ltr */
    case 3:
        set_tr(emu, get_rm16(emu, &modrm));
        break;
    default:
        printf("Not implemented: Op: 0F00 with ModR/M Op: %d\n", modrm.opcode);
        exit(1);
    }
}

void code_0f_01(Emulator *emu)
{
    emu->eip += 2;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);

    switch (modrm.opcode)
    {
    case 2:
        lgdt(emu, &modrm);
        break;
    case 3:
        lidt(emu, &modrm);
        break;
    default:
        printf("Not implemented: Op: 0F01 with ModR/M Op: %d\n", modrm.opcode);
        exit(1);
    }
}

/*
 * push fs: 2 bytes
 * Pushes FS register to stack.
 * 2 bytes: op (0F A0)
 */
void push_fs(Emulator *emu)
{
    push_segment_register(emu, FS);
    emu->eip += 2;
}

/*
 * pop fs: 2 bytes
 * Pops FS register to stack.
 * 2 bytes: op (0F A1)
 */
void pop_fs(Emulator *emu)
{
    pop_segment_register(emu, FS);
    emu->eip += 2;
}

/*
 * push gs: 2 bytes
 * Pushes GS register to stack.
 * 2 bytes: op (0F A8)
 */
void push_gs(Emulator *emu)
{
    push_segment_register(emu, GS);
    emu->eip += 2;
}

/*
 * pop gs: 2 bytes
 * Pops GS register to stack.
 * 2 bytes: op (0F A9)
 */
void pop_gs(Emulator *emu)
{
    pop_segment_register(emu, GS);
    emu->eip += 2;
}