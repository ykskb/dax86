#ifndef IO_H_
#define IO_H_

#include <stdint.h>

uint8_t io_in8(uint16_t address);
uint32_t io_in32(uint16_t address);

void io_out8(uint16_t address, uint8_t value);
void io_out32(uint16_t address, uint32_t value);

#endif
