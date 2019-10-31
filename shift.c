#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "shift.h"

/*
 * 1100 1100 rol 2 => 0011 0011
 * (rm8_val << 2):       0011 0000
 * (rm8_val >> (8 - 2)): 0000 0011
 * 0011 0000 | 0000 0011 = Result: 0011 0011
 */
uint8_t rol8(uint8_t val, uint8_t n)
{
    return (val << n) | (val >> (8 - n));
}

uint8_t ror8(uint8_t val, uint8_t n)
{
    return (val >> n) | (val << (8 - n));
}

/*
 * 1100 1100 1 rcl 3 => 0110 0111 0 (1001 1001 1 -> 0011 0011 1 -> 0110 0111 0)
 * (rm8_val << 3):      0110 0100 0
 * (rm_val >> (9 - 3)): 0000 0011 0
 * 0110 0100 0 | 0000 0011 0 = Result: 0110 0111 CF: 0
 */
ShiftRes8 rcl8(uint8_t val, uint8_t n, uint8_t carry)
{
    uint16_t ext_val = (uint16_t)val;
    uint16_t ext_carry = (uint16_t)carry;

    uint16_t cf_added = (ext_val << 1) | ext_carry;
    uint16_t result = (cf_added << n) | (cf_added >> (9 - n));
    ShiftRes8 res;
    res.result = (uint8_t)(result >> 1);
    res.carry = result & 1;
    return res;
}

/*
 * 1 1100 1100 rcr 3 => 1 0011 1001 (0 1110 0110 -> 0 0111 0011 -> 1 0011 1001)
 * (rm8_val >> 3):      0 0011 1001
 * (rm_val << (9 - 3)): 1 0000 0000
 * 0 0011 1001 | 1 0000 0000 = CF: 1 Result: 0011 1001
 */
ShiftRes8 rcr8(uint8_t val, uint8_t n, uint8_t carry)
{
    uint16_t ext_val = (uint16_t)val;
    uint16_t ext_carry = (uint16_t)carry;

    uint16_t cf_added = ext_val | (ext_carry << 8);
    uint16_t result = (cf_added >> n) | (cf_added << (9 - n));
    ShiftRes8 res;
    res.result = (uint8_t)result;
    res.carry = (result & 0x100) != 0;
    return res;
}

/*
 * shl rm8 imm8: 2|3 bytes
 * Shifts left, setting the carry flag accordingly.
 * 1 byte: op code()
 * 1|2 bytes: rm8
 * 1 byte: imm8
 * 1 1100 1100 << 3
 * 1 1001 1000
 * 1 0011 0000
 * 0 0110 0000
 */
ShiftRes8 shl8(uint8_t val, uint8_t n)
{
    uint16_t ext_val = (uint16_t)val;

    uint16_t result = ext_val << n;
    ShiftRes8 res;
    res.result = (uint8_t)result;
    res.carry = (result & 0x100) != 0;
    return res;
}

ShiftRes8 shr8(uint8_t val, uint8_t n, uint8_t carry)
{
    uint16_t ext_val = (uint16_t)val;
    uint16_t ext_carry = (uint16_t)carry;

    uint16_t cf_added = (ext_val << 1) | ext_carry;
    uint16_t result = cf_added >> n;
    ShiftRes8 res;
    res.result = (uint8_t)(result >> 1);
    res.carry = (result & 1) != 0;
    return res;
}

ShiftRes8 sar8(uint8_t val, uint8_t n, uint8_t carry)
{
    int16_t ext_val = (int8_t)val;
    int16_t ext_carry = (int16_t)carry;

    int16_t cf_added = (ext_val << 1) | ext_carry;
    int16_t result = cf_added >> n;
    ShiftRes8 res;
    res.result = (uint8_t)(result >> 1);
    res.carry = (result & 1) != 0;
    return res;
}

uint32_t rol32(uint32_t val, uint8_t n)
{
    return (val << n) | (val >> (32 - n));
}

uint32_t ror32(uint32_t val, uint8_t n)
{
    return (val >> n) | (val << (32 - n));
}

ShiftRes32 rcl32(uint32_t val, uint8_t n, uint8_t carry)
{
    uint64_t ext_val = (uint64_t)val;
    uint64_t ext_carry = (uint64_t)carry;

    uint64_t cf_added = (ext_val << 1) | ext_carry;
    uint64_t result = (cf_added << n) | (cf_added >> (33 - n));
    ShiftRes32 res;
    res.result = (uint32_t)(result >> 1);
    res.carry = (result & 1) != 0;
    return res;
}

ShiftRes32 rcr32(uint32_t val, uint8_t n, uint8_t carry)
{
    uint64_t ext_val = (uint64_t)val;
    uint64_t ext_carry = (uint64_t)carry;

    uint64_t cf_added = ext_val | (ext_carry << 32);
    uint64_t result = (cf_added >> n) | (cf_added << (33 - n));
    ShiftRes32 res;
    res.result = (uint32_t)(result);
    res.carry = (result & 0x100000000) != 0;
    return res;
}

ShiftRes32 shl32(uint32_t val, uint8_t n)
{
    uint64_t ext_val = (uint64_t)val;

    uint64_t result = ext_val << n;
    ShiftRes32 res;
    res.result = (uint32_t)(result);
    res.carry = (result & 0x100000000) != 0;
    return res;
}

ShiftRes32 shr32(uint32_t val, uint8_t n, uint8_t carry)
{
    uint64_t ext_val = (uint64_t)val;
    uint64_t ext_carry = (uint64_t)carry;

    uint64_t cf_added = (ext_val << 1) | ext_carry;
    uint64_t result = cf_added >> n;

    ShiftRes32 res;
    res.result = (uint32_t)(result >> 1);
    res.carry = (result & 1) != 0;
    return res;
}

ShiftRes32 sar32(uint32_t val, uint8_t n, uint8_t carry)
{
    int64_t ext_val = (int32_t)val;
    int64_t ext_carry = (int64_t)carry;

    int64_t cf_added = (ext_val << 1) | ext_carry;
    int64_t result = cf_added >> n;

    ShiftRes32 res;
    res.result = (uint32_t)(result >> 1);
    res.carry = (result & 1) != 0;
    return res;
}