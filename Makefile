TARGET = dax86
OBJS = main.o emulator_functions.o instructions.o modrm.o io.o

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
	$(CC) -o $@ $(OBJS) -lm

create-docker:
	docker build -t dax86 .
	docker run -dit -v $$(pwd):/dax86 --name dax86-c $$(docker images -q dax86)
	docker exec -it $$(docker ps -aqf "name=dax86-c") sh

clean-docker:
	docker stop $$(docker ps -aqf "name=dax86-c")
	docker rm $$(docker ps -aqf "name=dax86-c")
	docker image rm $$(docker images -q dax86)