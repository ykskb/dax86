/*
 * MP requires:
 * - CPUs such as ones in Intel486 or Pentium family
 * - APICs such as Intel 82489DX APIC
 * - Software-transparent cache and shared memory subsystem
 * - Software-visible components of the PC/AT platform.
 * 
 * APIC:
 * includes 2 units: local unit and IO unit which communicate on Interrupt Controller Communications (ICC) bus.
 * Local APIC units also provide interprocessor interrupts (IPIs):
 *     - allows any processor to interrupt any other processor or set of processors
 *     - INIT IPI: system startup
 *     - STARTUP IPI: system shutdown
 * Each local APIC has Local Unit ID Register.
 * Each IO APIC has IO Unit ID Register.
 * used by software for I/O interrupts and interprocessor interrupts
 * also used internally for accessingthe ICC bus.
 * NMI: Non-Maskable Interrupt (separated input)
 * Has 3 interrupt modes:
 *     - PIC Mode:
 *         Bypasses IO APIC & LAPIC (goes through 8259A-equivalent PICs)
 *         Controlled by IMCR (Interrupt Mode Control Register):
 *             Select IMCR: write 0x70 to IO port 0x22
 *             Write data: write data to IO port 0x23
 *                 0 (default): NMI and 8259 INTR connected directly to BSP
 *                 1: NMI and 825 INTR pass through APIC
 *     - Virtual Wire Mode:
 *         Uniprocessor environment for booting & running DOS shrink-wrapped software
 *         8259A-equivalent PIC -> Local APIC of BSP or
 *         8259A-equivalent PIC -> IO APIC -> Local APIC of BSP
 *     - Symmetric IO Mode:
 *         IO interrupts are generated IO APIC.
 *         8259 lines are masked or works in mixed mode.
 *             
 * 
 * System Memory for MP:
 * Standard AT memory architecture
 * 0xA0000 through 0x0FFFFF and 0xFFFE0000 through 0xFFFFFFFF are reserved for IO devices and BIOS.
 * MP creates high demands on memory performance. Secondary cache with high performance features is required.
 * (like write-backupdate policy and snooping cache-consistency protocol)
 * Memory-mapped IO devices should be mapped to top of physical memroy.
 * APIC default base memroy address: 0xFEC00000 and 0xFEE00000
 *      ________________  0xFFFF FFFF
 *     |___BIOS_Prom____| 0xFFFE 0000
 *     |________________| 0xFEF0 0000 *reserved for memory-mapped IO
 *     |___Local_APIC___| 0xFEE0 0000
 *     |________________| 0xFED0 0000 *reserved for memory-mapped IO
 *     |_____IO_APIC____| 0xFEC0 0000
 *     |  Memory-mapped |
 *     |____IO_SPACE____|
 *     |    Extended    |
 *     |_Memory_Region__| 0x0010 0000
 *     | Shadowed BIOS  | 0x000F FFFF
 *     |_____(128KB)____| 0x000F 0000
 *     | Shdwd exp BIOS | 0x000E FFFF
 *     |_____(128KB)____| 0x000E 0000
 *     | Expansion ROM  | 0x000D FFFF
 *     |_____(128KB)____| 0x000D 0000
 *     | ROM_extensions | 0x000C FFFF
 *     |_____(128KB)____| 0x000C 0000
 *     |  Video_Buffer  | 0x000F FFFF
 *     |____(384KB)_____| 0x000A 0000
 *     |      EBDA      | 0x0009 FFFF
 *     |____(128KB)_____| 0x0008 0000
 *     |  Memory Space  | 0x0007 FFFF
 *     |___(480.5KB)____| 0x0000 7E00
 *     |  OS Boot Sect. | 0x0000 7DFF
 *     |_____(512B)_____| 0x0000 7C00
 *     |  Memory Space  | 0x0000 7BFF
 *     |__(apx. 30KB)___| 0x0000 0500
 *     |       BDA      | 0x0000 04FF
 *     |_____(256B)_____| 0x0000 0400
 *     |  Real Mode IVT | 0x0000 03FF
 *     |______(1KB)_____| 0x0000 0000
 * 
 *     
 * <MP Configuration Table (2 data structure)>
 * 1. MP Floating Pointer Structure:
 *     - contains a physical address pointer to the MP configuration table & other MP feature information bytes. 
 *     - should be located at one of:
 *         a. in the first kilobyte of Extended BIOS Data Area (EBDA)
 *         b. within the last kilobyte of system base memory (if EBDA segment is undefined) like:
 *             - 639K-640K for systems with 640KB of base memory
 *             - 511K-512K for systems with 512 KB of base memory
 *         c. in the BIOS ROM address space between 0F0000h and 0FFFFFh
 * 
 *     |31________|23________|15________|7________0|
 *     |          MP Feature Bytes 2-5*            | 0x0C <- reserved for future
 *     | MP FB 1* | CheckSum | SPEC_REV |  Length* | 0x08
 *     |          Physical Address Pointer         | 0x04 <- beginning of MP Configuretion Table
 *     |__________Signature "_PM_" ("_MP_")________| 0x00
 *  
 *     * Length: length of the floating pointer structure table in paragraph (16-byte) units
 *     * MP FB 1: First byte of MP feature bytes. 0 if MP config table present, else default config value.
 *     * MP feature byte 2: 0: virtual wire mode, else IMCR is set & in PIC mode
 *
 * 2. MP Configuration Table:
 * 
 *     a. MP Configuration Table Header
 *         |31________|23________________|15________|7________0|
 *         |   Rsvd   | Ext Table Chksum |  Ext Table Length   | 0x28
 *         |         Memory-mapped address of Local APIC       | 0x24
 *         |        Entry Count          |    OEM Table Size   | 0x20
 *         |                 OEM Table Pointer                 | 0x1C
 *         |  space   |      space       |   space  |  space   | 0x18
 *         |  space   |        D         |    I     |  space   | 0x14
 *         |    D     |        O         |    R     |    P     | 0x10 <- "PROD ID" string
 *         |  space   |      space       |    D     |    I     | 0x0C
 *         |  space   |        M         |    E     |    O     | 0x08 <- "OEM ID" string
 *         | Checksum |     Spec Rev     |  Base Table Length  | 0x04
 *         |____P_____|________M_________|____C_____|____P_____| 0x00 <- Signature "PCMP" string
 *
 *     b. Base MP Configuration Table Entries
 *         - Processor Entry
 *             |31_________|23______________|15______________|7_________0|
 *             |                         Reserved                        |
 *             |                         Reserved                        |
 *             |                       Feature Flags*                    |
 *             |_______________________CPU Signature*____________________|
 *             | CPU Flags | Local APIC Ver | Local APIC ID | Entry Type*|
 *             |___|BP*|EN*|________________|_______________|_____0______|
 *
 *             *Feature Flags (from CPUID instruction):
 *                 0: contains FPU(Intel387).
 *                 1-6: Reserved.
 *                 7: MCE (Machine Check Exception)
 *                 8: CX8 (8-byte compare-and-exchange) is supported.
 *                 9: APIC is present.
 *             *CPU Signature: 0000 (family) 0000 (model) 0000 (stepping) -> Intel486 DX, Pentium etc
 *             *Entry Type: 0 for CPU
 *             *EN: 1 for usable
 *             *BP: bootstrap processor
 *         - Bus Entry
 *         - IO APIC Entry
 *             |31_____________|23______________|15______________|7_________0|
 *             |_______________Memory-mapped Address of IO APIC______________|
 *             | IO APIC Flags |    IO APIC     |  IO APIC ID   | Entry Type*|
 *             |___________|EN*|____Version_____|_______________|_____2______|
 *             *Entry Type: 2 for IO APIC entry
 *             *EN: 1 for usable.
 *         - IO Interrupt Assignment Entry
 *         - Local Interrupt Assignment Entry
 *
 *     c. Extended MP Configuration Table Entries
 *         - System Address Space Mapping Entry
 *         - Bus Hierarchy Descriptor Entry
 *         - Compatibility Bus Address Space Modifier Entry
 * 
 * <BIOS Programming Guidelines>
 * 1. system RESET sequence at power-on or reset button to put all components to initial state.
 * 2. POST (power-on self test) on all acrive processors:
 *     - initialization of components
 *     - constructing various system tables in the BIOS data area (400h-4FFh) for OS
 * (These 3 below might not be required if they are done by hardware.)
 * 3. BIOS puts APs to sleep (so only BSP would execute BIOS code.)
 * 4. Initialize APICs and other MP components if any.
 *     MP feature info bytes must identify default config type.
 *     - PIC mode (no BIOS programming needed) or
 *     - Virtual Wire Mode (BIOS needs to enable BSP first with LINT0 or LINT1 of the BSP’s local APIC.)
 * 5. Build MP configuration table about APICs and APs for OS
 *     Needs to indicate if MP configuration table is present or not on MP floating pointer table.
 *     If needed, BIOS initializes APs serially with status flags.
 * 
 * 
 * <OS Guidelines>
 * 1. BIOS initializes the floating table pointer with the configuration table’s memory address
 * 2. Search for MP floating pointer structure.
 * 2. BSP constructs the MP configuration table based on the total number of processors,buses, I/O APICs, and interrupt sources in the system.
 *    It sets all AP processor entry bits to zero.
 *    OS needs to keep BSP's APIC ID for shutdown.
 * 3. BSP enables each AP in turn by setting its AP status flag.
 *    APs follows with these steps:
 *         a. executes a CPU identification procedure
 *         b. reads its local APIC ID from the Local Unit ID Register
 *         c. uses this information to complete its entry in the MP configuration table
 *         d. AP clears its status flag before exiting BIOS
 *         e. AP executes HLT instruction (Real mode with interrupts disabled at this point.)
 * 4. BSP continues filling the rest of MP configuration entries.
 * 
 * EBDA (Extended BIOS Data Area):
 * - Always immediately below 0xA0000 in memory
 * - Guaranteed to be at most 128 KiB in size
 * - Older computers typically uses 1 KiB from 0x9FC00 - 0x9FFFF
 * - Size be found by INT 0x12 or 0x40E in BDA -> Location of bottom of EBDA
 * 
 * example:
 * EBDA Adress: 0x0009 FC000
 * Look for floating pointer 0xF0000 - 0x100000: 0xC00F 6A90
 * MP Floating Pointer: 0xC00F 6A90
 * MP Configuration Table Header: 0xC00F 6AA0
 */

