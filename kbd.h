#ifndef KBD_H_
#define KBD_H_

#include <stdint.h>

#include "ioapic.h"

#define KBD_BUF_SIZE 255
#define KBD_STATUS_IN 2
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
void write_ps2_output_port(uint8_t value);
void write_ps2_config_byte(uint8_t value);

#endif