
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

static void kbd_write_interrupt(Emulator *emu)
{
    if (kbd->buf_index_reset)
    {
        while (kbd->buf_out_index < KBD_BUF_SIZE)
        {
            if (write_interrupt(emu, T_IRQ0 + IRQ_KBD))
            {
                kbd->buf_out_index += 1;
            }
        }
        kbd->buf_index_reset = 0;
        kbd->buf_out_index = 0;
    }
    for (kbd->buf_out_index; kbd->buf_out_index < kbd->buf_index; kbd->buf_out_index++)
    {
        if (write_interrupt(emu, T_IRQ0 + IRQ_KBD))
        {
            printf("wrote interrupt");
            kbd->buf_out_index += 1;
        }
    }
    kbd->status &= ~KBD_STATUS_IN;
}

static void *kbd_loop(void *ptr)
{
    Emulator *emu = (Emulator *)ptr;
    while (1)
    {
        if (kbd->buf_index > KBD_BUF_SIZE)
        {
            kbd->buf_index = 0;
            kbd->buf_index_reset = 1;
        }
        uint8_t value = getchar();
        kbd->buf[kbd->buf_index] = value;
        kbd->status |= KBD_STATUS_IN;
        kbd->buf_index += 1;
        kbd_write_interrupt(emu);
    }
}

void init_kbd(Emulator *emu)
{
    kbd = malloc(sizeof(KBD));
    kbd->buf_index = 0;
    kbd->buf_out_index = 0;
    kbd->buf_index_reset = 0;
    kbd->status = 0;
    pthread_t kbd_thread_id;
    pthread_create(&kbd_thread_id, NULL, kbd_loop, (void *)emu);
}