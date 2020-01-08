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
 * mov al moffs8: 5 bytes
 * Copies moffs value to AL. (8 bit)
 * 1 byte: op (A0)
 * 4 byte: moffs
 */
void mov_al_moffs8(Emulator *emu)
{
    uint32_t offset = get_code32(emu, 1);
    uint8_t mem_val = get_memory8(emu, DS, offset);
    set_register8(emu, AL, mem_val);
    emu->eip += 5;
}

/*
 * mov eax moffs16/32: 5 bytes
 * Copies moffs value to AX/EAX. (16/32 bit)
 * 1 byte: op (A1)
 * 4 byte: moffs
 */
void mov_eax_moffs32(Emulator *emu)
{
    uint32_t offset = get_code32(emu, 1);
    uint32_t mem_val = get_memory32(emu, DS, offset);
    set_register32(emu, EAX, mem_val);
    emu->eip += 5;
}

/*
 * mov moffs8 al: 5 bytes
 * Copies AL value to moffs. (8 bit)
 * 1 byte: op (A2)
 * 4 byte: moffs
 */
void mov_moffs8_al(Emulator *emu)
{
    uint32_t offset = get_code32(emu, 1);
    uint8_t al_val = get_register8(emu, AL);
    set_memory8(emu, DS, offset, al_val);
    emu->eip += 5;
}

/*
 * mov moffs16/32 eax: 5 bytes
 * Copies AX/EAX value to moffs. (16/32 bit)
 * 1 byte: op (A3)
 * 4 byte: moffs
 */
void mov_moffs32_eax(Emulator *emu)
{
    uint32_t offset = get_code32(emu, 1);
    uint32_t eax_val = get_register32(emu, EAX);
    set_memory32(emu, DS, offset, eax_val);
    emu->eip += 5;
}

static void update_counter(Emulator *emu, int reg_index, uint32_t val, uint32_t byte_num)
{
    if (is_direction_down(emu))
    {
        set_register32(emu, reg_index, val - byte_num);
    }
    else
    {
        set_register32(emu, reg_index, val + byte_num);
    }
}

static void update_esi_edi(Emulator *emu, uint32_t esi_val, uint32_t edi_val, uint32_t byte_num)
{
    update_counter(emu, ESI, esi_val, byte_num);
    update_counter(emu, EDI, edi_val, byte_num);
}

/*
 * movsb: 1 byte
 * Moves byte at DS:SI/ESI to ES:DI/EDI.
 * 1 byte: op (A4)
 */
void movsb(Emulator *emu)
{
    uint32_t esi_val = get_register32(emu, ESI);
    uint32_t edi_val = get_register32(emu, EDI);
    uint8_t ds_esi_val = get_memory8(emu, DS, esi_val);
    set_memory8(emu, ES, edi_val, ds_esi_val);
    update_esi_edi(emu, esi_val, edi_val, 1);
    emu->eip += 1;
}

/*
 * movsd: 1 byte
 * Moves word/double word at DS:SI/ESI to ES:DI/EDI.
 * 1 byte: op (A5)
 */
void movsd(Emulator *emu)
{
    uint32_t esi_val = get_register32(emu, ESI);
    uint32_t edi_val = get_register32(emu, EDI);
    uint32_t ds_esi_val = get_memory32(emu, DS, esi_val);
    set_memory32(emu, ES, edi_val, ds_esi_val);
    update_esi_edi(emu, esi_val, edi_val, 4);
    emu->eip += 1;
}

/*
 * cmpsd: 1 byte
 * Compares byte of DS:SI/ESI value and ES:DI/EDI by subtracting in order.
 * 1 byte: op (A6)
 */
void cmpsb(Emulator *emu)
{
    uint32_t esi_val = get_register32(emu, ESI);
    uint32_t edi_val = get_register32(emu, EDI);
    uint8_t ds_esi_val = get_memory8(emu, DS, esi_val);
    uint8_t es_edi_val = get_memory8(emu, ES, edi_val);
    uint64_t result = (uint16_t)ds_esi_val - (uint16_t)es_edi_val;
    update_eflags_sub(emu, ds_esi_val, es_edi_val, result);
    update_esi_edi(emu, esi_val, edi_val, 1);
    emu->eip += 1;
}

/*
 * cmpsd: 1 byte
 * Compares dword of DS:SI/ESI value and ES:DI/EDI by subtracting in order.
 * 1 byte: op (A7)
 */
