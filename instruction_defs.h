#ifndef INSTRUCTION_DEFS_H_
#define INSTRUCTION_DEFS_H_

#include "emulator.h"
#include "modrm.h"

/* 0x0 */
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

/* 0x1 */
void adc_rm8_r8(Emulator *emu);
void adc_rm32_r32(Emulator *emu);
void adc_r8_rm8(Emulator *emu);
void adc_r32_rm32(Emulator *emu);
void adc_al_imm8(Emulator *emu);
void adc_eax_imm32(Emulator *emu);
void push_ss(Emulator *emu);
void pop_ss(Emulator *emu);
void sbb_rm8_r8(Emulator *emu);
void sbb_rm32_r32(Emulator *emu);
void sbb_r8_rm8(Emulator *emu);
void sbb_r32_rm32(Emulator *emu);
void sbb_al_imm8(Emulator *emu);
void sbb_eax_imm32(Emulator *emu);
void push_ds(Emulator *emu);
void pop_ds(Emulator *emu);

/* 0x2 */
void and_rm8_r8(Emulator *emu);
void and_rm32_r32(Emulator *emu);
void and_r8_rm8(Emulator *emu);
void and_r32_rm32(Emulator *emu);
void and_al_imm8(Emulator *emu);
void and_eax_imm32(Emulator *emu);
/* 26 ES override prefix */
/* 27 DAA Adjusts two packed BCD values. */
void sub_rm8_r8(Emulator *emu);
void sub_rm32_r32(Emulator *emu);
void sub_r8_rm8(Emulator *emu);
void sub_r32_rm32(Emulator *emu);
void sub_al_imm8(Emulator *emu);
void sub_eax_imm32(Emulator *emu);
/* 2E CS override prefix */

/* 0x3 */
void xor_rm8_r8(Emulator *emu);
void xor_rm32_r32(Emulator *emu);
void xor_r8_rm8(Emulator *emu);
void xor_r32_rm32(Emulator *emu);
void xor_al_imm8(Emulator *emu);
void xor_eax_imm32(Emulator *emu);
void cmp_rm8_r8(Emulator *emu);
void cmp_rm32_r32(Emulator *emu);
void cmp_r8_rm8(Emulator *emu);
void cmp_r32_rm32(Emulator *emu);
void cmp_al_imm8(Emulator *emu);
void cmp_eax_imm32(Emulator *emu);

/* 0x4 */
void inc_r32(Emulator *emu);
void dec_r32(Emulator *emu);

/* 0x5 */
void push_r32(Emulator *emu);
void pop_r32(Emulator *emu);

/* 0x6 */
void pushad(Emulator *emu);
void popad(Emulator *emu);
void push_imm32(Emulator *emu);
void push_imm8(Emulator *emu);

/* 0x7 */
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

/* 0x8 */
void code_80(Emulator *emu);
void code_81(Emulator *emu);
void code_83(Emulator *emu);
void test_rm8_r8(Emulator *emu);
void test_rm32_r32(Emulator *emu);
void xchg_rm8_r8(Emulator *emu);
void xchg_rm32_r32(Emulator *emu);
void mov_rm8_r8(Emulator *emu);
void mov_rm32_r32(Emulator *emu);
void mov_r8_rm8(Emulator *emu);
void mov_r32_rm32(Emulator *emu);
void mov_rm32_seg(Emulator *emu);
void lea_r32_m(Emulator *emu);
void mov_seg_rm32(Emulator *emu);
void pop_rm32(Emulator *emu);

/* 0xB */
void mov_r8_imm8(Emulator *emu);
void mov_r32_imm32(Emulator *emu);

/* 0xC */
void ret(Emulator *emu);
void mov_rm8_imm8(Emulator *emu);
void mov_rm32_imm32(Emulator *emu);
void leave(Emulator *emu);

/* 0xE */
void call_rel32(Emulator *emu);
void near_jump(Emulator *emu);
void short_jump(Emulator *emu);
void in_al_dx(Emulator *emu);
void out_dx_al(Emulator *emu);

/* 0xF */
void cli(Emulator *emu);
void sti(Emulator *emu);
void cld(Emulator *emu);
void std(Emulator *emu);
void inc_rm32(Emulator *emu, ModRM *modrm);
void code_ff(Emulator *emu);

#endif