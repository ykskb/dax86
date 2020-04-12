# dax86

x86 (i386) Emulator in C

Primary purpose is to run xv6 so that one can trace how an OS runs at CPU level.

In scope:

- Instructions required for running xv6
- Real mode
- Protected mode
- Paging
- Device emulation (Keyboard, APIC, Display etc)

Out of scope:

- FPU-related instructions
- Virtual 8086 mode

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