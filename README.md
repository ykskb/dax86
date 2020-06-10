# dax86

x86 (i386) Emulator in C

Why:

- I wanted to trace how an OS runs on CPU at archtecture / instruction level.

What:

- Runs vanilla xv6 (memfs) image from boot.
- Each instruction is hand-implemented.
- Representation of logic > performance

Done:

- Instructions required for running xv6
- Real mode
- Protected mode
- Paging
- MP configuration
- Software/Hardware interrupts
- Device emulation (disk, keyboard, APIC timer, local APIC, IO APIC, UART etc)

To do:

- FPU-related instructions
- Exception
- Virtual 8086 mode
- TLB

System requirements:

- Memory: 512MB
- CPU: scheduler loop will occupy CPU resource. `nice` command, cgroups or docker resource setting might help here.

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