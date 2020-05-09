#define IOAPIC 0xFEC00000 // Default physical address of IO APIC
#define REG_TABLE 0x10    // Redirection table base

struct ioapic
{
    unsigned int reg;
    unsigned int pad[3];
    unsigned int data;
};

static unsigned int
ioapicread(int reg)
{
    volatile struct ioapic *ioapic = (volatile struct ioapic *)IOAPIC;
    ioapic->reg = reg;
    return ioapic->data;
}

static void
ioapicwrite(int reg, unsigned int data)
{
    volatile struct ioapic *ioapic = (volatile struct ioapic *)IOAPIC;
    ioapic->reg = reg;
    ioapic->data = data;
}

int main(void)
{
    int i, id, maxintr;

    id = ioapicread(0x0) >> 24;

    ioapicwrite(REG_TABLE + 2 * 1, (32 + 3));
    ioapicwrite(REG_TABLE + 2 * 1 + 1, 3);

    maxintr = (ioapicread(0x01) >> 16) & 0xFF;
}