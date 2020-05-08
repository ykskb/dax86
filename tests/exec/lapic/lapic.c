#define ID (0x0020 / 4)    // ID
#define TPR (0x0080 / 4)   // Task Priority
#define ICRLO (0x0300 / 4) // Interrupt Command
#define INIT 0x00000500    // INIT/RESET
#define STARTUP 0x00000600 // Startup IPI
#define DELIVS 0x00001000  // Delivery status
#define ASSERT 0x00004000  // Assert interrupt (vs deassert)
#define DEASSERT 0x00000000
#define LEVEL 0x00008000 // Level triggered
#define BCAST 0x00080000 // Send to all APICs, including self.
#define BUSY 0x00001000
#define FIXED 0x00000000
#define ICRHI (0x0310 / 4)  // Interrupt Command [63:32]
#define TIMER (0x0320 / 4)  // Local Vector Table 0 (TIMER)
#define PCINT (0x0340 / 4)  // Performance Counter LVT
#define LINT0 (0x0350 / 4)  // Local Vector Table 1 (LINT0)
#define LINT1 (0x0360 / 4)  // Local Vector Table 2 (LINT1)
#define ERROR (0x0370 / 4)  // Local Vector Table 3 (ERROR)
#define TICR (0x0380 / 4)   // Timer Initial Count
#define MASKED 0x00010000   // Interrupt masked
#define PERIODIC 0x00020000 // Periodic

#define T_IRQ0 32 // IRQ 0 corresponds to int T_IRQ
#define IRQ_TIMER 0

#define LAPIC_DEFAULT_BASE 0xFEE00000

// volatile unsigned int *lapic = (unsigned int *)LAPIC_DEFAULT_BASE; // Initialized in mp.c

static unsigned int
lapicw(int index, int value)
{
    volatile unsigned int *lapic = (unsigned int *)LAPIC_DEFAULT_BASE;
    lapic[index] = value;
    return lapic[index];
}

int main(void)
{
    lapicw(LINT0, MASKED);
    lapicw(LINT1, MASKED);
    lapicw(ICRHI, 0);
    lapicw(ICRLO, BCAST | INIT | LEVEL);
    volatile unsigned int *lapic = (unsigned int *)LAPIC_DEFAULT_BASE;
    while (lapic[ICRLO] & DELIVS)
        ;
    lapicw(TPR, 0);
    lapicw(TIMER, PERIODIC | (T_IRQ0 + IRQ_TIMER));
    lapicw(TICR, 10000000);
}