#include "emulator.h"
#include "emulator_functions.h"
#include "lapic.h"
#include "ioapic.h"

#define BDA_ADDR_EBDA_LOC 0x40E
#define EBDA_LOC 0x9FC00
/* Size: 0x2C + EBDA_LOC (0x9FC10?) */
#define MP_CONF_TBL_HD_BASE 0x9FC2C
#define MP_CONF_TBL_ENT_BASE (0x9FC2C + 44)

static void create_bda(Emulator *emu)
{
    /* set the EBDA base address to conventional 0x9FC00 >> 4 = 0x9FC0 */
    _set_memory16(emu, BDA_ADDR_EBDA_LOC, (EBDA_LOC >> 4));
}

static void create_ebda(Emulator *emu)
{
    /* MP Floating Pointer 
     * |31________|23________|15________|7________0|
     * |          MP Feature Bytes 2-5*            | 0x0C
     * | MP FB 1* | CheckSum | SPEC_REV |  Length* | 0x08
     * |          Physical Address Pointer         | 0x04
     * |__________Signature "_PM_" ("_MP_")________| 0x00
     */
    _set_memory32(emu, EBDA_LOC, (uint32_t)0x5f504d5f);
    _set_memory32(emu, EBDA_LOC + 4, MP_CONF_TBL_HD_BASE);
    _set_memory32(emu, EBDA_LOC + 8, 0x006f0401);
    _set_memory32(emu, EBDA_LOC + 12, 0x0);
}

