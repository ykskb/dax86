#ifndef PAGING_H_
#define PAGING_H_

#include "emulator.h"

#define CR0_PG (1 << 31)
#define CR4_PSE (1 << 4)

#define PDE_PS (1 << 7)

void check_paging(Emulator *emu);

uint32_t get_phys_addr(Emulator *emu, uint32_t linear_addr, uint8_t write, uint8_t exec);

#endif