# dax86

x86 Simulator for Study-purpose | Implementation of x86 in C

Referenced these publications with many thanks:

- https://software.intel.com/en-us/articles/intel-sdm
- http://ref.x86asm.net/
- https://x86.puri.sm/
- https://c9x.me/x86/
- https://book.mynavi.jp/support/bookmook/x86/

Currently working on functionalities such as:

- More instructions (in-progress)
- Instruction prefixes (in-progress)
- More registers (in-progress)
- More eflags (in-progress)
- Protection mode (starting-soon)
- Paging (starting-soon)
- SIB (done)
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