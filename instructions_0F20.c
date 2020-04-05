#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "instruction_defs.h"
#include "emulator_functions.h"
#include "modrm.h"

void mov_r32_cr(Emulator *emu)
{
    emu->eip += 2;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t cr_val = get_ctrl_register32(emu, modrm.reg_index);
    set_register32(emu, modrm.rm, cr_val);
}

void mov_cr_r32(Emulator *emu)
{
    emu->eip += 2;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t r32_val = get_register32(emu, modrm.rm);
    set_ctrl_register32(emu, modrm.reg_index, r32_val);
}