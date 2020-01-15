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
 * GDTR: 
 * 48 bits long register to hold size and the starting address of GDT.
 * | 31                      16 | 15                       0 | 
 * |          Base 0:15         |        Limit 0:15          |
 * |____________________________|____________________________|
 * | 63      56 | 55 52 | 51 48 | 47        40 | 39       32 |
 * | Base 24:31 | Flags | Limit | Access Byte  | Base 16:23  |
 * |____________|_______|_16:19_|______________|_____________|
 * 
 * Access Byte:
 * Bit   | 7  |    5 & 6   | 4 |  3   | 2  | 1  | 0  |
 * Use   | Pr |  Privilege | 1 | Exec | DC | RW | AC |
 * Pr: present or not
 * Privilege: 0: kernel 3: user programs
 * Exec: 1: executable | 0: system descriptor
 * DC (CS): 1: lower level can execute | 0: only executable with privilege
 * DC (DS): 1: segments grow downward | 0: segments grow upward
 * RW (CS): 1: readable | 0: not readable 
 * RW (DS): 1: writable | 0: not writable
 * AC: 1: accessed | 0: not accessed
 * 
 * Bit 1 & 2 & 3:
 * 000: Data, read-only
 * 001: Data, read-write
 * 010: Stack, read-only
 * 011: Stack, read-write
 * 100: Code, execute-only
 * 101: Code, execute-read
 * 110: Code, execute-only, conforming
 * 111: Code, execute-read, conforming
 * 
 * Flags:
 * Bit | 7  |   6   | 5 | 4 |
 * Use | Gr | D(Sz) | 0 | 0 |
 * Gr: descriptor's segment size (limit): 0: in bytes (1,2...2^20 bytes) | 1: in 4KB blocks (1x2^12, 2x2^12...2^20x2^12)
 * D (Sz): Default operant size: 0: 16-bit protected mode | 1: 32-bit protected mode *for 286 compatibility
 * 
 * 
 * Descriptors Required
 * - Null Descriptor
 *   8 empty bytes
 * - Code Descriptor
 *   Base: 0x0 Limit: 0xFFFF
 *   Access: 0x9A (1001 1010b) Pr: 1 Privl: 0 Exec: 1 DC: 0 Read: 1
 *   Flags: 0xCF (1100 1111b) Gr: 1 Sz: 1
 * - Data Descriptor
 *   Base: 0x0 Limit: 0xFFFF
 *   Access: 0x92 (1001 0010b) Pr: 1 Privl: 0 Exec: 0 EC: 0 Write: 1
 *   Flags: 0xCF (1100 1111b) Gr: 1 Sz: 1
 */

/*
 * Segment Registers
 * | 15           3 |  2  | 1 0 |
 * |     Index      | TI  | RPL |
 * |________________|_____|_____|
 * Index: 13 bits: 2^13 = 8192 seg descriptors
 * TI: 0: GDT | 1: LDT
 * RPL: Required Privilege (Ring)
 */

/*
 * lgdt: 5 bytes
 * Loads GDTR with size and staring address of GDT.
 * 1 byte: op ()
 * 4 byte: Pointer to GDT entry.
 */
void lgdt(Emulator *emu, ModRM *modrm)
{
    uint32_t address = calc_memory_address(emu, modrm);
    uint16_t limit = get_memory16(emu, DS, address);
    uint32_t base = get_memory32(emu, DS, address + 2);
    set_gdtr(emu, limit, base);
}