static void create_mp_conf_tbl_header(Emulator *emu)
{
    /*
     * MP Configuration Table Header
     * |31________|23________________|15________|7________0|
     * |   Rsvd   | Ext Table Chksum |  Ext Table Length   | 0x28
     * |         Memory-mapped address of Local APIC       | 0x24
     * |        Entry Count          |    OEM Table Size   | 0x20
     * |                 OEM Table Pointer                 | 0x1C
     * |  space   |      space       |   space  |  space   | 0x18
     * |  space   |        D         |    I     |  space   | 0x14
     * |    D     |        O         |    R     |    P     | 0x10
     * |  space   |      space       |    D     |    I     | 0x0C
     * |  space   |        M         |    E     |    O     | 0x08
     * | Checksum |     Spec Rev     |  Base Table Length  | 0x04
     * |____P_____|________M_________|____C_____|____P_____| 0x00
     */
    _set_memory32(emu, MP_CONF_TBL_HD_BASE, (int32_t)0x504d4350);
    /* 
     * 1 CPU entry (20 bytes) + 1 IOAPIC entry (8 bytes) = 28 bytes
     * "Including the header": 44 bytes = 72 bytes (0x48)
     * Version 01 indicates 1.1
     */
    _set_memory32(emu, MP_CONF_TBL_HD_BASE + 0x4, 0xe9010048);
    _set_memory32(emu, MP_CONF_TBL_HD_BASE + 0x24, LAPIC_DEFAULT_BASE);
}

static void create_mp_conf_tbl_entries(Emulator *emu)
{
    /* |31_________|23______________|15______________|7_________0|
     * |                         Reserved                        |
     * |                         Reserved                        |
     * |                       Feature Flags                     |
     * |_______________________CPU Signature_____________________|
     * | CPU Flags | Local APIC Ver | Local APIC ID | Entry Type |
     * |_____|BP|EN|________________|_______________|_____0______|
     */
    _set_memory32(emu, MP_CONF_TBL_ENT_BASE, 0x0);
    /*
     * |31_____________|23______________|15______________|7_________0|
     * |_______________Memory-mapped Address of IO APIC______________|
     * | IO APIC Flags |    IO APIC     |  IO APIC ID   | Entry Type |
     * |____________|EN|____Version_____|_______________|_____2______|
     */
    _set_memory32(emu, MP_CONF_TBL_ENT_BASE + 0x14, 0x2);
    _set_memory32(emu, MP_CONF_TBL_ENT_BASE + 0x18, IOAPIC_DEFAULT_BASE);
}

void set_mp_config(Emulator *emu)
{
    create_bda(emu);
    create_ebda(emu);
    create_mp_conf_tbl_header(emu);
    create_mp_conf_tbl_entries(emu);
}