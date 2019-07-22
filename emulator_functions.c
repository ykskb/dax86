#include <stdint.h>

#include "emulator_functions.h"

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

void set_register32(Emulator *emu, int reg_index, uint32_t value)
{
    emu->registers[reg_index] = value;
}

uint32_t get_register32(Emulator *emu, int reg_index)
{
    return emu->registers[reg_index];
}

void push32(Emulator *emu, uint32_t value)
{
    /* New address would be ESP value - 4 bytes.  */
    uint32_t address = get_register32(emu, ESP) - 4;
    /* Updates ESP with the new address. */
    set_register32(emu, ESP, address);
    set_memory32(emu, address, value);
}

uint32_t pop32(Emulator *emu)
{
    uint32_t address = get_register32(emu, ESP);
    uint32_t value = get_memory32(emu, address);
    set_register32(emu, ESP, address + 4);
    return value;
}

/* Eflag Operations */

void set_carry_flag(Emulator *emu, int is_carry)
{
    /* Higher 32 bits are pass as int */
    if (is_carry)
    {
        emu->eflags |= CARRY_FLAG;
    }
    else
    {
        /* Deflag: AND ~(NOT): NOT 0001 = 1110 AND 1001 -> 1000 */
        emu->eflags &= ~CARRY_FLAG;
    }
}

void set_zero_flag(Emulator *emu, int is_zero)
{
    if (is_zero)
    {
        emu->eflags |= ZERO_FLAG;
    }
    else
    {
        emu->eflags &= ~ZERO_FLAG;
    }
}

void set_sign_flag(Emulator *emu, int is_sign)
{
    if (is_sign)
    {
        emu->eflags |= SIGN_FLAG;
    }
    else
    {
        emu->eflags &= ~SIGN_FLAG;
    }
}

void set_overflow_flag(Emulator *emu, int is_overflow)
{
    if (is_overflow)
    {
        emu->eflags |= OVERFLOW_FLAG;
    }
    else
    {
        emu->eflags &= ~OVERFLOW_FLAG;
    }
}

int32_t is_carry(Emulator *emu)
{
    return (emu->eflags & CARRY_FLAG) != 0;
}

int32_t is_zero(Emulator *emu)
{
    return (emu->eflags & ZERO_FLAG) != 0;
}

int32_t is_sign(Emulator *emu)
{
    return (emu->eflags & SIGN_FLAG) != 0;
}

int32_t is_overflow(Emulator *emu)
{
    return (emu->eflags & OVERFLOW_FLAG) != 0;
}

/* 
 * Overflow might happen only when 2 values have different signs in subtraction.
 * AND overflow changes the sign of value 1 in result. Ex: 4 bit case:
 * 1000 (-8) - 0001 (1) = 10111 (-9)
 * 1000 (-8) - 0010 (2) = 10110 (-10)
 * 0111 (7) - 1111 (-1) = 1000
 */
void update_eflags_sub(Emulator *emu, uint32_t value1, uint32_t value2, uint64_t result)
{
    /* sign bits */
    int sign1 = value1 >> 31;
    int sign2 = value2 >> 31;
    int signr = (result >> 31) & 1;

    set_carry_flag(emu, result >> 32); // higher 32 bits
    set_zero_flag(emu, result == 0);
    set_sign_flag(emu, signr);
    set_overflow_flag(emu, sign1 != sign2 && sign1 != signr);
}