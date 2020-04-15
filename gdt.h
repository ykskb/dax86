#ifndef GDT_H_
#define GDT_H_

#include "modrm.h"

#define CR0_PE (1)

void lgdt(Emulator *emu, ModRM *modrm);

void check_protected_mode_entry(Emulator *emu);

uint32_t get_linear_addr(Emulator *emu, uint16_t seg_val, uint32_t offset, uint8_t write, uint8_t exec);

#endif