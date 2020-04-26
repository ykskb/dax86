#ifndef DISK_H_
#define DISK_H_

#include <stdint.h>
#include <stdio.h>

/* Disk size: 10MB */
#define DISK_SIZE (1024 * 1024 * 10)

/*
 * <ATA>
 * 
 * IRQ:
 * - primary: 14
 * - secondary: 15
 * 1. Check PCI Busmaster Status byte, to verify that the IRQ came from the disk.
 * 2. Read Status Register once. (clears interrupt flag)
 * 3a. If the ERR bit is set, read Error Register.
 * 3b. If READ DMA operation, must read Busmaster Status Register.
 * 3c. If PIO read/write, check the status.
 * 4. Send EOI (0x20) to both PICs.
 * Device Control Register nIEN bit can be set to disable IRQ.
 * 
 * Addressing Modes
 * - 28-bit LBA
 *   0x0 to 0x0FFFFFFF (256M * 512 = 128GB)
 *   28-bit PIO is faster than 48-bit LBA
 * - 48-bit LBA
 * - CHS
 * 
 * Control Bus:
 * - primary: 0x1F0 through 0x1F7
 * - secondary: 0x170 through 0x177
 * 
 * Registers (port-base)
 * 
 * - IO Ports (assuming base is 0x1F0)
 *   port    R/W   name                    size_(LBA28/48)
 * | 0x1F0 | R/W | Data Register         | 16/16 |
 * | 0x1F1 | R   | Error Register        | 8/16  |
 * | 0x1F1 | W   | Features Register     | 8/16  |
 * | 0x1F2 | R/W | Sector Count Register | 8/16  | *0 is special value
 * | 0x1F3 | R/W | LBA Low               | 8/16  |
 * | 0x1F4 | R/W | LBA Mid               | 8/16  |
 * | 0x1F5 | R/W | LBA High              | 8/16  |
 * | 0x1F6 | R/W | Drive/Head Register   | 8/8   |
 * | 0x1F7 | R   | Status Register       | 8/8   |
 * | 0x1F7 | W   | Command Register      | 8/8   |
 * 
 * - Control Ports (assuming base is 0x3F6)
 *   port    R/W   name                        size_(LBA28/48)
 * | 0x3F6 | R   | Alternate Status Register | 8/8 | *status register isolated from interrupts
 * | 0x3F6 | W   | Device Control Register   | 8/8 | *resets bus or toggle interrupts
 * | 0x3F7 | R   | Drive Address Register    | 8/8 | *provides drive/head select info
 * 
 * - Status Register (0x1F7)
 * |  7  |  6  | 5  | 4   | 3   | 2    | 1   | 0   |
 * | BSY | RDY | DF | SRV | DRQ | CORR | IDX | ERR |
 * 
 * - Error Register (0x1F1)
 * |  7  |  6  | 5  | 4    | 3   | 2    | 1     | 0    |
 * | BBK | UNC | MC | IDNF | MCR | ABRT | TKZNF | AMNF |
 * 
 * - Drive/Head Register (0x1F6)
 * | 7 |  6  | 5 | 4   | 3     | 2     | 1     | 0    |
 * | 1 | LBA | 1 | DRV | Bits 24:27 of blk num in LBA | *Bits 0:3 of head in CHS
 * 
 * - Device Control Register (0x3F6)
 * |  7  |  6  | 5  | 4    | 3   | 2    | 1    | 0   |
 * | HOB | Rsv | Rsv | Rsv | Rsv | SRST | nIEN | Rsv |
 * 
 * - Drive Address Register (0x3F7)
 * |  7  |  6  | 5   | 4   | 3   | 2   | 1   | 0   |
 * | Rsv | WTG | HS3 | HS2 | HS1 | HS0 | DS1 | DS0 |
 * 
 * Ready Status
 * - Status port bit 7 (BSY) clears && bit 3 (DRQ) sets (& 0xC0: 1100 0000 = 0x40: 0100 0000)
 * - Status port bit 0 (ERR) sets 
 * - Status port bit 5 (DF) sets
 */
typedef struct
{
    uint8_t *storage;
    /* Registers */
    uint16_t data;
    uint8_t sec_count;
    uint8_t lba_low;
    uint8_t lba_mid;
    uint8_t lba_high;
    uint8_t drive_head;
    uint8_t status_command;
    /* Utility */
    uint32_t head_index;
} Disk;

Disk *create_disk_device();

/* Utility method for loading binary file to emu */
void load_data_to_disk(Disk *disk, FILE *f);

void set_sec_count(Disk *disk, uint8_t val);
void set_lba_low(Disk *disk, uint8_t val);
void set_lba_mid(Disk *disk, uint8_t val);
void set_lba_high(Disk *disk, uint8_t val);
void set_drive_head(Disk *disk, uint8_t val);
void set_disk_command(Disk *disk, uint8_t val);

uint8_t get_disk_status(Disk *disk);
uint32_t read_disk_data32(Disk *disk);

#endif