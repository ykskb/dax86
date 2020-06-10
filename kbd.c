
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

uint8_t scmap[128] = {
    /* 0x00 */
    NO,
    NO,
    NO,
    NO,
    NO,
    NO,
    NO,
    NO,
    0x0e, // backspace
    0x0f, // tab
    0x1c, // enter (LF)
    NO,
    NO,
    NO,
    NO,
    NO,
    /* 0x10 */
    NO,
    NO,
    NO,
    NO,
    NO,
    NO,
    NO,
    NO,
    NO,
    NO,
    NO,
    0x01, // esc
    NO,
    NO,
    NO,
    NO,
    /* 0x20 */
    0x39, // space
    NO,   // !
    NO,   // "
    NO,   // #
    NO,   // $
    NO,   // %
    NO,   // &
    0x28, // '
    NO,   // (
    NO,   // )
    NO,   // *
    NO,   // +
    0x33, // ,
    0x0c, // -
    0x34, // .
    0x35, // /

    /* 0x30 */
    0x0b, // 0
    0x02, // 1
    0x03, // 2
    0x04, // 3
    0x05, // 4
    0x06, // 5
    0x07, // 6
    0x08, // 7
    0x09, // 8
    0x0a, // 9
    NO,   // :
    0x27, // ;
    NO,   // <
    0x0d, // =
    NO,   // >
    NO,   // ?

    /* 0x40 */
    NO, // @
    NO, // A
    NO, // B
    NO, // C
    NO, // D
    NO, // E
    NO, // F
    NO, // G
    NO, // H
    NO, // I
    NO, // J
    NO, // K
    NO, // L
    NO, // M
    NO, // N
    NO, // O

    /* 0x50 */
    NO,   // P
    NO,   // Q
    NO,   // R
    NO,   // S
    NO,   // T
    NO,   // U
    NO,   // V
    NO,   // W
    NO,   // X
    NO,   // Y
    NO,   // Z
    0x1a, // [
    0x2b, // backslash
    0x1b, // ]
    NO,   // ^
    NO,   // _

    /* 0x60 */
    0x29, // `
    0x1e, // a
    0x30, // b
    0x2e, // c
    0x20, // d
    0x12, // e
    0x21, // f
    0x22, // g
    0x23, // h
    0x17, // i
    0x24, // j
    0x25, // k
    0x26, // l
    0x32, // m
    0x31, // n
    0x18, // o

    /* 0x70 */
    0x19, // p
    0x10, // q
    0x13, // r
    0x1f, // s
    0x14, // t
    0x16, // u
    0x2f, // v
    0x11, // w
    0x2d, // x
    0x15, // y
    0x2c, // z
    NO,   // {
    NO,   // |
    NO,   // }
    NO,   // ~
    0x0e  // delete character
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