
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

uint8_t scmap[128][3] = {
    /* 0x00 */
    {NO, NO, NO},
    {NO, NO, NO},
    {NO, NO, NO},
    {NO, NO, NO},
    {NO, NO, NO},
    {NO, NO, NO},
    {NO, NO, NO},
    {NO, NO, NO},
    {0x0e, NO, NO}, // backspace
    {0x0f, NO, NO}, // tab
    {0x1c, NO, NO}, // enter (LF)
    {NO, NO, NO},
    {NO, NO, NO},
    {NO, NO, NO},
    {NO, NO, NO},
    {NO, NO, NO},
    /* 0x10 */
    {NO, NO, NO},
    {NO, NO, NO},
    {NO, NO, NO},
    {NO, NO, NO},
    {NO, NO, NO},
    {NO, NO, NO},
    {NO, NO, NO},
    {NO, NO, NO},
    {NO, NO, NO},
    {NO, NO, NO},
    {NO, NO, NO},
    {0x01, NO, NO}, // esc
    {NO, NO, NO},
    {NO, NO, NO},
    {NO, NO, NO},
    {NO, NO, NO},
    /* 0x20 */
    {0x39, NO, NO},     // space
    {0x2a, 0x02, 0xaa}, // !
    {0x2a, 0x28, 0xaa}, // "
    {0x2a, 0x04, 0xaa}, // #
    {0x2a, 0x05, 0xaa}, // $
    {0x2a, 0x06, 0xaa}, // %
    {0x2a, 0x08, 0xaa}, // &
    {0x28, NO, NO},     // '
    {0x2a, 0x0a, 0xaa}, // (
    {0x2a, 0x0b, 0xaa}, // )
    {0x2a, 0x09, 0xaa}, // *
    {0x2a, 0x0d, 0xaa}, // +
    {0x33, NO, NO},     // ,
    {0x0c, NO, NO},     // -
    {0x34, NO, NO},     // .
    {0x35, NO, NO},     // /

    /* 0x30 */
    {0x0b, NO, NO},     // 0
    {0x02, NO, NO},     // 1
    {0x03, NO, NO},     // 2
    {0x04, NO, NO},     // 3
    {0x05, NO, NO},     // 4
    {0x06, NO, NO},     // 5
    {0x07, NO, NO},     // 6
    {0x08, NO, NO},     // 7
    {0x09, NO, NO},     // 8
    {0x0a, NO, NO},     // 9
    {0x2a, 0x27, 0xaa}, // :
    {0x27, NO, NO},     // ;
    {0x2a, 0x33, 0xaa}, // <
    {0x0d, NO, NO},     // =
    {0x2a, 0x34, 0xaa}, // >
    {0x2a, 0x35, 0xaa}, // ?

    /* 0x40 */
    {0x2a, 0x03, 0xaa}, // @
    {0x2a, 0x1e, 0xaa}, // A
    {0x2a, 0x30, 0xaa}, // B
    {0x2a, 0x2e, 0xaa}, // C
    {0x2a, 0x20, 0xaa}, // D
    {0x2a, 0x12, 0xaa}, // E
    {0x2a, 0x21, 0xaa}, // F
    {0x2a, 0x22, 0xaa}, // G
    {0x2a, 0x23, 0xaa}, // H
    {0x2a, 0x17, 0xaa}, // I
    {0x2a, 0x24, 0xaa}, // J
    {0x2a, 0x25, 0xaa}, // K
    {0x2a, 0x26, 0xaa}, // L
    {0x2a, 0x32, 0xaa}, // M
    {0x2a, 0x31, 0xaa}, // N
    {0x2a, 0x18, 0xaa}, // O

    /* 0x50 */
    {0x2a, 0x19, 0xaa}, // P
    {0x2a, 0x10, 0xaa}, // Q
    {0x2a, 0x13, 0xaa}, // R
    {0x2a, 0x1f, 0xaa}, // S
    {0x2a, 0x14, 0xaa}, // T
    {0x2a, 0x16, 0xaa}, // U
    {0x2a, 0x2f, 0xaa}, // V
    {0x2a, 0x11, 0xaa}, // W
    {0x2a, 0x2d, 0xaa}, // X
    {0x2a, 0x15, 0xaa}, // Y
    {0x2a, 0x2c, 0xaa}, // Z
    {0x1a, NO, NO},     // [
    {0x2b, NO, NO},     // backslash
    {0x1b, NO, NO},     // ]
    {0x2a, 0x07, 0xaa}, // ^
    {0x2a, 0x0c, 0xaa}, // _

    /* 0x60 */
    {0x29, NO, NO}, // `
    {0x1e, NO, NO}, // a
    {0x30, NO, NO}, // b
    {0x2e, NO, NO}, // c
    {0x20, NO, NO}, // d
    {0x12, NO, NO}, // e
    {0x21, NO, NO}, // f
    {0x22, NO, NO}, // g
    {0x23, NO, NO}, // h
    {0x17, NO, NO}, // i
    {0x24, NO, NO}, // j
    {0x25, NO, NO}, // k
    {0x26, NO, NO}, // l
    {0x32, NO, NO}, // m
    {0x31, NO, NO}, // n
    {0x18, NO, NO}, // o

    /* 0x70 */
    {0x19, NO, NO},     // p
    {0x10, NO, NO},     // q
    {0x13, NO, NO},     // r
    {0x1f, NO, NO},     // s
    {0x14, NO, NO},     // t
    {0x16, NO, NO},     // u
    {0x2f, NO, NO},     // v
    {0x11, NO, NO},     // w
    {0x2d, NO, NO},     // x
    {0x15, NO, NO},     // y
    {0x2c, NO, NO},     // z
    {0x2a, 0x1a, 0xaa}, // {
    {0x2a, 0x2b, 0xaa}, // |
    {0x2a, 0x1b, 0xaa}, // }
    {0x2a, 0x29, 0xaa}, // ~
    {0x0e, NO, NO}      // delete character
};

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
    int i;
    int pushed = 0;
    for (i = 0; i < 3; i++)
    {
        if (scmap[c][i])
        {
            pushed = 1;
            kbd->buf[kbd->buf_index] = scmap[c][i];
            if (kbd->buf_index > 254)
            {
                kbd->buf_index = 0;
            }
            else
            {
                kbd->buf_index += 1;
            }
        }
    }
    return pushed;
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