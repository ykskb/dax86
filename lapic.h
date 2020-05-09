#ifndef LAPIC_H_
#define LAPIC_H_

/*
 * <Local APIC Registers (0x0 to 0x3F0)>
 * The default base address 0xFEE00000.
 * (0x0 - 0x10: Reserved)
 * 0x20: RW: LAPIC ID Register
 * 0x30: R : LAPIC Version Register
 * (0x40 - 0x70: Reserved)
 * 0x80: RW: TPR (Task Priority Register)
 * 0x90: R : APR (Arbitration Priority Register)
 * 0xA0: R : PPR (Processor Pritority Register)
 * 0xB0: W : EOI Register
 * 0xC0: R : RRD (Remote Read Register)
 * 0xD0: RW: Logical Destination Register
 * 0xE0: RW: Destination Format Register
 * 0xF0: RW: Spurious Interrupt Vector Register
 * 0x100 - 0x170: R: ISR (In-service Register)
 * 0x180 - 0x1F0: R: TMR (Trigger Mode Register)
 * 0x200 - 0x270: R: IRR (Interrupt Request Register)
 * 0x280: R: Error Status Register
 * (0x290 - 0x2E0: Reserved)
 * 0x2F0: RW: CMCI (LVT Corrected Machine Check Interrupt Register)
 * 0x300 - 0x310: RW: ICR (Interrupt Command Register) *described below
 * 0x320: RW: LVT Timer Register *described below
 * 0x330: RW: LVT Thermal Sensor Register
 * 0x340: RW: LVT Performance Monitoring Counters Register
 * 0x350: RW: LVT LINT0 Register
 * 0x360: RW: LVT LINT1 Register
 * 0x370: RW: LVT Error Register
 * 0x380: RW: Initial Count Register (for Timer)
 * 0x390: R : Current Count Register (for Timer)
 * (0x3A0 - 0x3D0: Reserved)
 * 0x3E0: RW: Devide Configuration Register (for Timer)
 * (0x3F0: Reserved)
 * 
 * <LVT (Local Vector Table) Registers>
 * 0 - 7: Vector Number
 * 8 - 11: 100b if NMI (for timer)
 * 12: Set if interrupt pending
 * 13: Polarity. Set for low trigger.
 * 14: Remote IRR
 * 15: Trigger Mode. Set for level trigger.
 * 16: Set for mask.
 * 17 - 31: Reserved
 * 
 * <Interrupt Command Register>
 * 0 - 7: Vector Number / Start page number of SIPI (Start IPI)
 * 8 - 10: Destination Mode (0: normal, 1: lowest, 2: SMI, 4: NMI, 5: INIT, 6: SIPI)
 * 11: Destination Mode (0: physical destination, 1: logical destination)
 * 12: Delivery Status (0: accepted, 1: pending)
 * (13: Reserved)
 * 14: 0: INIT level de-assert
 * 15: 1: INIT level de-assert
 * 18 - 19: Destination type (0: 0x310 will be ignored, 1: to self, 2: to all processors, 3: to all but self)
 * (20: 31: Reserved)
 */

#include <stdint.h>
#include <pthread.h>

#include "emulator.h"

#define LAPIC_DEFAULT_BASE 0xFEE00000

#define TPR 0x0080        // Task Priority
#define EOI 0x00B0        // EOI
#define SVR 0x00F0        // Spurious Interrupt Vector
#define TIMER 0x0320      // Local Vector Table 0 (TIMER)
#define TICR 0x0380       // Timer Initial Count
#define LINT0 0x0350      // Local Vector Table 1 (LINT0)
#define LINT1 0x0360      // Local Vector Table 2 (LINT1)
#define ERROR 0x0370      // Local Vector Table 3 (ERROR)
#define MASKED 0x00010000 // Interrupt masked

LAPIC *create_lapic(Emulator *emu);

void lapic_send_intr(LAPIC *lapic);
void lapic_write_to_irr(LAPIC *lapic, uint8_t irq);

void lapic_write_reg(LAPIC *lapic, uint32_t addr, uint32_t val);
uint32_t lapic_read_reg(LAPIC *lapic, uint32_t addr);

void dump_lapic(LAPIC *lapic);

#endif