void cmpsd(Emulator *emu)
{
    uint32_t esi_val = get_register32(emu, ESI);
    uint32_t edi_val = get_register32(emu, EDI);
    uint32_t ds_esi_val = get_memory32(emu, DS, esi_val);
    uint32_t es_edi_val = get_memory32(emu, ES, edi_val);
    uint64_t result = (uint64_t)ds_esi_val - (uint64_t)es_edi_val;
    update_eflags_sub(emu, ds_esi_val, es_edi_val, result);
    update_esi_edi(emu, esi_val, edi_val, 4);
    emu->eip += 1;
}

/*
 * test al imm8: 2 bytes
 * Performs logical AND of operands and updates flags. No result store.
 * 1 byte: op (A8)
 * 1 byte: imm8 value
 */
void test_al_imm8(Emulator *emu)
{
    uint8_t imm8_val = get_code8(emu, 1);
    uint8_t al_val = get_register8(emu, AL);

    uint8_t result = al_val & imm8_val;
    update_eflags_logical_ops_8bit(emu, result);
    emu->eip += 2;
}

/*
 * test eax imm32: 5 bytes
 * Performs logical AND of operands and updates flags. No result store.
 * 1 byte: op (A9)
 * 4 bytes: imm32 value
 */
void test_eax_imm32(Emulator *emu)
{
    uint32_t eax_val = get_register32(emu, EAX);
    uint32_t imm32_val = get_code32(emu, 1);

    uint32_t result = eax_val & imm32_val;
    update_eflags_logical_ops(emu, result);
    emu->eip += 5;
}

/*
 * stosb: 1 byte
 * Stores byte value of AL to ES:DI/EDI.
 * 1 byte: op (AA)
 */
void stosb(Emulator *emu)
{
    uint8_t al_val = get_register8(emu, AL);
    uint32_t edi_val = get_register32(emu, EDI);
    set_memory8(emu, ES, edi_val, al_val);
    update_counter(emu, EDI, edi_val, 1);
    emu->eip += 1;
}

/*
 * stosb: 1 byte
 * Stores double word value of EAX to ES:DI/EDI.
 * 1 byte: op (AB)
 */
void stosd(Emulator *emu)
{
    uint32_t eax_val = get_register32(emu, EAX);
    uint32_t edi_val = get_register32(emu, EDI);
    set_memory32(emu, ES, edi_val, eax_val);
    update_counter(emu, EDI, edi_val, 4);
    emu->eip += 1;
}

/*
 * lodsb: 1 byte
 * Loads byte value DS:SI/ESI to AL.
 * 1 byte: op (AC)
 */
void lodsb(Emulator *emu)
{
    uint32_t esi_val = get_register32(emu, ESI);
    uint8_t ds_esi_val = get_memory8(emu, DS, esi_val);
    set_register8(emu, AL, ds_esi_val);
    update_counter(emu, ESI, esi_val, 1);
    emu->eip += 1;
}

/*
 * lodsd: 1 byte
 * Loads double word value DS:SI/ESI to EAX.
 * 1 byte: op (AD)
 */
void lodsd(Emulator *emu)
{
    uint32_t esi_val = get_register32(emu, ESI);
    uint32_t ds_esi_val = get_memory32(emu, DS, esi_val);
    set_register32(emu, EAX, ds_esi_val);
    update_counter(emu, ESI, esi_val, 4);
    emu->eip += 1;
}

/*
 * scasb: 1 byte
 * Compares AL and ES:DI/EDI by subtracting in order.
 * 1 byte: op (AE)
 */
void scasb(Emulator *emu)
{
    uint8_t al_val = get_register8(emu, AL);
    uint32_t edi_val = get_register32(emu, EDI);
    uint8_t es_edi_val = get_memory8(emu, ES, edi_val);
    uint64_t result = (uint16_t)al_val - (uint16_t)es_edi_val;
    update_eflags_sub(emu, al_val, es_edi_val, result);
    update_counter(emu, EDI, edi_val, 1);
    emu->eip += 1;
}

/*
 * scasd: 1 byte
 * Compares EAX and ES:DI/EDI by subtracting in order.
 * 1 byte: op (AF)
 */
void scasd(Emulator *emu)
{
    uint32_t eax_val = get_register32(emu, EAX);
    uint32_t edi_val = get_register32(emu, EDI);
    uint32_t es_edi_val = get_memory32(emu, ES, edi_val);
    uint64_t result = (uint64_t)eax_val - (uint64_t)es_edi_val;
    update_eflags_sub(emu, eax_val, es_edi_val, result);
    update_counter(emu, EDI, edi_val, 4);
    emu->eip += 1;
}