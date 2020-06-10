#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "disk.h"
#include "util.h"

#define SECTOR_SIZE 512

Disk *create_disk_device()
{
    Disk *disk = malloc(sizeof(Disk));
    disk->storage = malloc(DISK_SIZE);
    disk->data = 0;
    disk->sec_count = 0;
    disk->lba_low = 0;
    disk->lba_mid = 0;
    disk->lba_high = 0;
    disk->drive_head = 0;
    disk->status_command = 0x40;
    return disk;
}

void load_data_to_disk(Disk *disk, FILE *f)
{
    fseek(f, 0L, SEEK_END);
    long binary_size = ftell(f);
    fseek(f, 0L, SEEK_SET);
    if (binary_size > DISK_SIZE)
    {
        printf("Binary file bigger than disk size.");
        panic();
    }
    fread(disk->storage, 1, binary_size, f);
}

void set_sec_count(Disk *disk, uint8_t val)
{
    disk->sec_count = val;
}

void set_lba_low(Disk *disk, uint8_t val)
{
    disk->lba_low = val;
}

void set_lba_mid(Disk *disk, uint8_t val)
{
    disk->lba_mid = val;
}

void set_lba_high(Disk *disk, uint8_t val)
{
    disk->lba_high = val;
}

void set_drive_head(Disk *disk, uint8_t val)
{
    disk->drive_head = val;
}

static void set_head_index(Disk *disk)
{
    uint32_t head_index = 0 | disk->lba_low | (disk->lba_mid << 8) | (disk->lba_high << 16) |
                          ((disk->drive_head & 0x0F) << 24);
    disk->head_index = head_index * SECTOR_SIZE;
}

void set_disk_command(Disk *disk, uint8_t val)
{
    switch (val)
    {
    case 0x20:
        /* read sectors */
        set_head_index(disk);
        break;
    default:
        break;
    }
}

uint8_t get_disk_status(Disk *disk)
{
    return disk->status_command;
}

uint8_t read_disk_data8(Disk *disk)
{
    uint8_t data = disk->storage[disk->head_index];
    disk->head_index += 1;
    return data;
}

uint32_t read_disk_data32(Disk *disk)
{
    int i;
    uint32_t data = 0;
    for (i = 0; i < 4; i++)
    {
        data |= read_disk_data8(disk) << (8 * i);
    }
    return data;
}