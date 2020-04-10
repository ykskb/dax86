#include <stdint.h>

#include "emulator_functions.h"
#include "gdt.h"

/* Register Operations */

void set_register8(Emulator *emu, int index, uint8_t value)
{
    if (index < 4)
    {
        /* Resets lowest byte keeping the rest same. */
        uint32_t r = emu->registers[index] & 0xffffff00;
        emu->registers[index] = r | (uint32_t)value;
    }
    else
    {
        /* Resets 2nd byte keeping the rest same. */
        uint32_t r = emu->registers[index - 4] & 0xffff00ff;
        emu->registers[index - 4] = r | ((int32_t)value << 8);
    }
}

uint8_t get_register8(Emulator *emu, int index)
{
    if (index < 4)
    {
        return emu->registers[index] & 0xff;
    }
    else
    {
        /* AH - DH: Offset 8 bit higher. */
        return (emu->registers[index - 4] >> 8) & 0xff;
    }
}

void set_register32(Emulator *emu, int reg_index, uint32_t value)
{
    emu->registers[reg_index] = value;
}

uint32_t get_register32(Emulator *emu, int reg_index)
{
    return emu->registers[reg_index];
}

/* Segment Register Operations */

void set_seg_register16(Emulator *emu, int reg_index, uint16_t value)
{
    emu->segment_registers[reg_index] = value;
}

uint16_t get_seg_register16(Emulator *emu, int reg_index)
{
    return emu->segment_registers[reg_index];
}

void push_segment_register(Emulator *emu, int reg_index)
{
    uint16_t value = get_seg_register16(emu, reg_index);
    push16(emu, value);
}

void pop_segment_register(Emulator *emu, int reg_index)
{
    uint16_t value = pop16(emu);
    set_seg_register16(emu, reg_index, value);
}

/* Control Register Operations */

void check_protected_mode_entry(Emulator *emu)
{
    if (emu->is_pe)
        return;
    uint8_t cr0_pe = (emu->control_registers[CR0] & CR0_PE);
    uint16_t gdt_index = emu->segment_registers[CS] >> 3;
    uint16_t gdt_entry_count = (emu->gdtr.limit + 1) / 8;
    if (cr0_pe && gdt_index != 0 && gdt_entry_count > gdt_index)
        emu->is_pe = 1;
}

static void check_paging(Emulator *emu)
{
    if (emu->is_pg)
        return;
    if ((emu->control_registers[CR0] & CR0_PG) != 0)
        emu->is_pg = 1;
}

void set_ctrl_register32(Emulator *emu, int reg_index, uint32_t value)
{
    emu->control_registers[reg_index] = value;
}

uint32_t get_ctrl_register32(Emulator *emu, int reg_index)
{
    return emu->control_registers[reg_index];
}

/* Physical Memory Operations */

static uint32_t get_physical_address(Emulator *emu, int seg_index, uint32_t offset, uint8_t write)
{
    uint8_t exec = 0;
    if (seg_index == CS)
    {
        exec = 1;
    }

    uint16_t seg_val = get_seg_register16(emu, seg_index);

    if (emu->is_pe)
    {
        uint32_t linear = get_linear_addr(emu, seg_val, offset, write, exec);
        check_paging(emu);
        if (emu->is_pg)
        {
        }
        else
        {
            return linear;
        }
    }
    /* Real mode */
    uint32_t p_base = ((uint32_t)seg_val) << 4;
    return p_base + offset;
}

void _set_memory8(Emulator *emu, uint32_t p_address, uint8_t value)
{
    emu->memory[p_address] = value & 0xFF;
}

void _set_memory16(Emulator *emu, uint32_t p_address, uint16_t value)
{
    int i;
    for (i = 0; i < 2; i++)
    {
        /* passes right-most 8 bits first (little endian) */
        _set_memory8(emu, p_address + i, value >> (i * 8));
    }
}

void _set_memory32(Emulator *emu, uint32_t p_address, uint32_t value)
{
    int i;
    for (i = 0; i < 4; i++)
    {
        /* passes right-most 8 bits first (little endian) */
        _set_memory8(emu, p_address + i, value >> (i * 8));
    }
}

static uint8_t _get_memory8(Emulator *emu, uint32_t p_address)
{
    return emu->memory[p_address];
}

