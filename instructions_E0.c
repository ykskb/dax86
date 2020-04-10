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
 * loopnz rel8: 2 bytes
 * Jumps short after decrementing ECX (CX) if count != 0 and ZF==0.
 * 1 byte: op (E0)
 * 1 byte: rel8 value
 * Address size is assumed to be 32 bits here.
 */
void loopnz(Emulator *emu)
{
    uint32_t ecx_val = get_register32(emu, ECX);
    if (is_zero(emu))
    {
        emu->eip += 2;
        return;
    }
    if (ecx_val > 0)
    {
        ecx_val -= 1;
        set_register32(emu, ECX, ecx_val);
    }
    if (ecx_val != 0)
    {
        int8_t offset = get_sign_code8(emu, 1);
        emu->eip += (offset + 2);
        return;
    }
    emu->eip += 2;
}

/*
 * loopz rel8: 2 bytes
 * Jumps short after decrementing ECX (CX) if count != 0 and ZF==1.
 * 1 byte: op (E1)
 * 1 byte: rel8 value
 * Address size is assumed to be 32 bits here.
 */
void loopz(Emulator *emu)
{
    uint32_t ecx_val = get_register32(emu, ECX);
    if (!is_zero(emu))
    {
        emu->eip += 2;
        return;
    }
    if (ecx_val > 0)
    {
        ecx_val -= 1;
        set_register32(emu, ECX, ecx_val);
    }
    if (ecx_val != 0)
    {
        int8_t offset = get_sign_code8(emu, 1);
        emu->eip += (offset + 2);
        return;
    }
    emu->eip += 2;
}

/*
 * loop rel8: 2 bytes
 * Jumps short after decrementing ECX (CX) if count != 0.
 * 1 byte: op (E2)
 * 1 byte: rel8 value
 * Address size is assumed to be 32 bits here.
 */
void loop(Emulator *emu)
{
    uint32_t ecx_val = get_register32(emu, ECX);
    if (ecx_val > 0)
    {
        ecx_val -= 1;
        set_register32(emu, ECX, ecx_val);
    }
    if (ecx_val != 0)
    {
        int8_t offset = get_sign_code8(emu, 1);
        emu->eip += (offset + 2);
        return;
    }
    emu->eip += 2;
}

/*
 * jecxz rel8: 2 bytes
 * Jumps if ecx == 0.
 * 1 byte: op (E3)
 * 1 byte: rel8 value
 */
void jecxz(Emulator *emu)
{
    uint32_t ecx_val = get_register32(emu, ECX);
    if (ecx_val == 0)
    {
        int8_t offset = get_sign_code8(emu, 1);
        emu->eip += (offset + 2);
    }
    else
    {
        emu->eip += 2;
    }
}

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
 * jmp ptr16:16/32: 7 bytes
 * Jumps in ptr16:16/32.
 * 1 byte: op (EA)
 * 2 byte: cs value
 * 4 byte: eip
 */
void ptr_jump(Emulator *emu)
{
    uint32_t eip_val = get_code32(emu, 1);
    uint16_t cs_val = get_code16(emu, 5);
    set_seg_register16(emu, CS, cs_val);
    emu->eip = eip_val;
    check_protected_mode_entry(emu);
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
 * in eax dx: 1 byte
 * Input data to EAX from IO address specified on DX.
 * 1 byte: op (ED)
 */
void in_eax_dx(Emulator *emu)
{
    /* IO Port Address from DX */
    uint16_t address = get_register32(emu, EDX) & 0xffff;
    uint32_t value = io_in32(address);
    set_register32(emu, EAX, value);
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

/*
 * out dx eax: 1 byte
 * Output data on EAX to IO address specified on DX.
 * 1 byte: op (EF)
 */
void out_dx_eax(Emulator *emu)
{
    /* IO Port Address from DX */
    uint16_t address = get_register32(emu, EDX) & 0xffff;
    uint32_t value = get_register8(emu, EAX);
    io_out32(address, value);
    emu->eip += 1;
}