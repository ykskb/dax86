#define BACKSPACE 0x100
#define CRTPORT 0x3d4

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef uint pde_t;

static ushort *crt = (ushort *)0xb8000; // CGA memory

static void consputc(int);

static inline void
outb(ushort port, uchar data)
{
    asm volatile("out %0,%1"
                 :
                 : "a"(data), "d"(port));
}

static inline uchar
inb(ushort port)
{
    uchar data;

    asm volatile("in %1,%0"
                 : "=a"(data)
                 : "d"(port));
    return data;
}

static void
printint(int xx, int base, int sign)
{
    static char digits[] = "0123456789abcdef";
    char buf[16];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
        x = -xx;
    else
        x = xx;

    i = 0;
    do
    {
        buf[i++] = digits[x % base];
    } while ((x /= base) != 0);

    if (sign)
        buf[i++] = '-';

    while (--i >= 0)
        consputc(buf[i]);
}

static void
cgaputc(int c)
{
    int pos;

    // Cursor position: col + 80*row.
    outb(CRTPORT, 14);
    pos = inb(CRTPORT + 1) << 8;
    outb(CRTPORT, 15);
    pos |= inb(CRTPORT + 1);

    if (c == '\n')
        pos += 80 - pos % 80;
    else if (c == BACKSPACE)
    {
        if (pos > 0)
            --pos;
    }
    else
        crt[pos++] = (c & 0xff) | 0x0700; // black on white

    // if (pos < 0 || pos > 25 * 80)
    //     panic("pos under/overflow");

    if ((pos / 80) >= 24)
    { // Scroll up.
        // memmove(crt, crt + 80, sizeof(crt[0]) * 23 * 80);
        pos -= 80;
        // memset(crt + pos, 0, sizeof(crt[0]) * (24 * 80 - pos));
    }

    outb(CRTPORT, 14);
    outb(CRTPORT + 1, pos >> 8);
    outb(CRTPORT, 15);
    outb(CRTPORT + 1, pos);
    crt[pos] = ' ' | 0x0700;
}

#define COM1 0x3f8

void microdelay(int us)
{
}

void uartputc(int c)
{
    int i;

    for (i = 0; i < 128 && !(inb(COM1 + 5) & 0x20); i++)
        microdelay(10);
    outb(COM1 + 0, c);
}

void consputc(int c)
{
    if (c == BACKSPACE)
    {
        uartputc('\b');
        uartputc(' ');
        uartputc('\b');
    }
    else
        uartputc(c);
    cgaputc(c);
}

void cprintf(char *fmt, ...)
{
    int i, c, locking;
    uint *argp;
    char *s;

    // if (fmt == 0)
    //     panic("null fmt");

    argp = (uint *)(void *)(&fmt + 1);
    for (i = 0; (c = fmt[i] & 0xff) != 0; i++)
    {
        if (c != '%')
        {
            consputc(c);
            continue;
        }
        c = fmt[++i] & 0xff;
        if (c == 0)
            break;
        switch (c)
        {
        case 'd':
            printint(*argp++, 10, 1);
            break;
        case 'x':
        case 'p':
            printint(*argp++, 16, 0);
            break;
        case 's':
            if ((s = (char *)*argp++) == 0)
                s = "(null)";
            for (; *s; s++)
                consputc(*s);
            break;
        case '%':
            consputc('%');
            break;
        default:
            // Print unknown % sequence to draw attention.
            consputc('%');
            consputc(c);
            break;
        }
    }
}

int main(void)
{
    cprintf("test 123\n");
}