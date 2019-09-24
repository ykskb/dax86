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
 * push imm32: 5 bytes
 * Pushes 32-bit immediate value into stack.
 * 1 byte: op (68)
 * 4 bytes: immediate 32-bit value
 */
void push_imm32(Emulator *emu)
{
    uint32_t value = get_code32(emu, 1);
    push32(emu, value);
    emu->eip += 5;
}

/*
 * push imm8: 2 bytes
 * Pushes 8-bit immediate value into stack.
 * 1 byte: op (6A)
 * 1 byte: immediate 8-bit value
 */
void push_imm8(Emulator *emu)
{
    uint32_t value = get_code8(emu, 1);
    push32(emu, value);
    emu->eip += 2;
}