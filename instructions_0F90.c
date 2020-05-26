#include <stdint.h>

#include "instruction_defs.h"
#include "emulator_functions.h"

/*
 * sete
 * Sets rm8 to 1 if zero flag is set, otherwise 0.
 * 2 byte: op code (0F 94)
 * 1|2 bytes: ModR/M
 */
void sete(Emulator *emu)
{
    emu->eip += 2;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    if (is_zero(emu))
    {
        set_rm8(emu, &modrm, 1);
    }
    else
    {
        set_rm8(emu, &modrm, 0);
    }
}