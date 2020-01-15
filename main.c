#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "emulator.h"
#include "instructions.h"
#include "emulator_functions.h"

/* Memory size: 1MB */
#define MEMORY_SIZE (1024 * 1024)

char *register_names[] = {"EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI"};
char *seg_register_names[] = {"ES", "CS", "SS", "DS", "FS", "GS"};
char *ctr_register_names[] = {"CR0", "CR1", "CR2", "CR3", "CR4"};

static Emulator *create_emu(size_t mem_size, uint32_t eip, uint32_t esp)
{
    Emulator *emu = malloc(sizeof(Emulator));
    emu->memory = malloc(mem_size);

    /* Resets registers. */
    memset(emu->registers, 0, sizeof(emu->registers));
    memset(emu->segment_registers, 0, sizeof(emu->segment_registers));
    memset(emu->control_registers, 0, sizeof(emu->control_registers));
    emu->eip = eip;
    emu->registers[ESP] = esp;

    return emu;
}

static void destroy_emu(Emulator *emu)
{
    free(emu->memory);
    free(emu);
}

static void dump_registers(Emulator *emu)
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
static void dump_input(Emulator *emu)
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

static void dump_memory(Emulator *emu)
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

static void dump_eflags(Emulator *emu)
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

int remove_arg_at(int argc, char *argv[], int index)
{
    if (index < 0 || argc <= index)
    {
        return argc;
    }
    else
    {
        int i = index;
        for (; i < argc - 1; i++)
        {
            argv[i] = argv[i + 1];
        }
        argv[i] = NULL; // last element
        return argc - 1;
    }
}

int main(int argc, char *argv[])
{
    FILE *binary; // FILE: pointer to stream
    Emulator *emu;
    int i;
    int quiet = 0;

    while (i < argc)
    {
        if (strcmp(argv[i], "-q") == 0)
        {
            quiet = 1;
            argc = remove_arg_at(argc, argv, i);
        }
        else
        {
            i++;
        }
    }

    if (argc != 2)
    {
        printf("Usage: dax86 [filename]\n");
        return 1;
    }

    /*
    Initial setup: EIP: 0x7c00, ESP: 0x7c00
    BIOS places instructions at 0x7c00.
    */
    emu = create_emu(MEMORY_SIZE, 0x7c00, 0x7c00);

    binary = fopen(argv[1], "rb"); // rb: read-binary (r: translated mode for "\n")
    if (binary == NULL)
    {
        printf("Could not open: %s", argv[1]);
        return 1;
    }

    /* 
    Loads binary into emulator.
    Each index stores 1 byte of binary.
    Offset beginning by 0x7c00 which is program placement by BIOS.
    0x200 (512) * 1 byte = 512 bytes
    */
    fread(emu->memory + 0x7c00, 1, 0x200, binary); // fread(*ptr, byte, num(of byte to read), *stream)
    fclose(binary);

    // dump_input(emu);

    init_instructions();

    while (emu->eip < MEMORY_SIZE)
    {
        uint8_t op = get_code8(emu, 0);
        if (!quiet)
        {
            printf("CS: %04X EIP: %08X Op: %02X\n", get_seg_register16(emu, CS), emu->eip, op);
        }

        if (instructions[op] == NULL)
        {
            printf("Op: %x not implemented.\n", op); // %x: int as hex
            break;
        }

        instructions[op](emu);

        // dump_registers(emu);

        if (emu->eip == 0x00)
        {
            if (!quiet)
            {
                printf("End of program :)\n");
            }
            break;
        }
    }

    if (!quiet)
    {
        dump_registers(emu);
        dump_eflags(emu);
        dump_memory(emu);
    }
    destroy_emu(emu);
    return 0;
}