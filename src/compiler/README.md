
## Project Overview

The tool reads BVM bytecode from `code.vm` and generates NASM-compatible assembly. The generated assembly is assembled and linked into a native executable (`nativeApp`). The current implementation focuses on basic arithmetic and I/O operations.

---

## Implemented Instructions
Here's a comprehensive list of all BVM instructions with implementation status:


## BVM Instruction Implementation Status

| Instruction        | Category         | Implemented | Notes                                  |
|--------------------|------------------|-------------|----------------------------------------|
| PUSH               | Stack Operations | ✅           | Integer values only                    |
| ADD                | Arithmetic       | ✅           | Integer addition (`pop rax; add rbx`)  |
| DEC                | Arithmetic       | ✅           | Handled as subtraction                 |
| PRINT              | I/O              | ✅           | Integer printing via `PRINT_INT`        |
| POP                | Stack Operations | ❌           |                                        |
| PUSHF              | Stack Operations | ❌           |                                        |
| PUSHIP             | Stack Operations | ❌           |                                        |
| PUSHSP             | Stack Operations | ❌           |                                        |
| PRINTSTACK         | I/O              | ❌           |                                        |
| PRINTSTRING        | I/O              | ❌           |                                        |
| REVERSSTRING       | String           | ❌           |                                        |
| MUL                | Arithmetic       | ❌           |                                        |
| DIV                | Arithmetic       | ❌           |                                        |
| MOD                | Arithmetic       | ❌           |                                        |
| NOT                | Logic            | ❌           |                                        |
| SHR                | Bitwise          | ❌           |                                        |
| SHL                | Bitwise          | ❌           |                                        |
| XOR                | Bitwise          | ❌           |                                        |
| OR                 | Bitwise          | ❌           |                                        |
| AND                | Bitwise          | ❌           |                                        |
| BNOT               | Bitwise          | ❌           |                                        |
| DUP                | Stack Manip      | ❌           |                                        |
| OVER               | Stack Manip      | ❌           |                                        |
| ROT                | Stack Manip      | ❌           |                                        |
| SWAP_NO            | Stack Manip      | ❌           |                                        |
| IF                 | Control Flow     | ❌           |                                        |
| JMP                | Control Flow     | ❌           |                                        |
| JMPT               | Control Flow     | ❌           |                                        |
| JMPF               | Control Flow     | ❌           |                                        |
| JMPSTACK           | Control Flow     | ❌           |                                        |
| SETSP              | Memory           | ❌           |                                        |
| SETSPSTACK         | Memory           | ❌           |                                        |
| RESTORE            | Memory           | ❌           |                                        |
| COPY               | Memory           | ❌           |                                        |
| COPYSTACK          | Memory           | ❌           |                                        |
| SWAP               | Memory           | ❌           |                                        |
| MEM                | Memory           | ❌           |                                        |
| MEMSTACK           | Memory           | ❌           |                                        |
| INSTRUCTIONSTACK   | Debug            | ❌           |                                        |
| NOP                | Control Flow     | ❌           |                                        |
| HALT               | Control Flow     | ❌           |                                        |
| INC                | Arithmetic       | ❌           |                                        |
| WRITE              | Syscall          | ❌           | Requires `BVM_SYSCALLS`                |
| OPEN               | Syscall          | ❌           | Requires `BVM_SYSCALLS`                |
| CLOSE              | Syscall          | ❌           | Requires `BVM_SYSCALLS`                |
| DUPF               | Syscall          | ❌           | Requires `BVM_SYSCALLS`                |
| DUP2               | Syscall          | ❌           | Requires `BVM_SYSCALLS`                |
| EXIT               | Syscall          | ❌           | Requires `BVM_SYSCALLS`                |
| TRUNCATE           | Syscall          | ❌           | Requires `BVM_SYSCALLS`                |
| ISATTY             | Syscall          | ❌           | Requires `BVM_SYSCALLS`                |
| READ               | Syscall          | ❌           | Requires `BVM_SYSCALLS`                |
| SWAB               | Syscall          | ❌           | Requires `BVM_SYSCALLS`                |
| SLEEP              | Syscall          | ❌           | Requires `BVM_SYSCALLS`                |
| SYSTEMS            | Syscall          | ❌           | Requires `BVM_SYSTEM`                  |
| DRIVER             | Hardware         | ❌           |                                        |
| NEWLINE            | I/O              | ❌           |                                        |
| LABEL              | Metadata         | ❌           |                                        |
| FFI                | Interop          | ❌           |                                        |
| END                | Control Flow     | ✅           | Implicitly handled by parser           |


## Dependencies

- **NASM**: Netwide Assembler (for assembling `.asm` files).
- **ld**: GNU linker (for linking object files).
- **GCC**: For compiling the generator (optional).

---

## Building and Usage

### 1. Generate Bytcode
```bash
# Compile the generator (if not prebuilt)
make all

```

### 2. Assemble and Link
```bash
nasm -f elf64 nasmAsm.asm -o nasmAsm.o
ld nasmAsm.o -o nativeApp
or use make buildNativeC
```

### 3. Run the Executable
```bash
./nativeApp
```

---

