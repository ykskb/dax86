// Table entry types
#define MPPROC 0x00   // One per processor
#define MPBUS 0x01    // One per bus
#define MPIOAPIC 0x02 // One per I/O APIC
#define MPIOINTR 0x03 // One per bus interrupt source
#define MPLINTR 0x04  // One per system interrupt source

#define SERIALDATA 0x3f8

struct mp
{                               // floating pointer
    unsigned char signature[4]; // "_MP_"
    void *physaddr;             // phys addr of MP config table
    unsigned char length;       // 1
    unsigned char specrev;      // [14]
    unsigned char checksum;     // all bytes must add up to 0
    unsigned char type;         // MP system config type
    unsigned char imcrp;
    unsigned char reserved[3];
};

struct mpconf
{                               // configuration table header
    unsigned char signature[4]; // "PCMP"
    unsigned short length;      // total table length
    unsigned char version;      // [14]
    unsigned char checksum;     // all bytes must add up to 0
    unsigned char product[20];  // product id
    unsigned int *oemtable;     // OEM table pointer
    unsigned short oemlength;   // OEM table length
    unsigned short entry;       // entry count
    unsigned int *lapicaddr;    // address of local APIC
    unsigned short xlength;     // extended table length
    unsigned char xchecksum;    // extended table checksum
    unsigned char reserved;
};

struct mpproc
{                               // processor table entry
    unsigned char type;         // entry type (0)
    unsigned char apicid;       // local APIC id
    unsigned char version;      // local APIC verison
    unsigned char flags;        // CPU flags
#define MPBOOT 0x02             // This proc is the bootstrap processor.
    unsigned char signature[4]; // CPU signature
    unsigned int feature;       // feature flags from CPUID instruction
    unsigned char reserved[8];
};

struct mpioapic
{                          // I/O APIC table entry
    unsigned char type;    // entry type (2)
    unsigned char apicno;  // I/O APIC id
    unsigned char version; // I/O APIC version
    unsigned char flags;   // I/O APIC flags
    unsigned int *addr;    // I/O APIC address
};

static inline void
outb(unsigned short port, unsigned char data)
{
    asm volatile("out %0,%1"
                 :
                 : "a"(data), "d"(port));
}

void printint(unsigned int n)
{
    outb(SERIALDATA, (unsigned char)(n));
    outb(SERIALDATA, (unsigned char)(n >> 8));
    outb(SERIALDATA, (unsigned char)(n >> 16));
    outb(SERIALDATA, (unsigned char)(n >> 24));
}

int memcmp(void *str1, void *str2, unsigned int count)
{
    register const unsigned char *s1 = (const unsigned char *)str1;
    register const unsigned char *s2 = (const unsigned char *)str2;

    while (count-- > 0)
    {
        // outb(SERIALDATA, (unsigned char)*s1);
        // outb(SERIALDATA, (unsigned char)*s2);
        if (*s1++ != *s2++)
            return s1[-1] < s2[-1] ? -1 : 1;
    }
    return 0;
}

static unsigned char
sum(unsigned char *addr, int len)
{
    int i, sum;

    sum = 0;
    for (i = 0; i < len; i++)
    {
        sum += addr[i];
    }
    return sum;
}

// Look for an MP structure in the len bytes at addr.
static struct mp *
mpsearch1(unsigned int a, int len)
{
    unsigned char *e, *p, *addr;

    addr = (void *)(a);
    e = addr + len;
    for (p = addr; p < e; p += sizeof(struct mp))
    {
        if (memcmp(p, "_MP_", 4) == 0 && sum(p, sizeof(struct mp)) == 0)
            return (struct mp *)p;
    }
    return 0;
}

// Search for the MP Floating Pointer Structure, which according to the
// spec is in one of the following three locations:
// 1) in the first KB of the EBDA;
// 2) in the last KB of system base memory;
// 3) in the BIOS ROM between 0xE0000 and 0xFFFFF.
static struct mp *
mpsearch(void)
{
    unsigned char *bda;
    unsigned int p;
    struct mp *mp;

    bda = (unsigned char *)(0x400);

    if ((p = ((bda[0x0F] << 8) | bda[0x0E]) << 4))
    {
        if ((mp = mpsearch1(p, 1024)))
            return mp;
    }
    else
    {
        p = ((bda[0x14] << 8) | bda[0x13]) * 1024;
        if ((mp = mpsearch1(p - 1024, 1024)))
            return mp;
    }
    return mpsearch1(0xF0000, 0x10000);
}

// Search for an MP configuration table.  For now,
// don't accept the default configurations (physaddr == 0).
// Check for correct signature, calculate the checksum and,
// if correct, check the version.
// To do: check extended table checksum.
static struct mpconf *
mpconfig(struct mp **pmp)
{
    struct mpconf *conf;
    struct mp *mp;

    if ((mp = mpsearch()) == 0 || mp->physaddr == 0)
        return 0;
    conf = (struct mpconf *)((unsigned int)mp->physaddr);
    if (memcmp(conf, "PCMP", 4) != 0)
        return 0;
    if (conf->version != 1 && conf->version != 4)
        return 0;
    if (sum((unsigned char *)conf, conf->length) != 0)
        return 0;
    *pmp = mp;
    // outb(SERIALDATA, 0xdd);
    return conf;
}

int main(void)
{
    unsigned char *p, *e;
    int ismp;
    struct mp *mp;
    struct mpconf *conf;
    struct mpproc *proc;
    struct mpioapic *ioapic;

    if ((conf = mpconfig(&mp)) == 0)
        return;
    ismp = 1;
    // lapic = (unsigned int *)conf->lapicaddr;
    for (p = (unsigned char *)(conf + 1), e = (unsigned char *)conf + conf->length; p < e;)
    {
        switch (*p)
        {
        case MPPROC:
            proc = (struct mpproc *)p;
            // if (ncpu < NCPU)
            // {
            //     cpus[ncpu].apicid = proc->apicid; // apicid may differ from ncpu
            //     ncpu++;
            // }
            p += sizeof(struct mpproc);
            outb(SERIALDATA, 'c');
            continue;
        case MPIOAPIC:
            ioapic = (struct mpioapic *)p;
            // ioapicid = ioapic->apicno;
            p += sizeof(struct mpioapic);
            outb(SERIALDATA, 'i');
            continue;
        case MPBUS:
        case MPIOINTR:
        case MPLINTR:
            p += 8;
            continue;
        default:
            ismp = 0;
            break;
        }
    }
    outb(SERIALDATA, 'm');
    outb(SERIALDATA, 'p');
    // if (!ismp)
    //     panic("Didn't find a suitable machine");

    // if (mp->imcrp)
    // {
    //     // Bochs doesn't support IMCR, so this doesn't run on Bochs.
    //     // But it would on real hardware.
    //     outb(0x22, 0x70);          // Select IMCR
    //     outb(0x23, inb(0x23) | 1); // Mask external interrupts.
    // }
}