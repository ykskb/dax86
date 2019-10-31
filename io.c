#include "io.h"
#include <stdint.h>
#include <stdio.h>
#include "emulator.h"

uint8_t io_in8(uint16_t address)
{
    switch (address)
    {
    case 0x03f8:
        /* 
         * Convenience Implementation:
         * Actual IO checks 0x03fd (status port) and read 
         * from 0x03f8 (data port) if data is there.
         * Also while loop below clears buffer if needed.
         * while ((ch = getchar()) != '\n' && ch != EOF) continue;
         */
        return getchar();
        break;
    default:
        return 0;
    }
}

uint32_t io_in32(uint16_t address)
{
    switch (address)
    {
    case 0x03f8:
        return getchar();
        break;
    default:
        return 0;
    }
}

void io_out8(uint16_t address, uint8_t value)
{
    switch (address)
    {
    case 0x03f8:
        putchar(value);
        break;
    }
}

void io_out32(uint16_t address, uint32_t value)
{
    switch (address)
    {
    case 0x03f8:
        putchar(value);
        break;
    }
}
