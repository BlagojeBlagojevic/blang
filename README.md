# blang

**EXTREMELY IMPORTANT! THIS LANGUAGE IS A WORK IN PROGRESS! ANYTHING CAN CHANGE AT ANY MOMENT WITHOUT ANY NOTICE! USE THIS LANGUAGE AT YOUR OWN RISK!

Blang is a [Concatenative](https://en.wikipedia.org/wiki/Concatenative_programming_language) [Stack-Oriented](https://en.wikipedia.org/wiki/Stack-oriented_programming) [Programming Language](https://en.wikipedia.org/wiki/Programming_language) for [Computers](https://en.wikipedia.org/wiki/Computer)

*(If you never heard about this kind of languages before check out [https://concatenative.org/](https://concatenative.org/))*

## Development Milestones

- [x] Compiled to a custom instruction set (BVM bytcode)
- [x] Conditional/selection statements
- [x] Loops 
- [x] [Turing-complete](./examples/celluarautomata/rule110.blang)
- [x] Crossplatform
- [ ] Strings support
- [ ] Functions
- [ ] Self hosted
- [ ] Statically typed
- [ ] Optimized
- [ ] Implement a vm as a custom cpu for fpga


## Examples

Hello, World:

```blang
10  let newline
33  let exc2
33  let exc1
33  let exc
100 let d
105 let l2
114 let r
111 let o1
87  let W
32  let space
111 let o
105 let l1
105 let l
101 let e
72  let H
0 let counter
while
	ptr H counter + ptrval
	charprint
	1 counter + let counter
	counter 14 = if
		breakloop
	end
endloop

endscript .
```

Simple program that prints all upper letters:

```blang
65 let a
while
	a charprint 
	89 < if
		breakloop
	end 
1 a + let a
endloop
endscript .
```

## Quick Start
```c
  gcc main.c
```
 
```console
Usage:  Compile -c <path to program> <path to save>
        Run -r <path to saved>
```

Compilation(-c) generates bytecode. 
Run -r <path to saved> 


## Language Reference

This is what the language supports so far. **Since the language is a work in progress everything in this section is the subject to change.**

### Literals

#### Integer

Currently an integer is a sequence of decimal digits. When an integer is encountered it is pushed onto the data stack for processing by the relevant operations.

Example:

```blang
10 20 +
```

The code above pushes 10 and 20 onto the data stack and sums them up with `+` operation.

#### String
TBD
#### Character

Currently a character is a only used as a var on heap 
Example:

```blang
  35 let taraba 
  32 let space 
  10 let newLine
```

### Intrinsics (Built-in Words)

#### Stack Manipulation

| Name    | Signature        | Description                                                                                       |
| ---     | ---              | ---                                                                                               |
| `dup`   | `a -- a a`       | duplicate an element on top of the stack.                                                         |
| `swap`  | `a b -- b a`     | swap 2 elements on the top of the stack. TBD                                                      |
| `drop`  | `a b -- a`       | drops the top element of the stack.                                                               |
| `print` | `a b -- a`       | print the element on top of the stack in a free form to stdout and remove it from the stack.      |
| `over`  | `a b -- a b a`   | copy the element below the top of the stack. TBD                                                  |
| `rot`   | `a b c -- b c a` | rotate the top three stack elements. TBD                                                          |

#### Comparison

| Name | Signature                              | Description                                                       |
| ---  | ---                                    | ---                                                               |
| `= ` | `[a: int] [b: int] -- [a == b : int]` | checks if two elements on top of the stack are equal.              |
| `!=` | `[a: int] [b: int] -- [a != b : int]` | checks if two elements on top of the stack are not equal. TBD      |
| `> ` | `[a: int] [b: int] -- [a > b  : int]` | applies the greater comparison on top two elements.                |
| `< ` | `[a: int] [b: int] -- [a < b  : int]` | applies the less comparison on top two elements.                   |
| `>=` | `[a: int] [b: int] -- [a >= b : int]` | applies the greater or equal comparison on top two elements. TBD   |
| `<=` | `[a: int] [b: int] -- [a <= b : int]` | applies the greater or equal comparison on top two elements. TBD   |

#### Arithmetic

| Name     | Signature                                        | Description                                                                                                              |
| ---      | ---                                              | ---                                                                                                                      |
| `+`      | `[a: int] [b: int] -- [a + b: int]`              | sums up two elements on the top of the stack.                                                                            |
| `-`      | `[a: int] [b: int] -- [a - b: int]`              | subtracts two elements on the top of the stack                                                                           |
| `*`      | `[a: int] [b: int] -- [a * b: int]`              | multiples two elements on top of the stack                                                                               |
| `/`      | `[a: int] [b: int] -- [a / b: int]`              | divide two elements on top of the stack
| `%`      | `[a: int] [b: int] -- [a % b: int]`              | mod two elements on top of the stack                                                                                      |

#### Bitwise TBD

| Name  | Signature                            | Description                   |
| ---   | ---                                  | ---                           |
| `shr` | `[a: int] [b: int] -- [a >> b: int]` | right **unsigned** bit shift. |
| `shl` | `[a: int] [b: int] -- [a << b: int]` | light bit shift.              |
| `or`  | `[a: int] [b: int] -- [a \| b: int]` | bit `or`.                     |
| `and` | `[a: int] [b: int] -- [a & b: int]`  | bit `and`.                    |
| `not` | `[a: int] -- [~a: int]`              | bit `not`.                    |

#### Memory

| Name         | Signature                      | Description                                                                                               |
| ---          | ---                            | ---                                                                                                       |
| `let`        | `<stack> let <name>  ` | Store a value from <sp> at the var addres. If var is not declared create the var on <stacksize - numofvars - 1>.  |  
| `arr`        | `arr <name> <size>` | Create a array on first free addres on stacksize - numofvars - 1.    |
| `ptr`        | `ptr <name>` | Push a mem addres of a var on top of the stack.                             |                         
| `ptrval`     | `<stack> ptrval` | Push a value from mem addres from top of the stack on top of the stack. |
| `letarr`     | `<stack> <stack> ptrval` | Store a value from <sp-1> to mem addres on sp                   |

####System

- `syscall<n>` - TBD
- `c functions` - TBD

### Control Flow

#### if-condition
If only if 
```html
<Comparison> if
  <body> 
<end>
```
If/else if 

```html
<Comparison> dup if
  <body> 
<end>
else
  <body>
end
```

Example: 

```blang
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
end
```

#### while-loop

Infinite loop:

```html
while 
   <body>
endloop
```
Condition loop use breakloop to exit a loop:

```html
while
  <body>
  <Comparison> if
    <body>
    breakloop 
  <end>
endloop
```

### Procedures
TBD


#### Inline Procedures

TBD

#### Procedure Pointers

TBD

### Constants

TBD

### Memory

TBD

#### Global Memory

TBD

### Type Checking

TBD


#### Types of blang 

TBD
