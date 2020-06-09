# dax86

x86 (i386) Emulator in C

Why:

I wanted to trace how an OS runs on CPU.

What:

- Runs non-modified xv6 (memfs) image from boot.
- Each instruction is hand-implemented. (You'll know what I mean once you look into instruction functions.)
- Logic representation > performance

In scope:

- Instructions required for running xv6
- Real mode
- Protected mode
- Paging
- MP configuration
- Software/Hardware interrupts
- Device emulation (disk, keyboard, APIC timer, local APIC, IO APIC, UART etc)

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
# basic use
./dax86 [binary_file]

# run xv6
./dax86 xv6memfs.img

# verbose run (prints each op)
./dax86 [binary_file] -v
```

##### Test

```
# test all
./test.sh

# test specific one in tests/exec directory
./test.sh [test_name]

# directory test binary (stops at EIP: 0x0)
./dax86 test [binary_file]
```

##### Commands to Analyze Test Cases

- Disassemble Binary

```
# 32 bit all the way 
ndisasm -b 32 [binary_file]

# 32 bit after 0xFF bytes of real mode instructions
ndisasm -b 32 [binary_file] -k 0,0xFF
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