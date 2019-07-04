#include <stdint.h>

#include "emulator.h"

/* Retrieves code from memory by offset from EIP. */
uint32_t get_code8(Emulator *emu, int index)
{
    return emu->memory[emu->eip + index];
}

int32_t get_sign_code8(Emulator *emu, int index)
{
    return (int8_t)emu->memory[emu->eip + index];
}

uint32_t get_code32(Emulator *emu, int index)
{
    int i;
    uint32_t code = 0;
    /* i386 uses little endian (lower bytes to right). */
    for (i = 0; i < 4; i++)
    {
        /* Shift 8 bits to left per byte read */
        code |= get_code8(emu, index + i) << (i * 8);
    }
    return code;
}

int32_t get_sign_code32(Emulator *emu, int index)
{
    return (int32_t)get_code32(emu, index);
}