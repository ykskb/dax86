TARGET = dax86
OBJS = \
	main.o\
	emulator.o\
	emulator_functions.o\
	instructions.o\
	modrm.o\
	io.o\
	shift.o\
	gdt.o\
	paging.o\
	twos_complement.o\
	interrupt.o\
	kbd.o\
	disk.o\
	instructions_00.o\
	instructions_10.o\
	instructions_20.o\
	instructions_30.o\
	instructions_40.o\
	instructions_50.o\
	instructions_60.o\
	instructions_70.o\
	instructions_80.o\
	instructions_90.o\
	instructions_A0.o\
	instructions_B0.o\
	instructions_C0.o\
	instructions_D0.o\
	instructions_E0.o\
	instructions_F0.o\
	instructions_0F00.o\
	instructions_0F20.o\
	instructions_0FB0.o

CC = /usr/bin/gcc
CFLAGS += -Wall

.PHONY: all create-docker clean-docker
all :
	make $(TARGET)

# Dependencies
# gcc -c: only compile & assembly to .o
# $<: first prerequisite
%.o : %.c Makefile
	$(CC) $(CFLAGS) -c $<

$(TARGET) : $(OBJS) Makefile
	$(CC) -o $@ $(OBJS) -lm -lpthread

create-docker:
	docker build -t dax86 .
	docker run -dit -v $$(pwd):/dax86 --name dax86-c $$(docker images -q dax86)
	docker exec -it $$(docker ps -aqf "name=dax86-c") bash

clean-docker:
	docker stop $$(docker ps -aqf "name=dax86-c")
	docker rm $$(docker ps -aqf "name=dax86-c")
	docker image rm $$(docker images -q dax86)