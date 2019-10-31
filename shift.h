#ifndef SHIFT_H_
#define SHIFT_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "instruction_defs.h"
#include "emulator_functions.h"
#include "modrm.h"
#include "io.h"

typedef struct
{
    uint8_t result;
    uint8_t carry;
} ShiftRes8;

typedef struct
{
    uint32_t result;
    uint8_t carry;
} ShiftRes32;

uint8_t rol8(uint8_t val, uint8_t n);
uint8_t ror8(uint8_t val, uint8_t n);
ShiftRes8 rcl8(uint8_t val, uint8_t n, uint8_t carry);
ShiftRes8 rcr8(uint8_t val, uint8_t n, uint8_t carry);
ShiftRes8 shl8(uint8_t val, uint8_t n);
ShiftRes8 shr8(uint8_t val, uint8_t n, uint8_t carry);
ShiftRes8 sar8(uint8_t val, uint8_t n, uint8_t carry);

uint32_t rol32(uint32_t val, uint8_t n);
uint32_t ror32(uint32_t val, uint8_t n);
ShiftRes32 rcl32(uint32_t val, uint8_t n, uint8_t carry);
ShiftRes32 rcr32(uint32_t val, uint8_t n, uint8_t carry);
ShiftRes32 shl32(uint32_t val, uint8_t n);
ShiftRes32 shr32(uint32_t val, uint8_t n, uint8_t carry);
ShiftRes32 sar32(uint32_t val, uint8_t n, uint8_t carry);

#endif