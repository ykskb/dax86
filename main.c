#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Memory size: 1MB */
#define MEMORY_SIZE (1024 * 1024)

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
    REGISTERS_COUNT
};

char *register_names[] = {"EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI"};

typedef struct
{
    uint32_t registers[REGISTERS_COUNT];
    uint32_t eglags;
    uint8_t *memory;
    uint32_t eip;
} Emulator;

static Emulator *create_emu(size_t size, uint32_t eip, uint32_t esp)
{
    Emulator *emu = malloc(sizeof(Emulator));
    emu->memory = malloc(size);

    /* Resets registers. */
    memset(emu->registers, 0, sizeof(emu->registers));
    emu->eip = eip;
    emu->registers[ESP] = esp;

    return emu;
}

static void destroy_emu(Emulator *emu)
{
    free(emu->memory);
    free(emu);
}

static void dump_registers(Emulator *emu)
{
    int i;
    for (i = 0; i < REGISTERS_COUNT; i++)
    {
        printf("%s: %08x\n", register_names[i], emu->registers[i]); // %08x: prepends 0 to make 8 digits.
    }
    printf("EIP: %08x\n", emu->eip);
}

/* Retrieves code from memory by offset from EIP. */
uint32_t get_code8(Emulator *emu, int index)
{
    return emu->memory[emu->eip + index];
}

int32_t get_sign_code8(Emulator *emu, int index)
{
    return (int8_t)emu->memory[emu->eip + index];
}

uint32_t get_code32(Emulator *emu, int index)
{
    int i;
    uint32_t code = 0;
    /* i386 uses little endian (lower bytes to right). */
    for (i = 0; i < 4; i++)
    {
        /* Shift 8 bits to left per byte read */
        code |= get_code8(emu, index + i) << (i * 8);
    }
    return code;
}

/*
MOV reg 32bit-imm: 5 bytes
1 byte: op (0xb8) + reg
4 bytes: value (32 bit unsigned)
*/
void mov_r32_imm32(Emulator *emu)
{
    uint8_t reg = get_code8(emu, 0) - 0xB8;
    uint32_t value = get_code32(emu, 1);
    emu->registers[reg] = value; // does this map to enum Register?
    emu->eip += 5;
}

/*
JMP (Short): 2 bytes
1 byte: op
1 byte: operand (8 bit signed) -127 ~ 127
*/
void short_jump(Emulator *emu)
{
    int8_t offset = get_sign_code8(emu, 1);
    emu->eip += (offset + 2);
}

typedef void instruction_func_t(Emulator *); // instruction_func_t = void func(Emulator*)

instruction_func_t *instructions[256];

void init_instructions(void)
{
    int i;
    memset(instructions, 0, sizeof(instructions));
    for (i = 0; i < 8; i++)
    {
        instructions[0xB8 + i] = mov_r32_imm32; // Why 0xB8 ~ 0xBF??
    }
    instructions[0xEB] = short_jump; // Why 0xEB?
}

int main(int argc, char *argv[])
{
    FILE *binary; // FILE: pointer to stream
    Emulator *emu;

    if (argc != 2)
    {
        printf("Usage: dax86 [filename]\n");
        return 1;
    }

    /* Initial setup: EIP: 0, ESP: 0x7c00 */
    emu = create_emu(MEMORY_SIZE, 0x0000, 0x7c00);

    binary = fopen(argv[1], "rb"); // rb: read-binary (r: translated mode for "\n")
    if (binary == NULL)
    {
        printf("Could not open: %s", argv[1]);
        return 1;
    }

    /* 
    Loads binary into emulator.
    Each index stores 1 byte of binary.
    0x200 (512) * 1 byte = 512 bytes
    */
    fread(emu->memory, 1, 0x200, binary); // fread(*ptr, byte, num(of byte to read), *stream)
    fclose(binary);

    init_instructions();

    while (emu->eip < MEMORY_SIZE)
    {
        uint8_t op = get_code8(emu, 0);
        if (instructions[op] == NULL)
        {
            printf("Op: %x not implemented.\n", op); // %x: int as hex
            break;
        }

        instructions[op](emu);

        if (emu->eip == 0x00)
        {
            printf("End of program :)\n");
            break;
        }
    }

    dump_registers(emu);
    destroy_emu(emu);
    return 0;
}
