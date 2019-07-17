# dax86

x86 simulator for study-purpose.

Based on the book:

https://book.mynavi.jp/support/bookmook/x86/

I've tweaked parts and added comments for note-taking purposes. I intend to extend it with additional features as well.

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