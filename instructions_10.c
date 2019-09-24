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
 * push ss: 1 byte
 * Pushes value of SS to stack.
 * 1 byte: op (16)
 */
void push_ss(Emulator *emu)
{
    push_segment_register(emu, SS);
    emu->eip += 1;
}

/*
 * pop es: 1 byte
 * Pops value from stack to SS.
 * 1 byte: op (17)
 */
void pop_ss(Emulator *emu)
{
    pop_segment_register(emu, SS);
    emu->eip += 1;
}

/*
 * push ds: 1 byte
 * Pushes value of DS to stack.
 * 1 byte: op (1E)
 */
void push_ds(Emulator *emu)
{
    push_segment_register(emu, DS);
    emu->eip += 1;
}

/*
 * pop ds: 1 byte
 * Pops value from stack to DS.
 * 1 byte: op (1F)
 */
void pop_ds(Emulator *emu)
{
    pop_segment_register(emu, DS);
    emu->eip += 1;
}