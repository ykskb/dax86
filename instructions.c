#include <stdint.h>
#include <string.h>

#include "instructions.h"
#include "emulator_functions.h"

/*
mov reg 32bit-imm: 5 bytes
1 byte: op (b8) + reg (3bits)
4 bytes: value (32 bit unsigned)
*/
void mov_r32_imm32(Emulator *emu)
{
    uint8_t reg = get_code8(emu, 0) - 0xB8;
    uint32_t value = get_code32(emu, 1);
    emu->registers[reg] = value; // does this map to enum Register?
    emu->eip += 5;
}

/*
jmp (short): 2 bytes
1 byte: op (eb)
1 byte: offset from eip (8 bit signed) -127 ~ 127
*/
void short_jump(Emulator *emu)
{
    int8_t offset = get_sign_code8(emu, 1);
    emu->eip += (offset + 2);
}

/*
jmp (near): 5 bytes
1 byte: op (e9)
4 byte: offset from eip (32 bit signed)
 */
void near_jump(Emulator *emu)
{
    int32_t diff = get_sign_code32(emu, 1);
    emu->eip += (diff + 5);
}

instruction_func_t *instructions[256];

void init_instructions(void)
{
    int i;
    memset(instructions, 0, sizeof(instructions));

    // Why 0xB8 ~ 0xBF: op code includes 8 registers in 1 byte.
    for (i = 0; i < 8; i++)
    {
        instructions[0xB8 + i] = mov_r32_imm32;
    }
    instructions[0xE9] = near_jump;
    instructions[0xEB] = short_jump;
}