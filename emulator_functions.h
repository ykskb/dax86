#ifndef EMULATOR_FUNCTIONS_H_
#define EMULATOR_FUNCTIONS_H_

#include <stdint.h>

#include "emulator.h"

uint32_t get_code8(Emulator *emu, int index);

int32_t get_sign_code8(Emulator *emu, int index);

uint32_t get_code32(Emulator *emu, int index);

int32_t get_sign_code32(Emulator *emu, int index);

#endif
