#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>

#include "emulator.h"

void debug_append(uint16_t cs, uint32_t eip, uint8_t op, uint16_t ss, uint32_t esp_val);
void debug_print();
void panic_exit(Emulator *emu);
void sig_exit(Emulator *emu);
void print_emu(Emulator *emu);

#endif