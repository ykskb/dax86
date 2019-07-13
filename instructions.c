#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "instructions.h"
#include "emulator_functions.h"
#include "modrm.h"

/*
 * jmp (short): 2 bytes
 * Jumps with 8-bit signed offset.
 * 1 byte: op (EB)
 * 1 byte: offset from eip (8 bit signed) -127 ~ 127
 */
static void short_jump(Emulator *emu)
{
    int8_t offset = get_sign_code8(emu, 1);
    emu->eip += (offset + 2);
}

/*
 * jmp (near): 5 bytes
 * Jumps with 32-bit signed offset.
 * 1 byte: op (E9)
 * 4 byte: offset from eip (32 bit signed)
 */
static void near_jump(Emulator *emu)
{
    int32_t diff = get_sign_code32(emu, 1);
    emu->eip += (diff + 5);
}

/*
 * mov r32 imm32: 5 bytes
 * Copies imm value to register specified in op code (r32: 32 bit register).
 * 1 byte: op (B8) + reg (3bits)
 * 4 bytes: value (32 bit unsigned)
 */
static void mov_r32_imm32(Emulator *emu)
{
    uint8_t reg = get_code8(emu, 0) - 0xB8;
    uint32_t value = get_code32(emu, 1);
    emu->registers[reg] = value; // maps to Register Enum
    emu->eip += 5;
}

/*
 * mov rm32 imm32: 5 bytes
 * Copies imm value to register or memory specified by ModR/M (rm32).
 * 1 byte: op (C7)
 * 1 byte: ModR/M
 * 4 bytes: value (32)
 */
static void mov_rm32_imm32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm;
    parse_moderm(emu, &modrm);
    uint32_t value = get_code32(emu, 0);
    emu->eip += 4;
    set_rm32(emu, &modrm, value);
}

/*
 * mov rm32 r32: 2 bytes
 * Copies value of register specified by REG to ModR/M.
 * 1 byte: op (89)
 * 1 byte: ModR/M
 */
static void mov_rm32_r32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm;
    parse_moderm(emu, &modrm);
    /* Reads value from register specified by REG bits. */
    uint32_t r32 = get_r32(emu, &modrm);
    /* Sets value on memory/register specified by ModR/M. */
    set_rm32(emu, &modrm, r32);
}

/*
 * mov r32 rm32: 2 bytes
 * Copies value of ModR/M to REG.
 * 1 byte: op (8B)
 * 1 byte: ModR/M
 */
static void mov_r32_rm32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm;
    parse_moderm(emu, &modrm);
    /* Reads value from memory/register specified by ModR/M. */
    uint32_t rm32 = get_rm32(emu, &modrm);
    /* Sets value on register specified by REG bits. */
    set_r32(emu, &modrm, rm32);
}

/*
 * add rm32 r32: 2 bytes
 * Adds value of REG to ModR/M.
 * 1 byte: (01)
 * 1 byte: ModR/M
 */
static void add_rm32_r32(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm;
    parse_moderm(emu, &modrm);
    uint32_t r32 = get_r32(emu, &modrm);
    uint32_t rm32 = get_rm32(emu, &modrm);
    set_rm32(emu, &modrm, r32 + rm32);
}

/*
 * sub rm32 imm8: 3 bytes
 * Subtracts imm8 from RM32. Op code 83 and ModR/M op code: 101 execute this.
 * 1 byte: shared op (83)
 * 1 byte: ModR/M
 * 1 byte: imm8 to subtract
 */
static void sub_rm32_r32(Emulator *emu, ModRM *modrm)
{
    uint32_t rm32 = get_rm32(emu, modrm);
    uint32_t imm8 = (int32_t)get_sign_code8(emu, 0);
    emu->eip += 1;
    set_rm32(emu, modrm, rm32 - imm8);
}

static void code_83(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm;
    parse_moderm(emu, &modrm);

    switch (modrm.opcode)
    {
    case 5:
        sub_rm32_r32(emu, &modrm);
        break;

    default:
        printf("Not implemented: Op: 83 with ModR/M Op: %d\n", modrm.opcode);
        exit(1);
    }
}

/*
 * inc rm32: 2 bytes
 * Increments ModR/M. Op code 83 and ModR/M op code: 000 execute this.
 * 1 byte: shared op (FF)
 */
static void inc_rm32(Emulator *emu, ModRM *modrm)
{
    uint32_t value = get_rm32(emu, modrm);
    set_rm32(emu, modrm, value + 1);
}

static void code_ff(Emulator *emu)
{
    emu->eip += 1;
    ModRM modrm;
    parse_moderm(emu, &modrm);

    switch (modrm.opcode)
    {
    case 0:
        inc_rm32(emu, &modrm);
        break;

    default:
        printf("Not implemented: Op: FF with ModR/M Op: %d\n", modrm.opcode);
        exit(1);
    }
}

/*
 * push r32: 1 bytes
 * Pushes 32-bit value into memory from specified register.
 * 1 byte: op (50) + reg
 */
static void push_r32(Emulator *emu)
{
    u_int8_t reg = get_code8(emu, 0) - 0x50;
    push32(emu, get_register32(emu, reg));
    emu->eip += 1;
}

/*
 * pop r32: 1 bytes
 * Pops 32-bit value into specified register from memory.
 * 1 byte: op (58) + reg
 */
static void pop_r32(Emulator *emu)
{
    u_int8_t reg = get_code8(emu, 0) - 0x58;
    set_register32(emu, reg, pop32(emu));
    emu->eip += 1;
}

/*
 * call rel32: 5 bytes
 * Jumps by 32-bit number relatively from next address.
 * 1 byte: op (E8)
 * 4 bytes: relative number to jump.
 */
static void call_rel32(Emulator *emu)
{
    /* Offset value should be after the op code of 1 byte. */
    int32_t offset = get_sign_code32(emu, 1);
    /* Pushes the address after this call instruction. */
    push32(emu, emu->eip + 5);
    /* Adds the offset to EIP. */
    emu->eip += (offset + 5);
}

/* 
 * ret: 1 byte
 * Jumps to the address pushed by call (address after call instruction).
 * 1 byte: op (C3)
 */
static void ret(Emulator *emu)
{
    emu->eip = pop32(emu);
}

instruction_func_t *instructions[256];

void init_instructions(void)
{
    int i;
    memset(instructions, 0, sizeof(instructions));

    /* Last 3 digits indicates 8 different registers in op code. */
    for (i = 0; i < 8; i++)
    {
        instructions[0x50 + i] = push_r32;
    }

    for (i = 0; i < 8; i++)
    {
        instructions[0x58 + i] = pop_r32;
    }

    /* Why 0xB8 ~ 0xBF: op code includes 8 registers in 1 byte. */
    for (i = 0; i < 8; i++)
    {
        instructions[0xB8 + i] = mov_r32_imm32;
    }
    instructions[0x01] = add_rm32_r32;
    instructions[0x83] = code_83;
    instructions[0x89] = mov_rm32_r32;
    instructions[0x8B] = mov_r32_rm32;
    instructions[0xC7] = mov_rm32_imm32;
    instructions[0xE9] = near_jump;
    instructions[0xEB] = short_jump;
    instructions[0xFF] = code_ff;

    instructions[0xC3] = ret;
    instructions[0xE8] = call_rel32;
}