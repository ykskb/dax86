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

void code_0f_01(Emulator *emu)
{
    emu->eip += 2;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);

    switch (modrm.opcode)
    {
    case 2:
        switch (modrm.rm)
        {
        case 0:
            /* ModRM D0: xgetbv */
            break;
        case 1:
            /* ModRM D1: xsetbv */
            break;
        default:
            lgdt(emu, &modrm);
            break;
        }
        break;
    default:
        printf("Not implemented: Op: 0F01 with ModR/M Op: %d\n", modrm.opcode);
        exit(1);
    }
}