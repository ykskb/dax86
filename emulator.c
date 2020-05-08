#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "emulator.h"
#include "lapic.h"

/* Util for Print Binary */
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)       \
    (byte & 0x80 ? '1' : '0'),     \
        (byte & 0x40 ? '1' : '0'), \
        (byte & 0x20 ? '1' : '0'), \
        (byte & 0x10 ? '1' : '0'), \
        (byte & 0x08 ? '1' : '0'), \
        (byte & 0x04 ? '1' : '0'), \
        (byte & 0x02 ? '1' : '0'), \
        (byte & 0x01 ? '1' : '0')

Emulator *create_emu(uint32_t eip, uint32_t esp)
{
    Emulator *emu = malloc(sizeof(Emulator));

    /* Resets registers. */
    memset(emu->registers, 0, sizeof(emu->registers));
    memset(emu->segment_registers, 0, sizeof(emu->segment_registers));
    memset(emu->control_registers, 0, sizeof(emu->control_registers));
    emu->eip = eip;
    emu->registers[ESP] = esp;
    emu->int_r = 0;

    /* Devices */
    emu->lapic = create_lapic(emu);
    emu->memory = malloc(MEMORY_SIZE);
    emu->disk = NULL;

    /* Utility */
    emu->is_pe = 0;
    emu->is_pg = 0;
    emu->int_enabled = 1;
    emu->exception = NO_ERR;

    return emu;
}

void destroy_emu(Emulator *emu)
{
    free(emu->memory);
    free(emu);
}

void attach_disk(Emulator *emu, Disk *disk)
{
    emu->disk = disk;
}

/*
 * Loads first sector of binary file into emulator.
 * Each index stores 1 byte of binary.
 * BIOS places a sector ending with 0x55AA at 0x7c00.
 * This check is not implemented intentionally.
 * 0x200 (512) * 1 byte = 512 bytes
 * fread(*ptr, byte, num(of byte to read), *stream)
 */
void load_boot_sector(Emulator *emu)
{
    memcpy(emu->memory + 0x7c00, emu->disk->storage, 512);
}

char *register_names[] = {"EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI"};
char *seg_register_names[] = {"ES", "CS", "SS", "DS", "FS", "GS"};
char *ctr_register_names[] = {"CR0", "CR1", "CR2", "CR3", "CR4"};

void dump_registers(Emulator *emu)
{
    int i;
    for (i = 0; i < REGISTERS_COUNT; i++)
    {
        printf("%s: %08x\n", register_names[i], emu->registers[i]); // %08x: prepends 0 to make 8 digits.
    }
    printf("EIP: %08x\n", emu->eip);
    for (i = 0; i < SEGMENT_REGISTERS_COUNT; i++)
    {
        printf("%s: %08x\n", seg_register_names[i], emu->segment_registers[i]);
    }
    for (i = 0; i < CONTROL_REGISTER_COUNT; i++)
    {
        printf("%s: %08x\n", ctr_register_names[i], emu->control_registers[i]);
    }
    printf("GDTR: %04x %08x\n", emu->gdtr.limit, emu->gdtr.base);
}

/*
void dump_input(Emulator *emu)
{
    printf("Input:\n");
    int i;
    for (i = 0; i < MEMORY_SIZE; i++)
    {
        if (emu->memory[i])
        {
            printf("%08x\n", emu->memory[i]);
        }
    }
}
*/

void dump_memory(Emulator *emu)
{
    int i = 0x7b00;
    int i_per_line = 32;
    int display_length = 1024;
    int end_i = i + display_length;
    int j;
    printf("memory %X to %X:\n", i, end_i);
    printf("          ");
    for (j = 0; j < i_per_line; j++)
    {
        printf("%02X ", j);
    }
    printf("\n");
    while (i < end_i)
    {
        printf("%08X: ", i);
        for (j = 0; j < i_per_line; j++)
        {
            printf("%02X", emu->memory[i + j]);
            printf(" ");
        }
        i += i_per_line;
        printf("\n");
    }
}

void dump_eflags(Emulator *emu)
{
    int i;
    printf("eflags:");
    for (i = 3; i > -1; i--)
    {
        printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(emu->eflags >> (8 * i)));
        printf(" ");
    }
    printf("\n");
}