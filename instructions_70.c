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
 * Macro for jump with eflag.
 * Replaces the matching text and ## concatenates strings.
 * Defined until unescaped line break.
 */
#define DEFINE_JX(flag, is_flag)                              \
    void j##flag(Emulator *emu)                               \
    {                                                         \
        int diff = is_flag(emu) ? get_sign_code8(emu, 1) : 0; \
        emu->eip += (diff + 2);                               \
    }                                                         \
    void jn##flag(Emulator *emu)                              \
    {                                                         \
        int diff = is_flag(emu) ? 0 : get_sign_code8(emu, 1); \
        emu->eip += (diff + 2);                               \
    }

/*
 * jc (72), jnc (73), jz (74), jnz (75), js (78), jns (79), jo (70), jno (71): 2 bytes
 * 1 byte: op code
 * 1 byte: offset to jump
 */
DEFINE_JX(c, is_carry)
DEFINE_JX(z, is_zero)
DEFINE_JX(s, is_sign)
DEFINE_JX(o, is_overflow)

#undef DEFINE_JX

/*
 * jna (76)
 * Jumps if above (CF=1 or ZF=1)
 * 1 byte: op code
 * 1 byte: offset to jump
 */
void jna(Emulator *emu)
{
    int diff = (is_zero(emu) || is_carry(emu)) ? get_sign_code8(emu, 1) : 0;
    emu->eip += (diff + 2);
}

/*
 * ja (77)
 * Jumps if above (CF=0 and ZF=0)
 * 1 byte: op code
 * 1 byte: offset to jump
 */
void ja(Emulator *emu)
{
    int diff = (!is_zero(emu) && !is_carry(emu)) ? get_sign_code8(emu, 1) : 0;
    emu->eip += (diff + 2);
}

/*
 * jl (7C) and jle (7E)
 * Assuming there's no overflow, which means overflow_flag == 0,
 * if sign_flag is 0, left is bigger. Ex:
 * 3, 2: 3 - 2 = 1 => sign_flag: 0 => larger
 * -3, 2: -3 - (2) = -5 => sign_flag: 1 => smaller
 * -1, -4: -1 - (-4) = 3 => sign_flag: 0 => larger
 */
void jl(Emulator *emu)
{
    int diff = (is_sign(emu) != is_overflow(emu)) ? get_sign_code8(emu, 1) : 0;
    emu->eip += (diff + 2);
}

void jle(Emulator *emu)
{
    int diff = (is_zero(emu) || (is_sign(emu) != is_overflow(emu))) ? get_sign_code8(emu, 1) : 0;
    emu->eip += (diff + 2);
}

/*
 * jg (7f)
 * Jumps if greater (ZF=0 and SF=OF)
 * 1 byte: op code
 * 1 byte: offset to jump
 */
void jg(Emulator *emu)
{
    int diff = (!is_zero(emu) && (is_sign(emu) == is_overflow(emu))) ? get_sign_code8(emu, 1) : 0;
    emu->eip += (diff + 2);
}
