TARGET = if.bin
OBJS = crt0.o if.o

CC = gcc
LD = ld
AS = nasm
CFLAGS += -nostdlib -fno-asynchronous-unwind-tables \
	-g -fno-stack-protector -m32 -O0
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