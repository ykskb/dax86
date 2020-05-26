#include "io.h"
#include <stdint.h>
#include <stdio.h>
#include "emulator.h"
#include "kbd.h"
#include "instructions.h"

/* 
 * PS/2 (keyboard) Controller
 * 0x60: in|out: Data Register
 * 0x64: in: Status Register | out: Command Register
 */
#define PS2DATA 0x60
#define PS2STACMD 0x64

/* 
 * Serial (COM1)
 * 0x3f8: in|out: Data port
 * 0x3fd: in: Status port
 */
#define SERIALSTA 0x3fd
#define SERIALDATA 0x3f8
#define SERIALLINESTA 0x3fd

/*
 * Disk
 * 0x1F0: Data Register
 * 0x1F2: in|out: Sector Count Register
 * 0x1F3: in|out: LBA Low
 * 0x1F4: in|out: LBA Mid
 * 0x1F5: in|out: LBA High
 * 0x1F6: in|out: Drive/Head Register
 * 0x1F7: in: Status Register | out: Command Register
 */
#define DISKDATA 0x1f0
#define DISKSECCOUNT 0x1f2
#define DISKLBALOW 0x1f3
#define DISKLBAMID 0x1f4
#define DISKLBAHIGH 0x1f5
#define DISKDRVHEAD 0x1f6
#define DISKSTACMD 0x1f7

uint8_t io_in8(Emulator *emu, uint16_t address)
{
    switch (address)
    {
    case PS2STACMD:
        return get_kbd_status();
    case DISKSTACMD:
        return get_disk_status(emu->disk);
    case SERIALDATA:
        return 0;
        // return getchar();
    case SERIALLINESTA:
        return 0x20;
    default:
        if (!quiet)
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
    case SERIALDATA:
        return 0;
        // return getchar();
    default:
        if (!quiet)
            printf("IN32 on port %x not implemented.\n", address);
        return 0;
    }
}

void io_out8(Emulator *emu, uint16_t address, uint8_t value)
{
    switch (address)
    {
    case PS2DATA:
        write_ps2_config_byte(value);
        break;
    case PS2STACMD:
        write_ps2_output_port(value);
        break;
    case DISKSECCOUNT:
        set_sec_count(emu->disk, value);
        break;
    case DISKLBALOW:
        set_lba_low(emu->disk, value);
        break;
    case DISKLBAMID:
        set_lba_mid(emu->disk, value);
        break;
    case DISKLBAHIGH:
        set_lba_high(emu->disk, value);
        break;
    case DISKDRVHEAD:
        set_drive_head(emu->disk, value);
        break;
    case DISKSTACMD:
        set_disk_command(emu->disk, value);
        break;
    case SERIALDATA:
        putchar(value);
        break;
    default:
        if (!quiet)
            printf("OUT8 on port %x not implemented.\n", address);
        break;
    }
}

void io_out32(Emulator *emu, uint16_t address, uint32_t value)
{
    switch (address)
    {
    case SERIALDATA:
        putchar(value);
        break;
    default:
        if (!quiet)
            printf("OUT32 on port %x not implemented.\n", address);
        break;
    }
}
