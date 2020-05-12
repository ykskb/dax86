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
        int diff = is_flag(emu) ? get_sign_code32(emu, 1) : 0; \
        emu->eip += (diff + 6);                                \
    }                                                          \
    void jn##flag##32(Emulator * emu)                          \
    {                                                          \
        int diff = is_flag(emu) ? 0 : get_sign_code32(emu, 1); \
        emu->eip += (diff + 6);                                \
    }

DEFINE_NEAR_JX(c, is_carry)
DEFINE_NEAR_JX(z, is_zero)