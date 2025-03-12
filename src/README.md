## Prerequisites

- **GCC**: The GNU Compiler Collection (Or any other c compiler(CLANG, TCC) just change a CC var in makefile.
- **SDL2** (optional): Required if you want to build the project with SDL2 support.

## Building the Project

To build the project, you can use the provided `Makefile`. The following targets are available:

### Basic Build

- **`build`**: Compiles the compiler without SDL2 support.
  ```bash
  make build
  ```

- **`buildsdl`**: Compiles the compiler with SDL2 support.
  ```bash
  make buildsdl
  ```

### Compilation and Execution

- **`compile`**: Compiles a source file (`code.blang`) into bytecode (`code.vm`).
  ```bash
  make compile
  ```

- **`run`**: Executes the compiled bytecode (`code.vm`).
  ```bash
  make run
  ```

- **`runtime`**: Executes the compiled bytecode (`code.vm`) and measures the execution time.
  ```bash
  make runtime
  ```

- **`all`**: Builds the compiler, compiles the source file, and runs the compiled bytecode.
  ```bash
  make all
  ```

- **`allsdl`**: Builds the compiler with SDL2 support, compiles the source file, and runs the compiled bytecode.
  ```bash
  make allsdl
  ```
  
- **`buildNative`**: Builds the native assembly targeting(NASM) this is not yet finished.
  ```bash
  make buildNative
  ```

### Additional Commands

- **`dissasembly`**: Disassembles the bytecode (`code.vm`).
  ```bash
  make dissasembly
  ```

- **`writeHeader`**: Writes the header for the bytecode (`code.vm`).
  ```bash
  make writeHeader
  ```

- **`buildExeSdl`**: Builds an executable with SDL2 support.
  ```bash
  make buildExeSdl
  ```

- **`buildExe`**: Builds an executable without SDL2 support.
  ```bash
  make buildExe
  ```

- **`interpreter`**: Runs the bytecode interpreter (Vars are not yet suported).
  ```bash
  make interpreter
  ```

### Cleaning Up

- **`clean`**: Removes the compiled binary and bytecode files.
  ```bash
  make clean
  ```

## Usage

1. **Compile your source code to bycode**:
   ```bash
   make compile
   ```

2. **Run the compiled bytecode**:
   ```bash
   make run
   ```

3. **Clean up the project**:
   ```bash
   make clean
   ```

## Customization

You can modify the `Makefile` to change the compiler flags or add new targets as needed 
or use executable directly builded.
```
Usage:  Compile      -c <path to program> <path to save>
	Run 	     -r <path to saved>
	Disassemble  -d <path to saved>
	Write header -b <path to save>
	Interpreter  -i <none>

```

