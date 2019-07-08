TARGET = dax86
OBJS = main.o emulator_functions.o instructions.o modrm.o

CC = /usr/bin/gcc
CFLAGS += -Wall

.PHONY: all
all :
	make $(TARGET)

# Dependencies
# gcc -c: only compile & assembly to .o
# $<: first prerequisite
%.o : %.c Makefile
	$(CC) $(CFLAGS) -c $< 

$(TARGET) : $(OBJS) Makefile
	$(CC) -o $@ $(OBJS)
