
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "paging.h"
#include "emulator_functions.h"

void check_paging(Emulator *emu)
{
    if (emu->is_pg)
        return;
    if ((emu->control_registers[CR0] & CR0_PG) != 0)
        emu->is_pg = 1;
}

/*
 * Page Directory Entry
 * 31_____________________________12_11_____9____________________________________0
 * | Page table 4kb-aligned address | Avail. | G | S | 0 | A | D | W | U | R | P |
 * |_____________________________________________________________________________|
 * P: Present
 * R: Read/write
 * U: User/supervisor
 * W: Write-through / write-back
 * D: cache-Disabled
 * A: Accessed
 * S: page Size
 * 
 * Page Table Entry
 * 31_____________________________12_11_____9_______________________________________0
 * | Page table 4kb-aligned address | Avail. | G* | 0 | D* | A | C* | W | U | R | P |
 * |________________________________________________________________________________|
 * C: Same as Cache-disabled above
 * G: Global. 1: prevents TLB cache update on CR3 reset. (CR4 global enable needs to be on.)
 * D: Dirty flag (written)
 * 0: memory type when PAT is enabled.
 * 
 * Address in 3-level Scheme
 * 
 * 
 * Add
 */
uint32_t get_phys_addr(Emulator *emu, uint32_t linear_addr, uint8_t write, uint8_t exec)
{
    uint32_t pde_index = linear_addr >> 22;
    uint32_t pde = _get_memory32(emu, emu->control_registers[CR3] + (pde_index * 4));
    uint8_t cr4_pse = emu->control_registers[CR4] & CR4_PSE;
    uint8_t pde_ps = pde & PDE_PS;
    /* 
     * Page size extension enabled.
     * [10 bits]: Page directory entry index
     * [22 bits]: Offset from page directory entry base
     */
    if ((cr4_pse != 0) && (pde_ps != 0))
    {
        uint32_t phys_base = pde >> 12;
        uint32_t phys_offset = linear_addr & 0x3FFFFF;
        return phys_base + phys_offset;
    }
    /* 
     * Page directory + page table
     * [10 bits]: Page directory entry index
     * [10 bits]: Page table entry index
     * [12 bits]: Offset from page table entry base
     */
    else
    {
        uint16_t pt_base = pde >> 12;
        uint16_t pte_index = (linear_addr >> 12) & 0x3FF;
        uint32_t pte = _get_memory32(emu, pt_base + (pte_index * 4));
        uint32_t phys_base = pte >> 12;
        uint32_t phys_offset = linear_addr & 0xFFF;
        return phys_base + phys_offset;
    }
}