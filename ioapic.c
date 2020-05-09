#include <stdlib.h>
#include <string.h>

#include "ioapic.h"
#include "lapic.h"

IOAPIC *ioapic;

void init_ioapic()
{
    ioapic = malloc(sizeof(IOAPIC));
    ioapic->select_register = 0x0;
    ioapic->window_register = 0x0;
    ioapic->id_register = 0x0;
    ioapic->ver_register = 0x170011;
    ioapic->arb_register = 0x0;
    memset(ioapic->redirect_tbl, 0, sizeof(ioapic->redirect_tbl));
}

void add_lapic(uint8_t index, LAPIC *lapic)
{
    ioapic->lapic[index] = lapic;
}

static uint8_t get_dest_id(uint8_t irq)
{
    uint8_t i;
    for (i = 0; i < 48; i += 2)
    {
        if (irq == ioapic->redirect_tbl[i])
        {
            return ioapic->redirect_tbl[i + 1];
        }
    }
    return 0;
}

void ioapic_int_to_lapic(uint8_t irq)
{
    uint8_t dest_id = get_dest_id(irq);
    lapic_write_to_irr(ioapic->lapic[dest_id], irq);
}

void ioapic_write_reg(uint32_t addr, uint32_t val)
{
    uint8_t offset = addr - IOAPIC_DEFAULT_BASE;
    /* Writing on selector register. */
    if (offset == 0x0)
    {
        ioapic->select_register = val;
        /* Moves value into window register when selector register is set. */
        if (val == 0x0)
        {
            ioapic->window_register = ioapic->id_register;
        }
        else if (val == 0x1)
        {
            ioapic->window_register = ioapic->ver_register;
        }
        else if (val == 0x2)
        {
            ioapic->window_register = ioapic->arb_register;
        }
        return;
    }
    /* Writing on window register. */
    if (offset == 0x10)
    {
        if (ioapic->select_register >= 0x10)
        {
            uint8_t rdr_tbl_index = ioapic->select_register - 0x10;
            ioapic->redirect_tbl[rdr_tbl_index] = val;
        }
        return;
    }
}

uint32_t ioapic_read_reg(uint32_t addr)
{
    uint8_t offset = addr - IOAPIC_DEFAULT_BASE;
    if (offset == 0x10)
    {
        return ioapic->window_register;
    }
    return 0;
}

void dump_ioapic()
{
    printf("<IO APIC at %p>\n", (void *)ioapic);
    printf("IOREGSEL: %08x IOWIN: %08x IOAPICID: %08x\n", ioapic->select_register, ioapic->window_register, ioapic->id_register);
    printf("Redirection Tables: ");
    uint8_t i;
    for (i = 0; i < 48; i += 2)
    {
        if (ioapic->redirect_tbl[i] > 0)
        {
            printf("[IRQ: %08x DESTID: %08x] ", ioapic->redirect_tbl[i], ioapic->redirect_tbl[i + 1]);
        }
    }
}