static uint16_t _get_memory16(Emulator *emu, uint32_t p_address)
{
    int i;
    uint16_t mem_read = 0;
    for (i = 0; i < 2; i++)
    {
        /* little endian: first to far right */
        mem_read |= _get_memory8(emu, p_address + i) << (8 * i);
    }
    return mem_read;
}

uint32_t _get_memory32(Emulator *emu, uint32_t p_address)
{
    int i;
    uint32_t mem_read = 0;
    for (i = 0; i < 4; i++)
    {
        mem_read |= _get_memory8(emu, p_address + i) << (8 * i);
    }
    return mem_read;
}

/* Source Instruction Operations */

/* Retrieves code from memory by offset from EIP. */
uint8_t get_code8(Emulator *emu, int index)
{
    uint32_t p_addr = get_physical_address(emu, CS, emu->eip, 0);
    return emu->memory[p_addr + index];
}

int8_t get_sign_code8(Emulator *emu, int index)
{
    return (int8_t)get_code8(emu, index);
}

uint16_t get_code16(Emulator *emu, int index)
{
    int i;
    uint32_t code = 0;
    /* i386 uses little endian (lower bytes to right). */
    for (i = 0; i < 2; i++)
    {
        /* Shift 8 bits to left per byte read */
        code |= get_code8(emu, index + i) << (i * 8);
    }
    return code;
}

int16_t get_sign_code16(Emulator *emu, int index)
{
    return (int16_t)get_code16(emu, index);
}

uint32_t get_code32(Emulator *emu, int index)
{
    int i;
    uint32_t code = 0;
    for (i = 0; i < 4; i++)
    {
        code |= get_code8(emu, index + i) << (i * 8);
    }
    return code;
}

int32_t get_sign_code32(Emulator *emu, int index)
{
    return (int32_t)get_code32(emu, index);
}

/* Memory Operations with Segment Registers */

void set_memory8(Emulator *emu, int seg_index, uint32_t address, uint32_t value)
{
    uint32_t p_address = get_physical_address(emu, seg_index, address, 1);
    _set_memory8(emu, p_address, value);
}

void set_memory16(Emulator *emu, int seg_index, uint32_t address, uint16_t value)
{
    uint32_t p_address = get_physical_address(emu, seg_index, address, 1);
    _set_memory16(emu, p_address, value);
}

void set_memory32(Emulator *emu, int seg_index, uint32_t address, uint32_t value)
{
    uint32_t p_address = get_physical_address(emu, seg_index, address, 1);
    _set_memory32(emu, p_address, value);
}

uint8_t get_memory8(Emulator *emu, int seg_index, uint32_t address)
{
    uint32_t p_address = get_physical_address(emu, seg_index, address, 0);
    return _get_memory8(emu, p_address);
}

uint16_t get_memory16(Emulator *emu, int seg_index, uint32_t address)
{
    uint32_t p_address = get_physical_address(emu, seg_index, address, 0);
    return _get_memory16(emu, p_address);
}

uint32_t get_memory32(Emulator *emu, int seg_index, uint32_t address)
{
    uint32_t p_address = get_physical_address(emu, seg_index, address, 0);
    return _get_memory32(emu, p_address);
}

/* Stack Operations */

void push16(Emulator *emu, uint16_t value)
{
    /* New offset would be ESP value - 2 bytes.  */
    uint32_t offset = get_register32(emu, ESP) - 2;
    set_register32(emu, ESP, offset);
    uint32_t p_address = get_physical_address(emu, SS, offset, 1);
    _set_memory16(emu, p_address, value);
}

void push32(Emulator *emu, uint32_t value)
{
    /* New offset would be ESP value - 4 bytes.  */
    uint32_t offset = get_register32(emu, ESP) - 4;
    /* Updates ESP with the new address. */
    set_register32(emu, ESP, offset);
    uint32_t p_address = get_physical_address(emu, SS, offset, 1);
    _set_memory32(emu, p_address, value);
}

uint16_t pop16(Emulator *emu)
{
    uint32_t offset = get_register32(emu, ESP);
    uint32_t p_address = get_physical_address(emu, SS, offset, 0);
    uint16_t value = _get_memory16(emu, p_address);
    set_register32(emu, ESP, offset + 2);
    return value;
}

