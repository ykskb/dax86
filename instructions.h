#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#include "emulator.h"

void init_instructions(void);

typedef void instruction_func_t(Emulator *); // instruction_func_t = void func(Emulator*)

extern instruction_func_t *instructions[256];
extern uint8_t quiet;

#endif