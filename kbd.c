
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

#define KBD_DIB 0x01

KBD *kbd;

uint8_t get_kbd_status()
{
    if (kbd->buf_index != kbd->buf_out_index)
        kbd->status |= KBD_DIB;
    else
        kbd->status &= 0xFE;
    return kbd->status;
}

uint8_t get_kbd_data()
{
    uint8_t data = kbd->buf[kbd->buf_out_index];
    if (kbd->buf_out_index > 254)
    {
        kbd->buf_out_index = 0;
    }
    else
    {
        kbd->buf_out_index += 1;
    }
    return data;
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

static int append_to_buf(uint8_t c)
{
    if (c < 1 || c > 127)
        return 0;
    uint8_t sc = scmap[c];
    if (!sc)
        return 0;
    kbd->buf[kbd->buf_index] = sc;
    if (kbd->buf_index > 254)
    {
        kbd->buf_index = 0;
    }
    else
    {
        kbd->buf_index += 1;
    }
    return 1;
}

static void *kbd_loop()
{
    while (1)
    {
        uint8_t c = getchar();
        if (append_to_buf(c))
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