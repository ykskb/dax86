#include <stdint.h>
#include <stdlib.h>

#include "interrupt.h"
#include "emulator_functions.h"
#include "util.h"
#include "gdt.h"

/*
 *
 * IDT:
 * 48 bits long register to hold size and the starting address of GDT.
 * | 31                      16 | 15                       0 |
 * |     Seg Selector 0:15      |       Offset 0:15          |
 * |____________________________|____________________________|
 * | 63                      48 |47|46 45|44 40|39   35|34 32|
 * |        Offset 16:31        |Pr| DPL |Type |   0   | IST |
 * |____________________________|______________|_____________|
 */
void lidt(Emulator *emu, ModRM *modrm)
{
    uint32_t address = calc_memory_address(emu, modrm);
    uint16_t limit = get_memory16(emu, DS, address);
    uint32_t base = get_memory32(emu, DS, address + 2);
    set_idtr(emu, limit, get_physical_address(emu, DS, base, 0));
}

/* 
 * Real mode and VM (Virtual 8086) is not supported.
 */
void handle_interrupt(Emulator *emu, uint8_t vector, int sw)
{
    if (!emu->is_pe)
    {
        printf("Interrupt in real mode not implemented.\n");
        panic_exit(emu);
    }
    uint32_t entry_addr = emu->idtr.base + (vector * 8);
    if (entry_addr > (emu->idtr.base + emu->idtr.limit))
    {
        printf("IDT entry is beyond table limit.\n");
        panic_exit(emu);
    }
    uint32_t entry1 = _get_memory32(emu, entry_addr);
    uint32_t entry2 = _get_memory32(emu, entry_addr + 4);

    uint8_t cpl = get_seg_register16(emu, CS) & 3;
    uint8_t dpl = (entry2 >> 13) & 0x3;

    if (sw && (dpl < cpl))
    {
        printf("Privilege not met in software interrupt.\n");
        panic_exit(emu);
    }

    uint16_t gate_selector = entry1 >> 16;
    uint8_t gate_dpl = gate_selector & 3;
    uint32_t gate_offset = 0 | (entry1 & 0xFFFF) | (entry2 & 0xFFFF0000);
    /* Inter-privilege */
    if (gate_dpl < cpl)
    {
        printf("inter-privilege interrupt: %d\n", vector);
        uint16_t cur_ss = get_seg_register16(emu, SS);
        uint32_t cur_esp = get_register32(emu, ESP);
        uint16_t cur_cs = get_seg_register16(emu, CS);
        uint32_t cur_eip = emu->eip;

        /* Get TSS */
        uint8_t tss_desc_index = emu->tr >> 3;
        uint32_t tss_desc_base = emu->gdtr.base + (tss_desc_index * 8);
        uint32_t entry1 = _get_memory32(emu, tss_desc_base);
        uint32_t entry2 = _get_memory32(emu, tss_desc_base + 4);
        uint32_t tss_base = read_gdt_entry_base(entry1, entry2);
        uint16_t tss_ss = (uint16_t)get_memory32(emu, DS, tss_base + 8);
        uint32_t tss_esp = get_memory32(emu, DS, tss_base + 4);

        set_seg_register16(emu, SS, tss_ss);
        set_register32(emu, ESP, tss_esp);

        set_seg_register16(emu, CS, gate_selector);
        emu->eip = gate_offset;

        push32(emu, cur_ss);
        push32(emu, cur_esp);
        push32(emu, emu->eflags);
        push32(emu, cur_cs);
        push32(emu, cur_eip);
        /* error code */
        push32(emu, 0);
    }
    /* Kernel mode */
    else
    {
        printf("intra-level interrupt: %d\n", vector);
        uint16_t cur_cs = get_seg_register16(emu, CS);
        uint32_t cur_eip = emu->eip;
        push32(emu, emu->eflags);
        push32(emu, cur_cs);
        push32(emu, cur_eip);
        /* error code */
        push32(emu, 0);
        set_seg_register16(emu, CS, gate_selector);
        emu->eip = gate_offset;
    }
}