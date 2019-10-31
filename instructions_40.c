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
 * inc r32: 1 byte
 * Increments a value of specified 32-bit register in op code.
 * 1 byte: op (40) + reg index (3 bits)
 */
void inc_r32(Emulator *emu)
{
    uint8_t reg = get_code8(emu, 0) - 0x40;
    uint32_t r32_val = get_register32(emu, reg);
    uint32_t result = r32_val + 1;
    set_register32(emu, reg, result);
    update_eflags_add(emu, r32_val, 1, result);
    emu->eip += 1;
}

/*
 * dec r32: 1 byte
 * Decrements a value of specified 32-bit register in op code.
 * 1 byte: op (48) + reg index (3 bits)
 */
void dec_r32(Emulator *emu)
{
    uint8_t reg = get_code8(emu, 0) - 0x48;
    uint32_t r32_val = get_register32(emu, reg);
    uint32_t result = r32_val - 1;
    set_register32(emu, reg, result);
    update_eflags_sub(emu, r32_val, 1, result);
    emu->eip += 1;
}