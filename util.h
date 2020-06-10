#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>

#include "emulator.h"

typedef struct
{
    int verbose;
    int test;
} Config;

extern Config config;

void init_config(int verbose, int test);
void debug_append(uint16_t cs, uint32_t eip, uint8_t op, uint16_t ss, uint32_t esp_val);
void debug_print();
void print_emu(Emulator *emu);

void add_canon_echo();
void remove_canon_echo();

void panic() __attribute__((noreturn));
void panic_exit(Emulator *emu) __attribute__((noreturn));
void sig_exit(Emulator *emu) __attribute__((noreturn));

#endif