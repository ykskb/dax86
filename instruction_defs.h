#ifndef INSTRUCTION_DEFS_H_
#define INSTRUCTION_DEFS_H_

#include "emulator.h"
#include "modrm.h"

void add_rm8_r8(Emulator *emu);
void add_rm32_r32(Emulator *emu);
void add_r8_rm8(Emulator *emu);
void add_r32_rm32(Emulator *emu);
void add_al_imm8(Emulator *emu);
void add_eax_imm32(Emulator *emu);
void push_es(Emulator *emu);
void pop_es(Emulator *emu);
void or_rm8_r8(Emulator *emu);
void or_rm32_r32(Emulator *emu);
void or_r8_rm8(Emulator *emu);
void or_r32_rm32(Emulator *emu);
void or_al_imm8(Emulator *emu);
void or_eax_imm32(Emulator *emu);
void push_cs(Emulator *emu);

void push_ss(Emulator *emu);
void pop_ss(Emulator *emu);
void push_ds(Emulator *emu);
void pop_ds(Emulator *emu);

void cmp_al_imm8(Emulator *emu);
void cmp_eax_imm32(Emulator *emu);
void cmp_r32_rm32(Emulator *emu);

void inc_r32(Emulator *emu);

void push_r32(Emulator *emu);
void pop_r32(Emulator *emu);

void push_imm32(Emulator *emu);
void push_imm8(Emulator *emu);

void jo(Emulator *emu);
void jno(Emulator *emu);
void jc(Emulator *emu);
void jnc(Emulator *emu);
void jz(Emulator *emu);
void jnz(Emulator *emu);
void js(Emulator *emu);
void jns(Emulator *emu);
void jl(Emulator *emu);
void jle(Emulator *emu);

void code_83(Emulator *emu);
void mov_rm8_r8(Emulator *emu);
void mov_rm32_r32(Emulator *emu);
void mov_r8_rm8(Emulator *emu);
void mov_r32_rm32(Emulator *emu);
void mov_seg_rm16(Emulator *emu);

void mov_r8_imm8(Emulator *emu);
void mov_r32_imm32(Emulator *emu);

void ret(Emulator *emu);
void mov_rm32_imm32(Emulator *emu);
void leave(Emulator *emu);

void call_rel32(Emulator *emu);
void near_jump(Emulator *emu);
void short_jump(Emulator *emu);
void in_al_dx(Emulator *emu);
void out_dx_al(Emulator *emu);

void cli(Emulator *emu);
void sti(Emulator *emu);
void cld(Emulator *emu);
void std(Emulator *emu);
void inc_rm32(Emulator *emu, ModRM *modrm);
void code_ff(Emulator *emu);

#endif