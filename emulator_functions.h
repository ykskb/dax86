#ifndef EMULATOR_FUNCTIONS_H_
#define EMULATOR_FUNCTIONS_H_

#include <stdint.h>

#include "emulator.h"

/* Eflags */
#define CARRY_FLAG (1)
#define ZERO_FLAG (1 << 6)
#define SIGN_FLAG (1 << 7)
#define INT_ENABLE_FLAG (1 << 9)
#define DIRECTION_FLAG (1 << 10)
#define OVERFLOW_FLAG (1 << 11)

/* Register Operations */

void set_register8(Emulator *emu, int index, uint8_t value);
uint8_t get_register8(Emulator *emu, int index);

void set_register16(Emulator *emu, int reg_index, uint16_t value);
uint16_t get_register16(Emulator *emu, int reg_index);

void set_register32(Emulator *emu, int reg_index, uint32_t value);
uint32_t get_register32(Emulator *emu, int reg_index);

/* Segment Register Operations */

void set_seg_register16(Emulator *emu, int reg_index, uint16_t value);
uint16_t get_seg_register16(Emulator *emu, int reg_index);

/* Control Register Operations */

void set_ctrl_register32(Emulator *emu, int reg_index, uint32_t value);
uint32_t get_ctrl_register32(Emulator *emu, int reg_index);

/* Source Instruction Operations */

uint8_t get_code8(Emulator *emu, int index);
int8_t get_sign_code8(Emulator *emu, int index);

uint16_t get_code16(Emulator *emu, int index);
int16_t get_sign_code16(Emulator *emu, int index);

uint32_t get_code32(Emulator *emu, int index);
int32_t get_sign_code32(Emulator *emu, int index);

/* Physical Memory Operations */

uint32_t get_physical_address(Emulator *emu, int seg_index, uint32_t offset, uint8_t write);

void _set_memory8(Emulator *emu, uint32_t p_address, uint8_t value);
void _set_memory16(Emulator *emu, uint32_t p_address, uint16_t value);
void _set_memory32(Emulator *emu, uint32_t p_address, uint32_t value);

uint32_t _get_memory32(Emulator *emu, uint32_t p_address);

/* Memory Operations with Segment Registers */

void set_memory8(Emulator *emu, int seg_index, uint32_t address, uint32_t value);
void set_memory16(Emulator *emu, int seg_index, uint32_t address, uint16_t value);
void set_memory32(Emulator *emu, int seg_index, uint32_t address, uint32_t value);

uint8_t get_memory8(Emulator *emu, int seg_index, uint32_t address);
uint16_t get_memory16(Emulator *emu, int seg_index, uint32_t address);
uint32_t get_memory32(Emulator *emu, int seg_index, uint32_t address);

/* Stack Operations */

void push16(Emulator *emu, uint16_t value);
uint16_t pop16(Emulator *emu);

void push32(Emulator *emu, uint32_t value);
uint32_t pop32(Emulator *emu);

/* Segment Register to Memory */

void push_segment_register(Emulator *emu, int reg_index);
void pop_segment_register(Emulator *emu, int reg_index);

/* Eflag Operations */

void set_carry_flag(Emulator *emu, int is_carry);
void set_zero_flag(Emulator *emu, int is_zero);
void set_sign_flag(Emulator *emu, int is_sign);
void set_int_flag(Emulator *emu, int is_enabled);
void set_direction_flag(Emulator *emu, int is_down);
void set_overflow_flag(Emulator *emu, int is_overflow);

int32_t is_carry(Emulator *emu);
int32_t is_zero(Emulator *emu);
int32_t is_sign(Emulator *emu);
int32_t is_int_enabled(Emulator *emu);
int32_t is_direction_down(Emulator *emu);
int32_t is_overflow(Emulator *emu);

void update_eflags_add(Emulator *emu, uint32_t value1, uint32_t value2, uint64_t result);
void update_eflags_add_8bit(Emulator *emu, uint8_t value1, uint8_t value2, uint16_t result);
void update_eflags_add_16bit(Emulator *emu, uint8_t value1, uint8_t value2, uint16_t result);

void update_eflags_sub(Emulator *emu, uint32_t value1, uint32_t value2, uint64_t result);
void update_eflags_sub_8bit(Emulator *emu, uint8_t value1, uint8_t value2, uint16_t result);
void update_eflags_sub_16bit(Emulator *emu, uint16_t value1, uint16_t value2, uint32_t result);

void update_eflags_logical_ops(Emulator *emu, uint32_t result);
void update_eflags_logical_ops_8bit(Emulator *emu, uint8_t result);
void update_eflags_logical_ops_16bit(Emulator *emu, uint8_t result);

void update_eflags_mul(Emulator *emu, uint64_t upper_half_result);

void set_tr(Emulator *emu, uint16_t val);

/* DTR Operations */
void set_gdtr(Emulator *emu, uint16_t limit, uint32_t base);
void set_idtr(Emulator *emu, uint16_t limit, uint32_t base);

#endif
