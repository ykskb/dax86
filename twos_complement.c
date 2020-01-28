#include <stdint.h>

#include "twos_complement.h"

int8_t to_int8(uint8_t raw)
{
    if (raw > 127)
    {
        return (int8_t)(raw - 0xFF) + 0xFF;
    }
    else
    {
        return (int8_t)raw;
    }
}

int16_t to_int16(uint16_t raw)
{
    if (raw > 32767)
    {
        return (int16_t)(raw - 0xFFFF) + 0xFFFF;
    }
    else
    {
        return (int16_t)raw;
    }
}

int32_t to_int32(uint32_t raw)
{
    if (raw > 2147483647)
    {
        return (int32_t)(raw - 0xFFFFFFFF) + 0xFFFFFFFF;
    }
    else
    {
        return (int32_t)raw;
    }
}

int64_t to_int64(uint64_t raw)
{
    if (raw > 9223372036854775807)
    {
        return (int64_t)(raw - 0xFFFFFFFFFFFFFFFF) + 0xFFFFFFFFFFFFFFFF;
    }
    else
    {
        return (int64_t)raw;
    }
}