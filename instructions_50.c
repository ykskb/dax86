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
 * push r32: 1 bytes
 * Pushes 32-bit value into memory stack from specified register.
 * 1 byte: op (50) + reg
 */
void push_r32(Emulator *emu)
{
    u_int8_t reg = get_code8(emu, 0) - 0x50;
    push32(emu, get_register32(emu, reg));
    emu->eip += 1;
}

/*
 * pop r32: 1 bytes
 * Pops 32-bit value into specified register from memory.
 * 1 byte: op (58) + reg
 */
void pop_r32(Emulator *emu)
{
    u_int8_t reg = get_code8(emu, 0) - 0x58;
    set_register32(emu, reg, pop32(emu));
    emu->eip += 1;
}