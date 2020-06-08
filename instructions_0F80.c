#include <stdint.h>

#include "instruction_defs.h"
#include "emulator_functions.h"

/*
 * Macro for jump with eflag.
 * Replaces the matching text and ## concatenates strings.
 * Defined until unescaped line break.
 */
#define DEFINE_NEAR_JX(flag, is_flag)                          \
    void j##flag##32(Emulator * emu)                           \
    {                                                          \
        int diff = is_flag(emu) ? get_sign_code32(emu, 2) : 0; \
        emu->eip += (diff + 6);                                \
    }                                                          \
    void jn##flag##32(Emulator * emu)                          \
    {                                                          \
        int diff = is_flag(emu) ? 0 : get_sign_code32(emu, 2); \
        emu->eip += (diff + 6);                                \
    }
/*
 * jc | jnc
 * Jumps near if carry (CF=1) | not carry (CF=0)
 * 2 bytes: op code (0F 82 | 0F 83)
 * 4 bytes: offset to jump
 */
DEFINE_NEAR_JX(c, is_carry)

/*
 * jz | jnz
 * Jumps near if zero (ZF=1) | not zero (ZF=0)
 * 2 bytes: op code (0F 84 | 0F 85)
 * 4 bytes: offset to jump
 */
DEFINE_NEAR_JX(z, is_zero)

/*
 * jna | ja
 * Jumps if not above (CF=1 or ZF=1) | above (CF=0 and ZF=0)
 * 2 byte: op code (0F 86 | 0F 87)
 * 4 bytes: offset to jump
 */
void jna32(Emulator *emu)
{
    int diff = (is_carry(emu) || is_zero(emu)) ? get_sign_code32(emu, 2) : 0;
    emu->eip += (diff + 6);
}

void ja32(Emulator *emu)
{
    int diff = (!is_carry(emu) && !is_zero(emu)) ? get_sign_code32(emu, 2) : 0;
    emu->eip += (diff + 6);
}

/*
 * js | jns
 * Jumps near if sign (SF=1) | not sign (SF=0)
 * 2 bytes: op code (0F 88 | 0F 89)
 * 4 bytes: offset to jump
 */
DEFINE_NEAR_JX(s, is_sign)

/*
 * jge 
 * Jumps if greater or equal (SF=OF)
 * 2 byte: op code (0F 8D)
 * 4 bytes: offset to jump
 */
void jge32(Emulator *emu)
{
    int diff = (is_sign(emu) == is_overflow(emu)) ? get_sign_code32(emu, 2) : 0;
    emu->eip += (diff + 6);
}
/*
 * jng 
 * Jumps if not greater (ZF=1 or SF!=OF)
 * 2 byte: op code (0F 8E)
 * 4 bytes: offset to jump
 */
void jng32(Emulator *emu)
{
    int diff = (is_zero(emu) || (is_sign(emu) != is_overflow(emu))) ? get_sign_code32(emu, 2) : 0;
    emu->eip += (diff + 6);
}
/*
 * jg 
 * Jumps if greater (ZF=0 and SF=OF)
 * 2 byte: op code (0F 8F)
 * 4 bytes: offset to jump
 */
void jg32(Emulator *emu)
{
    int diff = (!is_zero(emu) && (is_sign(emu) == is_overflow(emu))) ? get_sign_code32(emu, 2) : 0;
    emu->eip += (diff + 6);
}