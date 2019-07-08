#include <stdint.h>

#include "emulator.h"

/* Source Instruction Operations */

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

/* Memory Operations */

void set_memory8(Emulator *emu, uint32_t address, uint32_t value)
{
    /* write last 8 bits */
    emu->memory[address] = value & 0xFF;
}

void set_memory32(Emulator *emu, uint32_t address, uint32_t value)
{
    int i;
    for (i = 0; i < 4; i++)
    {
        /* passes right-most 8 bits first (little endian) */
        set_memory8(emu, address + i, value >> (i * 8));
    }
}

uint32_t get_memory8(Emulator *emu, uint32_t address)
{
    return emu->memory[address];
}

uint32_t get_memory32(Emulator *emu, uint32_t address)
{
    int i;
    uint32_t mem_read = 0;
    for (i = 0; i < 4; i++)
    {
        /* little endian: first to far right */
        mem_read |= get_memory8(emu, address + i) << (8 * i);
    }
    return mem_read;
}

/* Register Operations */

void set_register32(Emulator *emu, int index, uint32_t value)
{
    emu->registers[index] = value;
}

uint32_t get_register32(Emulator *emu, int index)
{
    return emu->registers[index];
}