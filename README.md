# dax86

x86 Simulator | Implementation of x86 in C for Study-purpose

The goal of this project is to run xv6 on it so that one can have basic understanding of how OS works at the level of CPU architecture.

In scope:

Instructions required to run xv6:

- Real mode addressing
- Protection mode
- Paging mode etc

Out of scope:

- Floating point related instructions etc

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

Referenced these publications with many thanks:

- https://software.intel.com/en-us/articles/intel-sdm
- http://ref.x86asm.net/
- https://x86.puri.sm/
- https://c9x.me/x86/
- https://book.mynavi.jp/support/bookmook/x86/