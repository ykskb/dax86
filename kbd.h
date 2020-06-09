#ifndef KBD_H_
#define KBD_H_

#include <stdint.h>

#include "ioapic.h"

#define KBD_BUF_SIZE 255
#define KBD_STATUS_IN 2

#define NO 0
/*
 * Status Register
 * |_b_|_meaning________________________________________|
 * | 0 | Output Buffer: 0=empty / 1=full             *a |
 * | 1 | Input Buffer: 0=empty / 1=full              *b |
 * | 2 | System Flag: 0 on reset / 1 after passing POST |
 * | 3 | Command/Data: 0=data 1=command for device      |
 * | 4 | Chipset specific                               |
 * | 5 | Chipset specific                               |
 * | 6 | Time-out Error: 0=noerror / 1=error            |
 * | 7 | Parity Error: 0=noerror / 1=error              |
 * |____________________________________________________|
 * a. must be set before attempting to read data from IO port 0x60
 * b. must be clear before attempting to write data to IO port 0x60 or IO port 0x64
 */
typedef struct
{
    uint8_t status;
    IOAPIC *ioapic;
    unsigned int buf[256];
    uint8_t buf_index;
    uint8_t buf_out_index;
    uint8_t buf_index_reset;
} KBD;

void init_kbd(IOAPIC *ioapic);
uint8_t get_kbd_status();
uint8_t get_kbd_data();
void write_ps2_output_port(uint8_t value);
void write_ps2_config_byte(uint8_t value);

static uint8_t scmap[128] = {
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
    NO};

#endif