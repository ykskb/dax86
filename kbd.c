
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>

#include "kbd.h"
#include "emulator.h"
#include "emulator_functions.h"
#include "interrupt.h"

KBD *kbd;

uint8_t get_kbd_status()
{
    return kbd->status;
}

void write_ps2_output_port(uint8_t value)
{
    switch (value)
    {
    /* A20 gate */
    case 0xd1:
        break;
    default:
        break;
    }
}

void write_ps2_config_byte(uint8_t value)
{
    switch (value)
    {
    /* A20 enable */
    case 0xdf:
        break;
    default:
        break;
    }
}

static void *kbd_loop()
{
    while (1)
    {
        uint8_t value = getchar();
        kbd->buf[kbd->buf_index] = value;
        if (kbd->buf_index > 254)
        {
            kbd->buf_index = 0;
        }
        else
        {
            kbd->buf_index += 1;
        }
        ioapic_int_to_lapic(T_IRQ0 + IRQ_KBD);
    }
    return NULL;
}

void init_kbd(IOAPIC *ioapic)
{
    kbd = malloc(sizeof(KBD));
    kbd->status = 0;
    kbd->ioapic = ioapic;
    pthread_t kbd_thread_id;
    pthread_create(&kbd_thread_id, NULL, kbd_loop, NULL);
}