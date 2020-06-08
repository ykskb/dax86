#ifndef INSTRUCTION_DEFS_H_
#define INSTRUCTION_DEFS_H_

#include "emulator.h"
#include "modrm.h"

/* 0x00 */
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

/* 0x10 */
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

/* 0x20 */
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

/* 0x30 */
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

/* 0x40 */
void inc_r32(Emulator *emu);
void dec_r32(Emulator *emu);

/* 0x50 */
void push_r32(Emulator *emu);
void pop_r32(Emulator *emu);

/* 0x60 */
void pushad(Emulator *emu);
void popad(Emulator *emu);
void push_imm32(Emulator *emu);
void push_imm8(Emulator *emu);
void imul_r32_rm32_imm8(Emulator *emu);
void imul_r32_rm32_imm32(Emulator *emu);
void ins_m32_dx(Emulator *emu);

/* 0x70 */
void jo(Emulator *emu);
void jno(Emulator *emu);
void jc(Emulator *emu);
void jnc(Emulator *emu);
void jz(Emulator *emu);
void jnz(Emulator *emu);
void jna(Emulator *emu);
void ja(Emulator *emu);
void js(Emulator *emu);
void jns(Emulator *emu);
void jl(Emulator *emu);
void jle(Emulator *emu);
void jg(Emulator *emu);

/* 0x80 */
void code_80(Emulator *emu);
void code_81(Emulator *emu);
void code_83(Emulator *emu);
void code_83_rm16(Emulator *emu);
void test_rm8_r8(Emulator *emu);
void test_rm16_r16(Emulator *emu);
void test_rm32_r32(Emulator *emu);
void xchg_rm8_r8(Emulator *emu);
void xchg_rm32_r32(Emulator *emu);
void mov_rm8_r8(Emulator *emu);
void mov_rm32_r32(Emulator *emu);
void mov_rm16_r16(Emulator *emu);
void mov_r8_rm8(Emulator *emu);
void mov_r32_rm32(Emulator *emu);
void mov_rm32_seg(Emulator *emu);
void lea_r32_m(Emulator *emu);
void mov_seg_rm32(Emulator *emu);
void pop_rm32(Emulator *emu);

/* 0x90 */
void xchg_r32_r32(Emulator *emu);
void xchg_r16_r16(Emulator *emu);
void cwde(Emulator *emu);
void cdq(Emulator *emu);
void ptr_call(Emulator *emu);
void pushfd(Emulator *emu);
void popfd(Emulator *emu);
void sahf(Emulator *emu);
void lahf(Emulator *emu);

/* 0xA0 */
void mov_al_moffs8(Emulator *emu);
void mov_eax_moffs32(Emulator *emu);
void mov_moffs8_al(Emulator *emu);
void mov_moffs16_ax(Emulator *emu);
void mov_moffs32_eax(Emulator *emu);
void movsb(Emulator *emu);
void movsd(Emulator *emu);
void cmpsb(Emulator *emu);
void cmpsd(Emulator *emu);
void test_al_imm8(Emulator *emu);
void test_eax_imm32(Emulator *emu);
void stosb(Emulator *emu);
void stosd(Emulator *emu);
void lodsb(Emulator *emu);
void lodsd(Emulator *emu);
void scasb(Emulator *emu);
void scasd(Emulator *emu);

/* 0xB0 */
void mov_r8_imm8(Emulator *emu);
void mov_r32_imm32(Emulator *emu);
void mov_r16_imm16(Emulator *emu);

/* 0xC0 */
void code_c0(Emulator *emu);
void code_c01(Emulator *emu);
void ret(Emulator *emu);
void les(Emulator *emu);
void lds(Emulator *emu);
void mov_rm8_imm8(Emulator *emu);
void mov_rm16_imm16(Emulator *emu);
void mov_rm32_imm32(Emulator *emu);
void leave(Emulator *emu);
void ret_far(Emulator *emu);
void int_imm8(Emulator *emu);
void iret(Emulator *emu);

/* 0xD0 */
void code_d0(Emulator *emu);
void code_d1(Emulator *emu);
void code_d2(Emulator *emu);
void code_d3(Emulator *emu);
void set_al_on_c(Emulator *emu);

/* 0xE0 */
void loopnz(Emulator *emu);
void loopz(Emulator *emu);
void loop(Emulator *emu);
void jecxz(Emulator *emu);
void in_al_imm8(Emulator *emu);
void in_eax_imm8(Emulator *emu);
void out_imm8_al(Emulator *emu);
void out_imm8_eax(Emulator *emu);
void call_rel32(Emulator *emu);
void near_jump(Emulator *emu);
void ptr_jump(Emulator *emu);
void short_jump(Emulator *emu);
void in_al_dx(Emulator *emu);
void in_eax_dx(Emulator *emu);
void out_dx_al(Emulator *emu);
void out_dx_eax(Emulator *emu);

/* 0xF0 */
void cmc(Emulator *emu);
void code_f6(Emulator *emu);
void code_f7(Emulator *emu);
void clc(Emulator *emu);
void stc(Emulator *emu);
void cli(Emulator *emu);
void sti(Emulator *emu);
void cld(Emulator *emu);
void std(Emulator *emu);
void code_fe(Emulator *emu);
void code_ff(Emulator *emu);

/* 0x0F00 */
void code_0f_00(Emulator *emu);
void code_0f_01(Emulator *emu);
void push_fs(Emulator *emu);
void pop_fs(Emulator *emu);
void push_gs(Emulator *emu);
void pop_gs(Emulator *emu);

/* 0x0F20 */
void mov_r32_cr(Emulator *emu);
void mov_cr_r32(Emulator *emu);

/* 0x0F80 */
void jc32(Emulator *emu);
void jnc32(Emulator *emu);
void jz32(Emulator *emu);
void jnz32(Emulator *emu);
void jna32(Emulator *emu);
void ja32(Emulator *emu);
void js32(Emulator *emu);
void jns32(Emulator *emu);
void jge32(Emulator *emu);
void jng32(Emulator *emu);
void jg32(Emulator *emu);

/* 0x0F90 */
void sete(Emulator *emu);
void setne(Emulator *emu);

/* 0x0FB0 */
void movzx_r32_rm8(Emulator *emu);
void movzx_r32_rm16(Emulator *emu);
void movsx_r32_rm8(Emulator *emu);
void movsx_r32_rm16(Emulator *emu);

#endif