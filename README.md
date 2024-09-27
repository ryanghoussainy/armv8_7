
# ARM8_7 Project

This project consists of three parts:
1. a Raspberry PI 3.5 compiler, executing binary
2. an Assembler, converting assembly to binary
3. an extension, for which my group decided to make a shell emulator

### Requirements

- a C compiler (eg. **gcc**)
- **make** (recommended)

## 1. Emulator

### Compilation & Execution

In `arm8_7/src`:
```bash
make emulate
./emulate file_in.bin [file_out.txt]
```
If no `file_out.txt` is provided, then it defaults to stdout (it will print the results).

## 2. Assembler

### Compilation & Execution

In `arm8_7/src`:
```bash
make assemble
./assemble file_in.asm file_out.bin
```

## 3. Extension - Shell Emulator


### Compilation & Execution

In `arm8_7/extension`:
```bash
make shell
./shell file_in.txt [file_out.txt]
```

The emulator reads a text file which contains a list of Linux commands separated by a newline:
```
mkdir newdir
mkdir newdir2
ls
mkdir newdir/newdir3
ls newdir
cd /newdir/newdir3
ls
```

Like the Emulator, you can optionally provide a `file_out.txt`. If not provided, the results will be printed.
