#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>

#include "emulator.h"
#include "instructions.h"
#include "emulator_functions.h"
#include "ioapic.h"
#include "disk.h"
#include "kbd.h"
#include "mp.h"
#include "kbd.h"

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

IOAPIC *ioapic;
Emulator *emu;

#define DEBUG_SIZE 256

int debug_i = 0;
int debug_eips[DEBUG_SIZE];
int debug_ops[DEBUG_SIZE];
int debug_esp[DEBUG_SIZE];

void debug_append(uint32_t eip, uint8_t op, uint32_t esp_val)
{
    debug_eips[debug_i] = eip;
    debug_ops[debug_i] = op;
    debug_esp[debug_i] = esp_val;
    debug_i = (debug_i + 1) % DEBUG_SIZE;
}

void debug_print()
{
    printf("Last %d instructions:\n", DEBUG_SIZE);
    int i;
    for (i = debug_i; i < 256; i++)
    {
        printf("EIP: %x Op: %02x ESP: %x\n", debug_eips[i], debug_ops[i], debug_esp[i]);
    }
    for (i = 0; i < debug_i; i++)
    {
        printf("EIP: %x Op: %02x ESP: %x\n", debug_eips[i], debug_ops[i], debug_esp[i]);
    }
}

void termination_handler(int signum)
{
    if (signum == SIGSEGV)
    {
        debug_print();
        printf("Segmentation fault at EIP: %08X.\n", emu->eip);
        dump_registers(emu);
        exit(1);
    }
    debug_print();
    dump_registers(emu);
    dump_memory(emu, 0x7b00, 1024);
    // enable_canon_echo();
    exit(0);
}

void set_signals()
{
    struct sigaction new_action, old_action;

    new_action.sa_handler = termination_handler;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;

    sigaction(SIGSEGV, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction(SIGSEGV, &new_action, NULL);
    sigaction(SIGINT, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction(SIGINT, &new_action, NULL);
    sigaction(SIGHUP, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction(SIGHUP, &new_action, NULL);
    sigaction(SIGTERM, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction(SIGTERM, &new_action, NULL);
    sigaction(SIGQUIT, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction(SIGQUIT, &new_action, NULL);
    sigaction(SIGSTOP, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction(SIGSTOP, &new_action, NULL);
}

int main(int argc, char *argv[])
{
    FILE *binary; // FILE: pointer to stream
    int i;
    quiet = 0;

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

    set_signals();

    /* Main memory */
    uint8_t *memory = malloc(MEMORY_SIZE);

    /*
     * Initial setup: EIP: 0x7c00, ESP: 0x7c00
     * BIOS places instructions at 0x7c00.
     */
    emu = create_emu(memory, 0x7c00, 0x7c00);

    /* BIOS configures MP settings */
    set_mp_config(emu);

    /* Binary file loading */
    binary = fopen(argv[1], "rb"); // rb: read-binary (r: translated mode for "\n")
    if (binary == NULL)
    {
        printf("Could not open: %s", argv[1]);
        return 1;
    }
    Disk *disk = create_disk_device();
    load_data_to_disk(disk, binary);
    fclose(binary);

    attach_disk(emu, disk);

    init_ioapic();
    add_lapic(0, emu->lapic);

    load_boot_sector(emu);

    init_kbd(ioapic);

    /* dump_input(emu); */

    init_instructions();

    while ((emu->eip < MEMORY_SIZE) || (emu->is_pg))
    {
        uint8_t op = get_code8(emu, 0);
        if (!quiet)
            printf("CS: %04X EIP: %08X Op: %02X\n", get_seg_register16(emu, CS), emu->eip, op);

        if (instructions[op] == NULL)
        {
            debug_print();
            printf("EIP: %08X Op: %x not implemented.\n", emu->eip, op);
            dump_registers(emu);
            break;
        }

        debug_append(emu->eip, op, emu->registers[ESP]);

        instructions[op](emu);

        // dump_registers(emu);

        if (emu->eip == 0x00)
        {
            if (!quiet)
                printf("End of program :)\n");
            break;
        }
    }

    if (!quiet)
    {
        dump_registers(emu);
        dump_eflags(emu);
        dump_memory(emu, 0x7b00, 1024);
        dump_lapic(emu->lapic);
        dump_ioapic();
    }
    destroy_emu(emu);
    return 0;
}