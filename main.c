#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>

#include "emulator.h"
#include "instructions.h"
#include "emulator_functions.h"
#include "ioapic.h"
#include "disk.h"
#include "kbd.h"
#include "mp.h"
#include "interrupt.h"
#include "util.h"

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

void termination_handler(int signum)
{
    if (signum == SIGSEGV)
    {
        printf("Segmentation fault at EIP: %08X.\n", emu->eip);
        panic_exit(emu);
    }
    sig_exit(emu);
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
    int i = 0;
    init_config(0, 0);

    while (i < argc)
    {
        if (strcmp(argv[i], "-v") == 0)
        {
            config.verbose = 1;
            argc = remove_arg_at(argc, argv, i);
        }
        else if (strcmp(argv[i], "test") == 0)
        {
            config.test = 1;
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

    set_signals();
    remove_canon_echo();

    while ((emu->eip < MEMORY_SIZE) || (emu->is_pg))
    {
        uint8_t op = get_code8(emu, 0);
        if (config.verbose)
            printf("CS: %04X EIP: %08X Op: %02X\n", get_seg_register16(emu, CS), emu->eip, op);

        if (instructions[op] == NULL)
        {
            printf("EIP: %08X Op: %x not implemented.\n", emu->eip, op);
            panic_exit(emu);
        }

        debug_append(emu->segment_registers[CS], emu->eip, op, emu->segment_registers[SS], emu->registers[ESP]);

        instructions[op](emu);

        if (emu->int_enabled == 1 && emu->int_r > 0)
        {
            handle_interrupt(emu, emu->int_r, 0);
            emu->int_r = 0;
        }

        // dump_registers(emu);

        if (config.test && emu->eip == 0x00)
        {
            if (config.verbose)
                printf("End of program :)\n");
            break;
        }
    }

    if (config.verbose)
    {
        print_emu(emu);
    }
    destroy_emu(emu);
    normal_exit();
}