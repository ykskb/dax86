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
 * mov r8 imm8: 2 bytes
 * Copies 8-bit imm value to register specified in op code.
 * 1 byte: op (B0) + reg index (3 bits)
 * 1 byte: value (8-bit unsigned)
 */
void mov_r8_imm8(Emulator *emu)
{
    uint8_t reg = get_code8(emu, 0) - 0xB0;
    set_register8(emu, reg, get_code8(emu, 1));
    emu->eip += 2;
}

/*
 * mov r32 imm32: 5 bytes
 * Copies imm value to register specified in op code (r32: 32 bit register).
 * 1 byte: op (B8) + reg index (3bits)
 * 4 bytes: value (32 bit unsigned)
 */
void mov_r32_imm32(Emulator *emu)
{
    uint8_t reg = get_code8(emu, 0) - 0xB8;
    uint32_t value = get_code32(emu, 1);
    emu->registers[reg] = value; // maps to Register Enum
    emu->eip += 5;
}