uint32_t pop32(Emulator *emu)
{
    uint32_t offset = get_register32(emu, ESP);
    uint32_t p_address = get_physical_address(emu, SS, offset, 0);
    uint32_t value = _get_memory32(emu, p_address);
    set_register32(emu, ESP, offset + 4);
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

void set_int_flag(Emulator *emu, int is_enabled)
{
    if (is_enabled)
    {
        emu->eflags |= INT_ENABLE_FLAG;
    }
    else
    {
        emu->eflags &= ~INT_ENABLE_FLAG;
    }
}

void set_direction_flag(Emulator *emu, int is_down)
{
    if (is_down)
    {
        emu->eflags |= DIRECTION_FLAG;
    }
    else
    {
        emu->eflags &= ~DIRECTION_FLAG;
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

int32_t is_int_enabled(Emulator *emu)
{
    return (emu->eflags & INT_ENABLE_FLAG) != 0;
}

int32_t is_direction_down(Emulator *emu)
{
    return (emu->eflags & DIRECTION_FLAG) != 0;
}

int32_t is_overflow(Emulator *emu)
{
    return (emu->eflags & OVERFLOW_FLAG) != 0;
}

void update_eflags_add(Emulator *emu, uint32_t value1, uint32_t value2, uint64_t result)
{
    /* sign bits */
    int sign1 = value1 >> 31;
    int sign2 = value2 >> 31;
    int signr = (result >> 31) & 1;

    /* 1111 1111 (255) + 1111 1111 (255) = 1 1111 1110 (510)*/
    set_carry_flag(emu, result >> 32);
    /* 
     * For assumed signed numbers.
     * Overflow: Most significant bit (sign bit) is changed. 
     * Happens only with +n + +n or +n - -n.
     * 0111 1111 (127) + 0111 1111 (127) = 1111 1110 (254)
    */
    set_overflow_flag(emu, sign1 == sign2 && sign1 != signr);
    set_sign_flag(emu, signr);
}

void update_eflags_add_8bit(Emulator *emu, uint8_t value1, uint8_t value2, uint16_t result)
{
    int sign1 = value1 >> 7;
    int sign2 = value2 >> 7;
    int signr = (result >> 7) & 1;
    set_carry_flag(emu, result >> 8);
    set_overflow_flag(emu, sign1 == sign2 && sign1 != signr);
    set_sign_flag(emu, signr);
}

/* 
 * 1000 (-8) - 0001 (1) = 10111 (-9) -> carry
 * 1000 (-8) - 0010 (2) = 10110 (-10) -> carry
 * Overflow might happen only when 2 values have different signs in subtraction.
 * AND overflow changes the sign of value 1 in result. Ex: 4 bit case:
 * 0111 (7) - 1111 (-1) = 1000 (-8) *wrong (sign1 != sign2 && sign1 != signr)* -> overflow
 * 0111 (7) - 1000 (-8) = 1111 (-1) *wrong (sign1 != sign2 && sign1 != signr)* -> overflow
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

/*
 * imul & mul both updates OF and CF when a significant bit gets into upper half.
 */
void update_eflags_mul(Emulator *emu, uint64_t upper_half_result)
{
    if (upper_half_result == 0)
    {
        set_overflow_flag(emu, 0);
        set_carry_flag(emu, 0);
    }
    else
    {
        set_overflow_flag(emu, 1);
        set_carry_flag(emu, 1);
    }
}

void update_eflags_sub_8bit(Emulator *emu, uint8_t value1, uint8_t value2, uint16_t result)
{
    int sign1 = value1 >> 7;
    int sign2 = value2 >> 7;
    int signr = (result >> 7) & 1;

    set_carry_flag(emu, result >> 8); // higher 32 bits
    set_zero_flag(emu, result == 0);
    set_sign_flag(emu, signr);
    set_overflow_flag(emu, sign1 != sign2 && sign1 != signr);
}

void update_eflags_logical_ops(Emulator *emu, uint32_t result)
{
    int signr = (result >> 31) & 1;
    set_carry_flag(emu, 0);
    set_zero_flag(emu, result == 0);
    set_sign_flag(emu, signr);
    set_overflow_flag(emu, 0);
}

void update_eflags_logical_ops_8bit(Emulator *emu, uint8_t result)
{
    int signr = (result >> 7) & 1;
    set_carry_flag(emu, 0);
    set_zero_flag(emu, result == 0);
    set_sign_flag(emu, signr);
    set_overflow_flag(emu, 0);
}

void set_gdtr(Emulator *emu, uint16_t limit, uint32_t base)
{
    emu->gdtr.limit = limit;
    emu->gdtr.base = base;
}