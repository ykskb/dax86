#include <stdlib.h>
#include <unistd.h>

#include "util.h"
#include "emulator.h"
#include "lapic.h"
#include "ioapic.h"

#include <termios.h>

#define DEBUG_SIZE 256

Config config;
int debug_i = 0;
int debug_cs[DEBUG_SIZE];
int debug_eips[DEBUG_SIZE];
int debug_ops[DEBUG_SIZE];
int debug_ss[DEBUG_SIZE];
int debug_esp[DEBUG_SIZE];

void init_config(int verbose, int test)
{
    config.verbose = verbose;
    config.test = test;
}

void debug_append(uint16_t cs, uint32_t eip, uint8_t op, uint16_t ss, uint32_t esp_val)
{
    debug_cs[debug_i] = cs;
    debug_eips[debug_i] = eip;
    debug_ops[debug_i] = op;
    debug_ss[debug_i] = ss;
    debug_esp[debug_i] = esp_val;
    debug_i = (debug_i + 1) % DEBUG_SIZE;
}

void debug_print()
{
    printf("Last %d instructions:\n", DEBUG_SIZE);
    int i;
    for (i = debug_i; i < 256; i++)
    {
        printf("CS: %04x EIP: %08x Op: %02x SS: %04x ESP: %08x\n", debug_cs[i], debug_eips[i], debug_ops[i], debug_ss[i], debug_esp[i]);
    }
    for (i = 0; i < debug_i; i++)
    {
        printf("CS: %04x EIP: %08x Op: %02x SS: %04x ESP: %08x\n", debug_cs[i], debug_eips[i], debug_ops[i], debug_ss[i], debug_esp[i]);
    }
}

void print_emu(Emulator *emu)
{
    dump_registers(emu);
    dump_eflags(emu);
    dump_memory(emu, 0x7b00, 1024);
    dump_lapic(emu->lapic);
    dump_ioapic();
}

void add_canon_echo()
{
    static struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void remove_canon_echo()
{
    static struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void panic()
{
    add_canon_echo();
    exit(1);
}

void panic_exit(Emulator *emu)
{
    add_canon_echo();
    debug_print();
    print_emu(emu);
    exit(1);
}

void sig_exit(Emulator *emu)
{
    add_canon_echo();
    debug_print();
    print_emu(emu);
    exit(0);
}