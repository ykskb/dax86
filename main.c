#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "emulator.h"
#include "instructions.h"
#include "emulator_functions.h"
#include "disk.h"

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
     * Initial setup: EIP: 0x7c00, ESP: 0x7c00
     * BIOS places instructions at 0x7c00.
     */
    emu = create_emu(0x7c00, 0x7c00);

    binary = fopen(argv[1], "rb"); // rb: read-binary (r: translated mode for "\n")
    if (binary == NULL)
    {
        printf("Could not open: %s", argv[1]);
        return 1;
    }

    load_data_to_disk(emu->disk, binary);
    load_boot_sector(emu, binary);

    fclose(binary);

    /* dump_input(emu); */

    init_instructions();

    while ((emu->eip < MEMORY_SIZE) || (emu->is_pg))
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