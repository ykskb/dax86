#ifndef IOAPIC_H_
#define IOAPIC_H_

#include <stdint.h>

#include "lapic.h"

/*
 * <Memory-mapped Registers>
 * 
 * - IOREGSEL
 *   I/O REGISTER SELECT REGISTER
 *   0xFEC0 xy00 (xy=See APICBASE Register in the PIIX3)
 *   |31_____________8|7____________________0|
 *   |____Reserved____|_APIC_Register_Offset_|
 * 
 * - IOWIN
 *   I/O WINDOW REGISTER
 *   0xFEC0 xy10 (xy=See APICBASE Register in PIIX3)
 *   |31__________________________________0|
 *   |_________APIC_Register_Data__________|
 * 
 * <Registers>
 * 
 * - IOAPICID
 *   Offset: 0x0
 *   |31________28|27___24|23_____________0|
 *   |__Reserved__|APIC_ID|____Reserved____|
 * 
 * - IOAPICVER
 *   Offset: 0x1
 *   |31_24|23___________________16|15_8|7_______0|
 *   |Rsvd_|_Max_Redirection_Entry_|Rsvd|_Version_|
 * 
 * - IOAPICARB: arbitration priority of for bus ownershipt (rotating priority scheme: 15 wins)
 *   Offset: 0x2
 *   |31________28|27__________24|23_____________0|
 *   |__Reserved__|Arbitration_ID|____Reserved____|
 * 
 * - IOREDTBL[23:0] 64-bit entries (2 x 32-bit registers)
 *   Offset:
 *   0x10-0x11 (IOREDTBL0)
 *   0x12-0x13 (IOREDTBL1)
 *   ...
 *   0x3E-0x3F (IOREDTBL23)
 * 
 *   |63___________56|55________________________32|
 *   |__Destination__|_________Reserved___________|
 * 
 *   Destination Field:
 *     APIC ID in physical destination mode *xv6 uses this with APIC ID (cpunum)
 *     Set of processors in logical mode
 * 
 *   |31______17|16__|15_|14_|13_|12__|11__|10_8|7____________0|
 *   | Reserved ||Msk|Trg|IRR|Pol|DelS|DesM|DelM|__Int_Vector__|
 * 
 *   Msk = Interrupt mask: 1: masked
 *   Trg = Trigger mode: 1: level sensitive / 0: edge sensitive
 *   IRR = Remote IRR: APIC Ack in level-sensitive mode: 1: ack in local APIC -> 0: EOI from local APIC
 *   Pol = Interrupt Pin Polarity: 0: high active / 1: low active
 *   DelS = Delivery Status: 0: idle / 1: pending to send
 *   DesM = Destination Mode:
 *     0: physical mode (Destination APIC is by ID)
 *     1: logical mode (Destination Format Register and Logical Destination Register in each Local APIC)
 *   DelM = Delivery Mode:
 *     0: Fixed (Signal on INTR to all destinations. Both edge/level.)
 *     1: Lowest Priority (To processor with lowest priority. Both edge/level.)
 *     2: SMI (System management interrrupt. Vector is ignored.)
 *     3: Reserved
 *     4: NMI (NMI to all destinations. Vector is ignored. Only edge.)
 *     5: INIT (INIT to all destinations. Only edge.)
 *     6: Reserved
 *     7: ExtINT (INTR signal from external connection like 8259A to all destinations. Only edge.)
 *   Int Vector:  8-bit field containing the interrupt (10h to FEh).
 */

typedef struct
{
    uint32_t id_register;
    uint32_t ver_register;
    uint32_t arb_register;
    uint32_t redirect_tbl[48];
    LAPIC *lapic[8];
} IOAPIC;

IOAPIC *create_ioapic();

void add_lapic(IOAPIC *ioapic, uint8_t index, LAPIC *lapic);
void ioapic_int_to_lapic(IOAPIC *ioapic, uint8_t irq);

#endif