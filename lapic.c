#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "lapic.h"
#include "interrupt.h"

LAPIC *create_lapic(Emulator *emu)
{
    LAPIC *lapic = malloc(sizeof(LAPIC));
    memset(lapic->registers, 0, sizeof(lapic->registers));
    lapic->emu = emu;
    pthread_t timer_thread;
    lapic->timer_thread = &timer_thread;
    return lapic;
}

void lapic_send_intr(LAPIC *lapic)
{
    if (!lapic->int_enabled || !lapic->unit_enabled || !lapic->emu->int_enabled || lapic->registers[EOI] != 0)
    {
        return;
    }
    pthread_mutex_lock(&lapic->lock);
    uint8_t i;
    for (i = 0; i < 255; i++)
    {
        if (lapic->irr[i] > 0)
        {
            lapic->isr[i] = lapic->irr[i];
            lapic->isr_index = i;
            lapic->emu->int_r = lapic->irr[i];
            lapic->registers[EOI] = 1;
            lapic->irr[i] = 0;
            break;
        }
    }
    pthread_mutex_unlock(&lapic->lock);
}

static void lapic_eoi(LAPIC *lapic)
{
    pthread_mutex_lock(&lapic->lock);
    lapic->isr[lapic->isr_index] = 0;
    pthread_mutex_unlock(&lapic->lock);
    lapic_send_intr(lapic);
}

void lapic_write_to_irr(LAPIC *lapic, uint8_t irq)
{
    if (!lapic->unit_enabled || !lapic->int_enabled)
    {
        return;
    }
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

/*
 * Timer interrupt
 * Decrement Rate: bus frequency / DCR (divide configuration register)
 * ICR (Initial Count Register) - 1 at decrement rate above
 * Interrupt is generated at ICR:0 and counter is reset to ICR.
 * For xv6 it's aimed to be 100 times/sec, so the loop below just waits 10ms.
 */
static void *timer_loop(void *ptr)
{
    LAPIC *lapic = (LAPIC *)ptr;
    while (1)
    {
        lapic_write_to_irr(lapic, T_IRQ0 + IRQ_TIMER);
        usleep(10 * 1000);
    }
    return NULL;
}

static void check_int_enabled(LAPIC *lapic)
{
    if (lapic->registers[LINT0 >> 4] == MASKED && lapic->registers[LINT1 >> 4] == MASKED)
    {
        lapic->int_enabled = 1;
    }
    else
    {
        lapic->int_enabled = 0;
    }
}

void lapic_write_reg(LAPIC *lapic, uint32_t addr, uint32_t val)
{
    uint32_t offset = (addr - LAPIC_DEFAULT_BASE);
    uint8_t index = offset >> 4;
    lapic->registers[index] = val;
    if (offset == SVR)
    {
        if (val & 0x100)
        {
            lapic->unit_enabled = 1;
        }
        return;
    }
    else if (offset == TICR)
    {
        pthread_create(lapic->timer_thread, NULL, timer_loop, (void *)lapic);
    }
    else if (offset == TPR)
    {
        check_int_enabled(lapic);
    }
    else if (offset == EOI && val == 0)
    {
        check_int_enabled(lapic);
        lapic_eoi(lapic);
    }
}

uint32_t lapic_read_reg(LAPIC *lapic, uint32_t addr)
{
    uint8_t index = (addr - LAPIC_DEFAULT_BASE) >> 4;
    return lapic->registers[index];
}

void dump_lapic(LAPIC *lapic)
{
    printf("<LAPIC at %p>\n", (void *)lapic);
    printf("TPR: %08x EOI: %08x SVR: %08x\n", lapic->registers[TPR >> 4], lapic->registers[EOI >> 4], lapic->registers[SVR >> 4]);
    printf("TIMER: %08x TICR: %08x\n", lapic->registers[TIMER >> 4], lapic->registers[TICR >> 4]);
    printf("LINT0: %08x LINT1: %08x\n", lapic->registers[LINT0 >> 4], lapic->registers[LINT1 >> 4]);
}