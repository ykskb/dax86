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

static Emulator *create_emu(size_t size, uint32_t eip, uint32_t esp)
{
    Emulator *emu = malloc(sizeof(Emulator));
    emu->memory = malloc(size);

    /* Resets registers. */
    memset(emu->registers, 0, sizeof(emu->registers));
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
}

int main(int argc, char *argv[])
{
    FILE *binary; // FILE: pointer to stream
    Emulator *emu;

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

    init_instructions();

    while (emu->eip < MEMORY_SIZE)
    {
        uint8_t op = get_code8(emu, 0);
        if (instructions[op] == NULL)
        {
            printf("Op: %x not implemented.\n", op); // %x: int as hex
            break;
        }

        instructions[op](emu);

        if (emu->eip == 0x00)
        {
            printf("End of program :)\n");
            break;
        }
    }

    dump_registers(emu);
    destroy_emu(emu);
    return 0;
}
