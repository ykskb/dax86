TARGET = main_func.bin
OBJS = crt0.o main_func.o

I386_ELF_GCC = /usr/local/Cellar/i386-elf-gcc/8.2.0/lib/gcc/i386-elf/8.2.0/include

CC = gcc
LD = ld
AS = nasm
CFLAGS += -nostdlib -fno-asynchronous-unwind-tables \
	-g -fno-stack-protector -m32
LDFLAGS += -m elf_i386 --entry=start --oformat=binary -Ttext 0x7c00

.PHONY: all
all :
	make $(TARGET)

%.o : %.c Makefile
	$(CC) $(CFLAGS) -c $<

%.o : %.asm Makefile
	$(AS) -f elf $<

$(TARGET) : $(OBJS) Makefile
	$(LD) $(LDFLAGS) -o $@ $(OBJS)