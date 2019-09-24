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
 * call rel32: 5 bytes
 * Jumps by 32-bit number relatively from next address.
 * 1 byte: op (E8)
 * 4 bytes: relative number to jump.
 */
void call_rel32(Emulator *emu)
{
    /* Offset value should be after the op code of 1 byte. */
    int32_t offset = get_sign_code32(emu, 1);
    /* Pushes the address after this call instruction. */
    push32(emu, emu->eip + 5);
    /* Adds the offset to EIP. */
    emu->eip += (offset + 5);
}

/*
 * jmp (near): 5 bytes
 * Jumps with 32-bit signed offset.
 * 1 byte: op (E9)
 * 4 byte: offset from eip (32 bit signed)
 */
void near_jump(Emulator *emu)
{
    int32_t diff = get_sign_code32(emu, 1);
    emu->eip += (diff + 5);
}

/*
 * jmp (short): 2 bytes
 * Jumps with 8-bit signed offset.
 * 1 byte: op (EB)
 * 1 byte: offset from eip (8 bit signed) -127 ~ 127
 */
void short_jump(Emulator *emu)
{
    int8_t offset = get_sign_code8(emu, 1);
    emu->eip += (offset + 2);
}

/*
 * in al dx: 1 byte
 * Input data to AL from IO address specified on DX.
 * 1 byte: op (EC)
 */
void in_al_dx(Emulator *emu)
{
    /* IO Port Address from DX */
    uint16_t address = get_register32(emu, EDX) & 0xffff;
    uint8_t value = io_in8(address);
    set_register8(emu, AL, value);
    emu->eip += 1;
}

/*
 * out dx al: 1 byte
 * Output data on AL to IO address specified on DX.
 * 1 byte: op (EE)
 */
void out_dx_al(Emulator *emu)
{
    /* IO Port Address from DX */
    uint16_t address = get_register32(emu, EDX) & 0xffff;
    uint8_t value = get_register8(emu, AL);
    io_out8(address, value);
    emu->eip += 1;
}