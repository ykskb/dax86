#include <stdint.h>

#include "interrupt.h"

uint8_t write_interrupt(Emulator *emu, uint8_t irq)
{
    if (emu->int_enabled)
    {
        printf("writing int: %d\n", irq);
        emu->int_pin = irq;
        printf("written emu->int_pin: %d\n", emu->int_pin);
        return 1;
    }
    else
    {
        printf("int busy\n");
        return 0;
    }
}