#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "gdt.h"
#include "emulator_functions.h"
#include "emulator.h"
#include "util.h"

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
 * CPL: Current Privilege (Ring)
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
    set_gdtr(emu, limit, get_physical_address(emu, DS, base, 0));
}

void check_protected_mode_entry(Emulator *emu)
{
    if (emu->is_pe)
        return;
    uint8_t cr0_pe = (emu->control_registers[CR0] & CR0_PE);
    uint16_t gdt_index = get_seg_register16(emu, CS) >> 3;
    uint16_t gdt_entry_count = (emu->gdtr.limit + 1) / 8;
    if (cr0_pe && gdt_index != 0 && gdt_entry_count > gdt_index)
        emu->is_pe = 1;
}

static void gdt_access_error(char *msg, Emulator *emu)
{
    printf("GDT access error: %s\n", msg);
    panic_exit(emu);
}

static void check_entry_access(Emulator *emu, uint8_t access_byte, uint8_t cpl, uint8_t write, uint8_t exec)
{
    if (write)
    {
        uint8_t rw = access_byte & 2;
        if (!rw)
        {
            gdt_access_error("not writable", emu);
        }
    }
    if (exec)
    {
        uint8_t d_exec = access_byte & 8;
        if (!d_exec)
        {
            gdt_access_error("not executable", emu);
        }
    }
    uint8_t dpl = (access_byte >> 5) & 3;
    if (dpl < cpl)
    {
        gdt_access_error("entry privilege not met", emu);
    }
}

uint32_t read_gdt_entry_base(uint32_t entry1, uint32_t entry2)
{
    uint32_t base_low = (entry1 >> 16);
    uint32_t base_mid = (entry2 & 0xFF);
    uint32_t base_high = (entry2 >> 24);
    return base_low | (base_mid << 16) | (base_high << 24);
}

static void check_entry_limit(Emulator *emu, uint32_t entry1, uint32_t entry2, uint32_t offset)
{
    /* If paging is on, limit is most likely set to max. */
    if (emu->is_pg)
    {
        return;
    }
    uint32_t limit_low = entry1 & 0xFFFF;
    uint32_t limit_high = (entry2 >> 16) & 0xF;
    uint32_t limit = limit_low | (limit_high << 16);

    uint8_t flags = (entry2 >> 20) & 0xF;
    uint8_t g = flags & 8;
    if (g)
    {
        uint64_t limit_val = limit * (2 ^ 12);
        if ((uint64_t)offset > limit_val)
        {
            gdt_access_error("beyond entry limit", emu);
        }
    }
    else
    {
        if (offset > limit)
        {
            gdt_access_error("beyond entry limit", emu);
        }
    }
}

uint32_t get_linear_addr(Emulator *emu, uint16_t seg_val, uint32_t offset, uint8_t write, uint8_t exec)
{
    int entry_index = seg_val >> 3;
    uint8_t ti = seg_val & (1 << 2);
    uint8_t cpl = seg_val & 3;
    if (ti)
    {
        /* LGT */
        return 0;
    }
    else
    {
        /* GDT */
        uint32_t entry_addr = emu->gdtr.base + (entry_index * 8);
        if (entry_addr > (emu->gdtr.base + emu->gdtr.limit + 1))
        {
            gdt_access_error("beyond table limit", emu);
        }
        uint32_t entry1 = _get_memory32(emu, entry_addr);
        uint32_t entry2 = _get_memory32(emu, entry_addr + 4);
        uint8_t access_byte = (entry2 >> 8) & 0xFF;
        check_entry_access(emu, access_byte, cpl, write, exec);

        uint32_t base = read_gdt_entry_base(entry1, entry2);
        check_entry_limit(emu, entry1, entry2, offset);

        return base + offset;
    }
}
