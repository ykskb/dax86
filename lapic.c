#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lapic.h"

LAPIC *create_lapic(Emulator *emu)
{
    LAPIC *lapic = malloc(sizeof(LAPIC));
    memset(lapic->registers, 0, sizeof(lapic->registers));
    lapic->emu = emu;
    return lapic;
}

void lapic_send_intr(LAPIC *lapic)
{
    if (lapic->emu->int_enabled && lapic->eoi == 0)
    {
        pthread_mutex_lock(&lapic->lock);
        uint8_t i;
        for (i = 0; i < 255; i++)
        {
            if (lapic->irr[i] > 0)
            {
                lapic->isr[i] = lapic->irr[i];
                lapic->isr_index = i;
                lapic->emu->int_r = lapic->irr[i];
                lapic->eoi = 1;
                lapic->irr[i] = 0;
                break;
            }
        }
        pthread_mutex_unlock(&lapic->lock);
    }
}

void lapic_eoi(LAPIC *lapic)
{
    pthread_mutex_lock(&lapic->lock);
    lapic->eoi = 0;
    lapic->isr[lapic->isr_index] = 0;
    pthread_mutex_unlock(&lapic->lock);
    lapic_send_intr(lapic);
}

void lapic_write_to_irr(LAPIC *lapic, uint8_t irq)
{
    pthread_mutex_lock(&lapic->lock);
    uint8_t i;
    for (i = 0; i < 255; i++)
    {
        if (lapic->irr[i] == 0)
        {
            lapic->irr[i] = irq;
            pthread_mutex_unlock(&lapic->lock);
            lapic_send_intr(lapic);
            return;
        }
    }
    pthread_mutex_unlock(&lapic->lock);
}