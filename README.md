

# Blang

**EXTREMELY IMPORTANT! THIS LANGUAGE IS A WORK IN PROGRESS! ANYTHING CAN CHANGE AT ANY MOMENT WITHOUT ANY NOTICE! USE THIS LANGUAGE AT YOUR OWN RISK!**

Blang is a **[Concatenative](https://en.wikipedia.org/wiki/Concatenative_programming_language)**, **[Stack-Oriented](https://en.wikipedia.org/wiki/Stack-oriented_programming)** **[Programming Language](https://en.wikipedia.org/wiki/Programming_language)** for **[Computers](https://en.wikipedia.org/wiki/Computer)**.

*(If you've never heard about this kind of language before, check out [https://concatenative.org/](https://concatenative.org/))*

---

## Development Milestones

- [x] Compiled to a custom instruction set (BVM bytecode)
- [x] Conditional/selection statements
- [x] Loops
- [x] [Turing-complete](./examples/celluarautomata/rule110.blang)
- [x] Cross-platform
- [x] Strings support
- [x] Functions
- [ ] Self-hosted
- [ ] Statically typed
- [ ] Optimized
- [ ] Implement a VM as a custom CPU for FPGA

---

## Examples

### Hello, World:

```forth
0 "HelloWorld" printstring
endscript .
```

### Simple program that prints all uppercase letters:

```forth
65 ? a
while
    a charprint 
    89 < if
        breakloop
    end 
    1 a + ? a
endloop
endscript .
```

---

## Quick Start

### Compile the project:

```bash
gcc main.c
```

OR

```bash
make all
```

### Compile to bytecode:

```bash
./main.out -c <path-to-program> <path-to-save>
```

### Run the bytecode:

```bash
./main.out -r <path-to-bytecode>
```

### Usage:

```bash
Usage:  Compile -c <path-to-program> <path-to-save>
        Run -r <path-to-saved>
```

---

## Language Reference

This section describes the features supported by the language so far. **Since the language is a work in progress, everything in this section is subject to change.**

### Literals

#### Integer

An integer is a sequence of decimal digits. When an integer is encountered, it is pushed onto the data stack for processing by the relevant operations.

Example:

```forth
10 20 +
```

The code above pushes `10` and `20` onto the data stack and sums them up with the `+` operation.

#### String

TBD

#### Character

A character is currently used as a variable on the heap. For printing a character, there is a function called `charprint`. For working with strings, there is a function called `printstring`. You must provide an end token, which is `0`.

Example:

```forth
35 ? taraba 
32 ? space 
10 ? newLine
```

---

### Intrinsics (Built-in Words)

#### Stack Manipulation

| Name    | Signature        | Description                                                                                       |
| ---     | ---              | ---                                                                                               |
| `dup`   | `a -- a a`       | Duplicate an element on top of the stack.                                                         |
| `swap`  | `a b -- b a`     | Swap 2 elements on the top of the stack.                                                          |
| `drop`  | `a b -- a`       | Drop the top element of the stack.                                                                |
| `print` | `a b -- a`       | Print the element on top of the stack in a free form to stdout and remove it from the stack.      |
| `over`  | `a b -- a b a`   | Copy the element below the top of the stack.                                                      |
| `rot`   | `a b c -- b c a` | Rotate the top three stack elements.                                                              |

#### Comparison

| Name | Signature                              | Description                                                       |
| ---  | ---                                    | ---                                                               |
| `= ` | `[a: int] [b: int] -- [a == b : int]` | Check if two elements on top of the stack are equal.              |
| `=!` | `[a: int] [b: int] -- [a != b : int]` | Check if two elements on top of the stack are not equal.          |
| `> ` | `[a: int] [b: int] -- [a > b  : int]` | Apply the greater comparison on top two elements.                |
| `< ` | `[a: int] [b: int] -- [a < b  : int]` | Apply the less comparison on top two elements.                   |
| `<!` | `[a: int] [b: int] -- [a >= b : int]` | Apply the greater or equal comparison on top two elements.       |
| `>!` | `[a: int] [b: int] -- [a <= b : int]` | Apply the less or equal comparison on top two elements.          |

#### Arithmetic

| Name     | Signature                                        | Description                                                                                                              |
| ---      | ---                                              | ---                                                                                                                      |
| `+`      | `[a: int] [b: int] -- [a + b: int]`              | Sum up two elements on the top of the stack.                                                                            |
| `-`      | `[a: int] [b: int] -- [a - b: int]`              | Subtract two elements on the top of the stack.                                                                           |
| `*`      | `[a: int] [b: int] -- [a * b: int]`              | Multiply two elements on top of the stack.                                                                               |
| `/`      | `[a: int] [b: int] -- [a / b: int]`              | Divide two elements on top of the stack.                                                                                 |
| `%`      | `[a: int] [b: int] -- [a % b: int]`              | Modulo two elements on top of the stack.                                                                                 |

#### Bitwise

| Name  | Signature                            | Description                   |
| ---   | ---                                  | ---                           |
| `shr` | `[a: int] [b: int] -- [a >> b: int]` | Right **unsigned** bit shift. |
| `shl` | `[a: int] [b: int] -- [a << b: int]` | Left bit shift.               |
| `or`  | `[a: int] [b: int] -- [a \| b: int]` | Bitwise `OR`.                 |
| `and` | `[a: int] [b: int] -- [a & b: int]`  | Bitwise `AND`.                |
| `not` | `[a: int] -- [~a: int]`              | Bitwise `NOT`.                |

#### Memory

| Name         | Signature                      | Description                                                                                               |
| ---          | ---                            | ---                                                                                                       |
| `?`          | `<stack> let <name>`           | Store a value from `<sp>` at the var address. If the var is not declared, create the var on `<stacksize - numofvars - 1>`.  |  
| `arr`        | `arr <name> <size>`            | Create an array at the first free address on `stacksize - numofvars - 1`.                                 |
| `&`          | `ptr <name>`                   | Push a memory address of a var onto the top of the stack.                                                 |                         
| `@`          | `<stack> ptrval`               | Push a value from the memory address on top of the stack onto the top of the stack.                       |
| `??`         | `<stack> <stack> ptrval`       | Store a value from `<sp-1>` to the memory address on `<sp>`.                                              |

#### System

- `syscall<n>` - TBD
- `c functions` - TBD

---

### Control Flow

#### If-Condition

Simple `if`:

```html
<Comparison> if
  <body> 
end
```

`If/else`:

```html
<Comparison> dup if
  <body> 
end
else
  <body>
end
```

Example:

```forth
0 10 
= dup
if
    20 print
    30 40 = dup
    if 
        30 print
    end
    else 
        50 print
    end
end
else
    30 print
end
40 print
endscript .
```

#### While-Loop

Infinite loop:

```html
while 
   <body>
endloop
```

Conditional loop (use `breakloop` to exit):

```html
while
  <body>
  <Comparison> if
    <body>
    breakloop 
  end
endloop
```

---

### Procedures and Functions

The `word` keyword declares a procedure or function. Values can be passed through the stack or the heap.

Example:

```forth
word printString 
    while
        dup @ charprint 
        0 = if
            breakloop
        end
        1 + 
    endloop
    drop
endword

& H printString
```

#### Procedure Pointers

TBD

---

### Constants

Constants are declared like this:

```forth
word pi 3.1415 endword
```

---

### Memory

The heap is built on top of the stack. Variables are global for the entire duration of the program.

---

### Type Checking

There are 4 types: `int64`, `float64`, `uint64`, and `ch` (basically `uint64`, but used in strings). The type is inferred for math operations based on the last element on the stack. For example, if we have `10 10.2` on the stack and we perform the `+` operation, the `+` operation will use the `float` type as the operand.

---

## Example Programs

### Memory Copy Example

```forth
0
arr toCopyInto 50 

word memcpy
    ? memcpySize
    drop
    ? memcpySrc
    drop
    ? memcpyDest
    drop
    SP ? memcpySP
    drop
    while
        memcpySrc @ memcpyDest ??
        memcpySrc 1 + ? memcpySrc 
        memcpyDest 1 + ? memcpyDest
        memcpySize 0 = if
            breakloop
        end
        memcpySize -1 + ? memcpySize
    endloop
    memcpySP SET	
endword

word memmove memcpy endword

SP ? temp
drop

0 "HelloWorld" 10 
& toCopyInto0 temp 12 memcpy

1 & toCopyInto0 12 write 
0 & toCopyInto0 12 read 
1 & toCopyInto0 6 write 
10 sleep 
1 & toCopyInto0 6 write
endscript .
```

### Game of Life Implementation
![Screenshot_7](https://github.com/user-attachments/assets/570c8376-1456-491c-8ac7-4f36eb254b77)




```forth
0

50 ? MAX_X_POS 
40 ? MAX_Y_POS 
word BACKGROUND_CHAR 32 endword  
word CELL_CHAR 35 endword   
word SLEEP_TIME 0.1 endword

arr checkCoords 100

arr mainMatrix 2000



arr neighborCountBuffer 2000



word increment 1 + endword

word matrixIndex  MAX_X_POS updateCell_y * updateCell_x +   endword
word matrixIndexP MAX_X_POS printCells_y * printCells_x +   endword
word matrixIndexC MAX_X_POS tempY        *        tempX +   endword
1234 ? seed

word rand	
	164603309 seed * 14738995463 or ? seed
endword


word matrixInit
	0 ? printCells_x
	0 ? printCells_y
	SP ? printCells_SP
	
	while MAX_Y_POS printCells_y loopBreak
		printCells_y 1 + ? printCells_y
		
		0 ? printCells_x
		
		while printCells_x MAX_X_POS  loopBreak
			printCells_x 1 + ? printCells_x
			2 rand / 1000000 >  dup if
				BACKGROUND_CHAR
				& mainMatrix0 matrixIndexP + ??
			end 
			else
				CELL_CHAR
				& mainMatrix0 matrixIndexP + ??
			end 
		endloop
	endloop
	0 SET
endword


word loopBreak 
			= if
				breakloop
			end  
endword


word updateCell 
	
	0 ? updateCell_x
	MAX_Y_POS ? updateCell_y
	SP ? upadteCell_SP
	
	while 0 updateCell_y loopBreak
		updateCell_y -1 + ? updateCell_y
		
		0 ? updateCell_x
		
		while updateCell_x MAX_X_POS  loopBreak
			updateCell_x 1 + ? updateCell_x
				
			3 & neighborCountBuffer0 matrixIndex + @   
			= if
						CELL_CHAR
						& mainMatrix0 matrixIndex + ??			
			end
			3 & neighborCountBuffer0 matrixIndex + @ >  
			2 & neighborCountBuffer0 matrixIndex + @ <
			or
			1 = if
						BACKGROUND_CHAR 
						& mainMatrix0 matrixIndex + ??
							
			end
		endloop
			
	endloop
	 
	upadteCell_SP SET
endword


word countNeighbors
	
	? posY
	drop
	 
	? posX
	drop
	
	SP ? countNeighbors_SP
	
	
	1 posY  + ? checkCoords0
 -1 posX  + ? checkCoords1
	
	1 posY  + ? checkCoords2
	0 posX  + ? checkCoords3
	
	1 posY  + ? checkCoords4
	1 posX  + ? checkCoords5

	0 posY  + ? checkCoords6
	1 posX  + ? checkCoords7

 -1 posY  + ? checkCoords8
	1 posX  + ? checkCoords9
	
 -1 posY  + ? checkCoords10
	0 posX  + ? checkCoords11
	
 -1 posY  + ? checkCoords12
 -1 posX  + ? checkCoords13
	
	0 posY  + ? checkCoords14
 -1 posX  + ? checkCoords15
	
	0 ? i
	0 ? neighbors
	
	
	while
		& checkCoords0 i + @ ? tempY	
		& checkCoords1 i + @ ? tempX
		
		0 tempX < if
			MAX_X_POS -1 + ? tempX 
		end
		
		MAX_X_POS -1 + tempX > if
			0 ? tempX 
		end
		
		0 tempY < if
			MAX_Y_POS -1 + ? tempY 
		end
		
		MAX_Y_POS -1 + tempY > if
			0 ? tempY 
		end
		
				
		& mainMatrix0 matrixIndexC + @ CELL_CHAR = if
			neighbors 1 + ? neighbors	
		end
		
					
		i 2 + ? i 
		i 16 = if 
			neighbors 
			breakloop
		end
	endloop

	countNeighbors_SP SET
	neighbors
	 
	
endword


word printCells 
	
	0 ? printCells_x
	MAX_Y_POS ? printCells_y
	SP ? printCells_SP
	
	while printCells_y 0  loopBreak
		printCells_y -1 + ? printCells_y
		
		-1 ? printCells_x
		
		while printCells_x MAX_X_POS  loopBreak
			printCells_x 1 + ? printCells_x
			& mainMatrix0 matrixIndexP + @
			charprint 0
			printCells_x printCells_y	countNeighbors
			& neighborCountBuffer0 matrixIndexP + ?? 		
		endloop
		10 charprint
	endloop
	 
	 printCells_SP SET
endword


word convayLoop

	SP print
	while
		
		0 SET
		0 "cls" system
		0 10 10 9 9 "GameOfLife" 10 9 9 9 9 "By:" 32 "B.B." 10 10 printstring
		0 "________________________________________________________" 10 10 10 printstring
		printCells
		updateCell			
		SLEEP_TIME sleep
		0 10  "________________________________________________________" 10 10 10 10 10 10 10 10 10 10 10 printstring
		0 ? counter
		while 
				counter 1 + ? counter
				counter 1000000 = if
					breakloop 
				end
		endloop
		
	endloop

	
endword


word main
0 SET 

10000000 ? seed
matrixInit

0 "color" 32 "2" system

CELL_CHAR ? mainMatrix925  
CELL_CHAR ? mainMatrix926  
CELL_CHAR ? mainMatrix927



CELL_CHAR ? mainMatrix120  
CELL_CHAR ? mainMatrix170  
CELL_CHAR ? mainMatrix220


CELL_CHAR ? mainMatrix1025  
CELL_CHAR ? mainMatrix1026  
CELL_CHAR ? mainMatrix1075
CELL_CHAR ? mainMatrix1076



convayLoop

endword

main

endscript . 


```

---



## Contributing

Contributions are welcome! If you'd like to contribute, please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature or bugfix.
3. Commit your changes.
4. Push your branch to your fork.
5. Submit a pull request.

---

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

## Acknowledgments

- Inspired by concatenative and stack-oriented languages like Forth and Factor.
- Special thanks to the [concatenative.org](https://concatenative.org/) community for their resources and inspiration.

---
