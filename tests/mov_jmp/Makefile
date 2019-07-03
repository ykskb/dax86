TARGET = mov_jmp.bin
I386_ELF_GCC = /usr/local/Cellar/i386-elf-gcc/8.2.0/lib/gcc/i386-elf/8.2.0/include

CC = gcc
LD = ld
AS = nasm
CFLAGS += -nostdlib -fno-asynchronous-unwind-tables \
	-g -fno-stack-protector
LDFLAGS += --entry=func --oformat=binary

.PHONY: all
all :
	make $(TARGET)

%.o : %.c Makefile
	$(CC) $(CFLAGS) -c $<

%.bin : %.o Makefile
	$(LD) $(LDFLAGS) -o $@ $<

%.bin : %.asm Makefile
	$(AS) -f bin -o $@ $<