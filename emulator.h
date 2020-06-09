#ifndef EMULATOR_H_
#define EMULATOR_H_

#include <stdint.h>
#include <pthread.h>

#include "disk.h"

/* Memory size: 512MB */
#define MEMORY_SIZE (1024 * 1024 * 512)

#define APIC_REGISTERS_SIZE 64

/*
 * In order of REG of ModR/M
 * EAX: 000, ECX: 001 ... EDI: 111
 * 
 * 32  | 16 | 8  | 8
 * EAX | AX | AH | AL
 * EBX | BX | BH | BL
 * ECX | CX | CH | CL
 * EDX | DX | DH | DL
 * ESI
 * EDI
 * ESP
 * EBP
 */
enum Register
{
    EAX,
    ECX,
    EDX,
    EBX,
    ESP,
    EBP,
    ESI,
    EDI,
    REGISTERS_COUNT,
    AL = EAX,
    CL = ECX,
    DL = EDX,
    BL = EBX,
    AH = AL + 4,
    CH = CL + 4,
    DH = DL + 4,
    BH = BL + 4
};

enum SegmentRegister
{
    ES,
    CS,
    SS,
    DS,
    FS,
    GS,
    SEGMENT_REGISTERS_COUNT
};

enum ControlRegister
{
    CR0,
    CR1,
    CR2,
    CR3,
    CR4,
    CONTROL_REGISTER_COUNT
};

/* GDTR:
 * |0      15|16        47|
 * |  LIMIT  |    BASE    |
 * |_________|____________|
 */
typedef struct
{
    uint16_t limit;
    uint32_t base;
} Gdtr;

typedef struct
{
    uint16_t limit;
    uint32_t base;
} Idtr;

enum Exception
{
    NO_ERR,
    E_DE,
    E_DB,
    E_BP,
    E_OF,
    E_BR,
    E_UD,
    E_NM,
    E_DF,
    E_TS,
    E_NP,
    E_SS,
    E_GP,
    E_PF,
    E_MF,
    E_AC,
    E_MC,
    E_XF,
    E_VE,
    E_SX
};

typedef struct LAPIC LAPIC;
typedef struct Emulator Emulator;

/*
 * FLAGS:
 * | n-th bit                                      |
 * | 0   | 1   | 2   | 3   | 4   | 5   | 6   | 7   |
 * | CF  | Rsv | PF  | Rsv | AF  | Rsv | ZF  | SF  |
 * 
 * | 8   | 9   | 10  | 11  | 12  | 13  | 14  | 15  |
 * | TF  | IF  | DF  | OF  |   IOPL    | NT* | Rsv |
 * 
 * EFLAGS:
 * | 16  | 17  | 18  | 19  | 20  | 21  | 22  | 23  |
 * | RF  | VM  | AC  | VIF | VIP | ID  |    Rsv    |
 * 
 * | 24  | 25  | 26  | 27  | 28  | 29  | 30  | 31  |
 * |                     Rsvd                      |
 * 
 * NT: nested task flag
 * 
 * RFLAGS:
 * Rsv...
 */
struct Emulator
{
    /* Registers */
    uint32_t eflags;
    uint32_t registers[REGISTERS_COUNT];
    uint16_t segment_registers[SEGMENT_REGISTERS_COUNT];
    uint32_t control_registers[CONTROL_REGISTER_COUNT];
    uint32_t eip;
    Gdtr gdtr;
    Idtr idtr;
    uint16_t tr;
    uint8_t int_r;
    /* Devices */
    struct LAPIC *lapic;
    uint8_t *memory;
    Disk *disk;
    /* Utility */
    uint8_t is_pe;
    uint8_t is_pg;
    uint8_t int_enabled;
    uint8_t exception;
};

struct LAPIC
{
    uint32_t registers[APIC_REGISTERS_SIZE];
    Emulator *emu;

    /* convenience properties */
    uint8_t unit_enabled;
    uint8_t int_enabled;
    uint8_t irr[32];
    uint8_t isr[32];
    uint8_t isr_index;
    pthread_mutex_t lock;
    pthread_t *timer_thread;
};

Emulator *create_emu(uint8_t *memory, uint32_t eip, uint32_t esp);
void destroy_emu(Emulator *emu);

void attach_disk(Emulator *emu, Disk *disk);
void load_boot_sector(Emulator *emu);

void dump_registers(Emulator *emu);
void dump_memory(Emulator *emu, int from, int len);
void dump_eflags(Emulator *emu);
/*
void dump_input(Emulator *emu);
*/

#endif