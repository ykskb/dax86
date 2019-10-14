#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "instruction_defs.h"
#include "emulator_functions.h"
#include "modrm.h"
#include "io.h"

/*
 * pushad: 1 bytes
 * Pushes contents of general-purpose registers into stack.
 * 1 byte: op (60)
 */
void pushad(Emulator *emu)
{
    uint32_t original_esp = get_register32(emu, ESP);
    push32(emu, get_register32(emu, EAX));
    push32(emu, get_register32(emu, ECX));
    push32(emu, get_register32(emu, EDX));
    push32(emu, get_register32(emu, EBX));
    push32(emu, original_esp);
    push32(emu, get_register32(emu, EBP));
    push32(emu, get_register32(emu, ESI));
    push32(emu, get_register32(emu, EDI));
    emu->eip += 1;
}

/*
 * popad: 1 bytes
 * Pops from stack into general-purpose registers.
 * 1 byte: op (61)
 */
void popad(Emulator *emu)
{
    set_register32(emu, EDI, pop32(emu));
    set_register32(emu, ESI, pop32(emu));
    set_register32(emu, EBP, pop32(emu));
    /* Skips original ESP value by incrementing ESP by 4 bytes. */
    set_register32(emu, ESP, get_register32(emu, ESP) + 4);
    set_register32(emu, EBX, pop32(emu));
    set_register32(emu, EDX, pop32(emu));
    set_register32(emu, ECX, pop32(emu));
    set_register32(emu, EAX, pop32(emu));
    emu->eip += 1;
}

/*
 * push imm32: 5 bytes
 * Pushes 32-bit immediate value into stack.
 * 1 byte: op (68)
 * 4 bytes: immediate 32-bit value
 */
void push_imm32(Emulator *emu)
{
    uint32_t value = get_code32(emu, 1);
    push32(emu, value);
    emu->eip += 5;
}

/*
 * push imm8: 2 bytes
 * Pushes 8-bit immediate value into stack.
 * 1 byte: op (6A)
 * 1 byte: immediate 8-bit value
 */
void push_imm8(Emulator *emu)
{
    uint8_t value = get_code8(emu, 1);
    push32(emu, (uint32_t)value);
    emu->eip += 2;
}