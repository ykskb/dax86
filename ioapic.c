#include <stdlib.h>

#include "ioapic.h"
#include "lapic.h"

IOAPIC *create_ioapic()
{
    IOAPIC *ioapic = malloc(sizeof(IOAPIC));
    return ioapic;
}

void add_lapic(IOAPIC *ioapic, uint8_t index, LAPIC *lapic)
{
    ioapic->lapic[index] = lapic;
}

static uint8_t get_dest_id(IOAPIC *ioapic, uint8_t irq)
{
    return 0;
}

void ioapic_int_to_lapic(IOAPIC *ioapic, uint8_t irq)
{
    uint8_t dest_id = get_dest_id(ioapic, irq);
    lapic_write_to_irr(ioapic->lapic[dest_id], irq);
}