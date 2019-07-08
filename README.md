# dax86

x86 simulator for study-purpose.

Based on the book:

https://book.mynavi.jp/support/bookmook/x86/

I've tweaked parts and added comments for note-taking purposes. I intend to extend it with additional features as well.

##### Build

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

<!-- Architecture of source binaries is i386. To compile source binaries, the command below should be run:

```
brew install i386-elf-gcc
``` -->

##### Commands to Analyze Test Cases and Others

- Disassemble Binary

```
ndisasm [binary_file]
```

- View Binary

```
# hex
xxd [bin_file]

# bin
xxd -b [binary_file]
```