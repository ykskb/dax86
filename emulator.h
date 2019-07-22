#ifndef EMULATOR_H_
#define EMULATOR_H_

#include <stdint.h>

/*
In order of REG of ModR/M
EAX: 000, ECX: 001 ... EDI: 111
*/
enum Register
{
    EAX,
    ECX,
    EDX,
    EBX,
    ESP,
    EBP,
    ESI,
    EDI,
    REGISTERS_COUNT
};

typedef struct
{
    uint32_t registers[REGISTERS_COUNT];
    uint32_t eflags;
    uint8_t *memory;
    uint32_t eip;
} Emulator;

#endif