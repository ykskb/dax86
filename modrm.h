#ifndef MODRM_H_
#define MODRM_H_

#include <stdint.h>

#include "emulator.h"

typedef struct
{
    uint8_t scale; // 2 bits
    uint8_t index; // 3 bits
    uint8_t base;  // 3 bits
} SIB;

typedef struct
{
    /* Mod: 2 bits */
    uint8_t mod;
    /* REG: 3 bits */
    union {                // union: different data at the same location
        uint8_t opcode;    // used to extend opcode (e.g. inc: ff + 000 and dec: ff + 001)
        uint8_t reg_index; // used to specify register (target or source)
    };
    uint8_t rm; // 3 bits
    SIB sib;    // 1 byte
    union {
        int8_t disp8;
        int16_t disp16;
        uint32_t disp32;
    };
} ModRM;

ModRM create_modrm();

/*
 * Parses ModR/M, SIB and Displacement.
 * EIP of Emulator needs to be pointing ModR/M Byte.
 */
void parse_modrm(Emulator *emu, ModRM *modrm);

/* Calcurates the memory address. Exposed for LEA instruction. */
uint32_t calc_memory_address(Emulator *emu, ModRM *modrm);

/* Sets 8-bit value to register or memory depending on Mod of ModR/M byte. */
void set_rm8(Emulator *emu, ModRM *modrm, uint8_t value);

/* Gets 8-bit value from register or memory depending on Mod of ModR/M byte. */
uint8_t get_rm8(Emulator *emu, ModRM *modrm);

void set_rm16(Emulator *emu, ModRM *modrm, uint8_t value);
uint16_t get_rm16(Emulator *emu, ModRM *modrm);

void set_rm32(Emulator *emu, ModRM *modrm, uint32_t value);
uint32_t get_rm32(Emulator *emu, ModRM *modrm);

/* Sets 8-bit value on the register specified by REG of ModR/M. */
void set_r8(Emulator *emu, ModRM *modrm, uint8_t value);

/* Get 8-bit value from the register specified by REG of ModR/M. */
uint8_t get_r8(Emulator *emu, ModRM *modrm);

void set_r16(Emulator *emu, ModRM *modrm, uint8_t value);
uint8_t get_r16(Emulator *emu, ModRM *modrm);

void set_r32(Emulator *emu, ModRM *modrm, uint32_t value);
uint32_t get_r32(Emulator *emu, ModRM *modrm);

/* Sets 16-bit value on the segment register specified by REG of ModR/M. */
void set_seg_r(Emulator *emu, ModRM *modrm, uint16_t value);

/* Get 16-bit value from the segment register specified by REG of ModR/M. */
uint16_t get_seg_r(Emulator *emu, ModRM *modrm);

#endif