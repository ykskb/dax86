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
    set_register32(emu, reg, get_register32(emu, reg) + 1);
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
    set_register32(emu, reg, get_register32(emu, reg) - 1);
    emu->eip += 1;
}