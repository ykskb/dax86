#include "io.h"
#include <stdint.h>
#include <stdio.h>
#include "emulator.h"
#include "kbd.h"

/* 
 * 0x60
 * PS/2 (keyboard) Controller
 * in/out: Data Register
 * 
 * 0x64
 * PS/2 (keyboard) Controller
 * in: Status Register
 * out: Command Register
 * 
 * 0x3f8
 * Serial COM1 port: Convenience Implementation
 * Actual IO checks 0x03fd (status port) and read 
 * from 0x3f8 (data port) if data is there.
 * Also while loop below clears buffer if needed.
 * while ((ch = getchar()) != '\n' && ch != EOF) continue;
 */

uint8_t io_in8(Emulator *emu, uint16_t address)
{
    switch (address)
    {
    case 0x64:
        return get_kbd_status();
    case 0x1f7:
        return get_disk_status(emu->disk);
    case 0x3f8:
        return getchar();
    default:
        printf("IN8 on port %x not implemented.\n", address);
        return 0;
    }
}

uint32_t io_in32(Emulator *emu, uint16_t address)
{
    switch (address)
    {
    case 0x1F0:
        return read_disk_data32(emu->disk);
    case 0x3f8:
        return getchar();
    default:
        printf("IN32 on port %x not implemented.\n", address);
        return 0;
    }
}

void io_out8(Emulator *emu, uint16_t address, uint8_t value)
{
    switch (address)
    {
    case 0x60:
        write_ps2_config_byte(value);
        break;
    case 0x64:
        write_ps2_output_port(value);
        break;
    case 0x1f2:
        set_sec_count(emu->disk, value);
        break;
    case 0x1f3:
        set_lba_low(emu->disk, value);
        break;
    case 0x1f4:
        set_lba_mid(emu->disk, value);
        break;
    case 0x1f5:
        set_lba_high(emu->disk, value);
        break;
    case 0x1f6:
        set_drive_head(emu->disk, value);
        break;
    case 0x1f7:
        set_disk_command(emu->disk, value);
        break;
    case 0x3f8:
        putchar(value);
        break;
    default:
        printf("OUT8 on port %x not implemented.\n", address);
        break;
    }
}

void io_out32(Emulator *emu, uint16_t address, uint32_t value)
{
    switch (address)
    {
    case 0x3f8:
        putchar(value);
        break;
    default:
        printf("OUT32 on port %x not implemented.\n", address);
        break;
    }
}
