# dax86

x86 simulator for study-purpose (without reading QEMU)

Originally based on the book:

https://book.mynavi.jp/support/bookmook/x86/

Currently working on extension of this, adding functionalities such as:

- SIB
- Instruction prefixes
- More registers
- More instructions
- More eflags
- Protection mode
- Paging
- more

with bunch of comments for note-taking purposes.

Goal is to make xv86 work on this.

##### Setup Environment using Docker

The base image of Mini Debian Jessie is used.

The command below will build image, run a container with the image and execute shell in interactive mode.

```
make create-docker
```

To clean created container and image, run the command below.

```
make clean-docker
```

##### Build dax86

```
make
```

##### Run

```
./dax86 [binary_file]
```

##### Test

```
./test.sh
```

##### Commands to Analyze Test Cases and Others

- Disassemble Binary

```
ndisasm -b 32 [binary_file]
```

- View Binary

```
# hex
xxd [bin_file]

# bin
xxd -b [binary_file]
```