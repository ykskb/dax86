#include <stdint.h>

#include "instruction_defs.h"
#include "emulator_functions.h"
#include "modrm.h"

/*
 * mov r32 rm8: 2 bytes
 * Copies value of zero-extended rm8 value to r32.
 * 2 byte: op (0FB6)
 * 1 byte: ModR/M
 */
void movzx_r32_rm8(Emulator *emu)
{
    emu->eip += 2;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t rm16 = 0 | get_rm8(emu, &modrm);
    set_r32(emu, &modrm, rm16);
}

/*
 * mov r32 rm16: 2 bytes
 * Copies value of zero-extended rm16 value to r32.
 * 2 byte: op (0FB7)
 * 1 byte: ModR/M
 */
void movzx_r32_rm16(Emulator *emu)
{
    emu->eip += 2;
    ModRM modrm = create_modrm();
    parse_modrm(emu, &modrm);
    uint32_t rm16 = 0 | get_rm16(emu, &modrm);
    set_r32(emu, &modrm, rm16);
}
