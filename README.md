

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
gcc -O3 lex.c parser.c main.c -o main.out
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

### Stack Manipulation

| Name    | Signature        | Description                                                                                       |
| ---     | ---              | ---                                                                                               |
| `dup`   | `a -- a a`       | Duplicate an element on top of the stack.                                                         |
| `swap`  | `a b -- b a`     | Swap 2 elements on the top of the stack.                                                          |
| `drop`  | `a --`           | Remove the top element from the stack.                                                            |
| `print` | `a --`           | Print the element on top of the stack and remove it.                                              |
| `over`  | `a b -- a b a`   | Copy the element below the top of the stack.                                                      |
| `rot`   | `a b c -- b c a` | Rotate the top three stack elements.                                                              |

### Arithmetic

| Name     | Signature                                        | Description                                                                                                              |
| ---      | ---                                              | ---                                                                                                                      |
| `+`      | `[a: int] [b: int] -- [a + b: int]`              | Sum up two elements on the top of the stack.                                                                             |
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

- `syscall`:

| Name       | Signature                        | Description                                                                 |
|------------|----------------------------------|-----------------------------------------------------------------------------|
| `write`    | `fd ptr len -- result`           | Write `len` bytes from `ptr` to file descriptor `fd`.                       |
| `close`    | `fd -- result`                   | Close file descriptor `fd`.                                                 |
| `exit`     | `status --`                      | Terminate the program with exit `status`.                                   |
| `truncate` | `fd length -- result`            | Truncate file descriptor `fd` to `length` bytes.                            |
| `isatty`   | `fd -- result`                   | Check if `fd` refers to a terminal. Returns 1 if true, 0 otherwise.         |
| `read`     | `fd ptr len -- bytes_read`       | Read up to `len` bytes from `fd` into `ptr`. Returns number of bytes read.  |
| `sleep`    | `seconds --`                     | Sleep for `seconds` seconds.                                                |
| `system`   | `command_ptr -- exit_code`       | Execute shell command specified by `command_ptr`.                           |
| `dupF`     | `fd -- new_fd`                   | Duplicate file descriptor `fd`.                                             |
| `dup2`     | `old_fd new_fd -- new_fd`        | Duplicate `old_fd` to `new_fd`, closing `new_fd` first if open.             |
| `halt`     | `--`                             | Immediately halt the program execution.                                     |

I'll help you create a section about FFI (Foreign Function Interface) for your Blang README. Here's a suggested addition that fits with your existing structure:

---

## Foreign Function Interface (FFI)

Blang provides FFI capabilities through its VM implementation, allowing interaction with C libraries and system calls. This enables low-level operations and integration with existing libraries.

### Basic Usage
FFI operations are handled through the `DRIVER` instruction and system call intrinsics. Here's how it works:

### Device Interface Example
The VM supports device registration for hardware/software interactions. Here's a C-side device implementation template:

```c
#ifndef DEVICE_H
#define DEVICE_H
//#define BVM_IMPLEMENTATION
//#define SDL_DEVICE
#include "bvm_type.h"
#ifdef SDL_DEVICE
#include<SDL2/SDL.h>

typedef enum {
    INIT_SDL,
    CHECK_EVENT,
    PUSH_EVENT_FROM_QUEUE,
    RENDERER_CLEAR,
    SET_RENDER_DRAW_COLOR,
    SDL_RENDER_FILL_RECT,
    SDL_RENDER_PRESENT,
    SDL_DELAY,
    SDL_MOUSE,
    NUM_OF_DEVICES
}DeviceType;

static const char* device_name[] = {
    "initSdl",
    "checkEvent",
    "pushEvent",
    "renderClear",
    "drawColor",
    "renderRect",
    "renderPresent",
    "sdlDelay",
    "sdlMouse",
    "NUM_OF_DEVICES",
};

typedef struct device{
    void (*func_pointer)(Stack*);
    int stackSize; 
}Devices;
 
static volatile Devices devices[NUM_OF_DEVICES];

static SDL_Window *window;
static SDL_Renderer *renderer;

#define WIDTH  800
#define HEIGHT 600

//(WIDTH HEIGHT --)
static inline void initSdl(Stack *stack){
    //Stack *s;
    printf("Inited sdl!!!\n");
    Word width = stackPop(stack);
    Word height = stackPop(stack);
    printf("Width: %d Height %d\n", width._asI64, height._asI64);
    SDL_CreateWindowAndRenderer(width._asI64, height._asI64, NULL, &window, &renderer);
    //while(1){

    //}
    return;
}

//(-- key event.type)
static inline void pushEventFromQueue(Stack *stack){
    //Word typeOfEvent = stackPop(stack);
    SDL_Event event;
    if(SDL_PollEvent(&event)){
        stackPush(stack, (i64)event.key.keysym.sym);
        stackPush(stack, (i64)event.type);
 
    }
    else{
        stackPush(stack, -1);
        stackPush(stack, -1); //NO EVENT IN A QUEUE
    }
}

//(err +)
static inline void renderClear(Stack *stack){
    int err = SDL_RenderClear(renderer);//MAYBE PUSHES ERROR CODE 
    stackPush(stack, (i64)err);
}


//(Event +)
static inline void checkEventInLoop(Stack *stack){
    Word typeOfEvent = stackPop(stack);
    
    SDL_Event event;
    //printf("Event loop\n");
    if(SDL_PollEvent(&event)){
        if(event.type == typeOfEvent._asI64){
            //MAYBE SEND DATA TO A STACK
            exit(-1);

        }
    }
}
//(a, g, b, r ----)
static inline void drawColor(Stack *stack){
    Word a = stackPop(stack);
    Word b = stackPop(stack);
    Word g = stackPop(stack);
    Word r = stackPop(stack);
    SDL_SetRenderDrawColor(renderer, (u8)r._asI64, (u8)g._asI64, (u8)b._asI64, (u8)a._asI64);
}
//(h,w,y,x ----)
static inline void rednerRect(Stack *stack){
    Word h = stackPop(stack);
    Word w = stackPop(stack);
    Word y = stackPop(stack);
    Word x = stackPop(stack);
    SDL_Rect r;
    r.h =  (int)h._asI64; 
    r.w =  (int)w._asI64;
    r.y =  (int)y._asI64; 
    r.x =  (int)x._asI64;
    if(SDL_RenderFillRect(renderer, &r) != 0){
        printf("Error in drawing of a rectangle\n");
    }
    //SDL_Delay(1);
}
//(--)
static inline void renderPresent(Stack *stack){
    //printf("Drawing\n");
    SDL_RenderPresent(renderer);
    //printf("Drawing\n"); 
}
//DELAY(dealy --) 
static inline void sdlDelay(Stack *stack){
    Word a = stackPop(stack);
    SDL_Delay((u32)a._asI64);
}
//MOUSE(-- x, y)
static inline void sdlMouse(Stack *stack){
    int X, Y;
    
    SDL_GetMouseState(&X, &Y);
    stackPush(stack, (i64)X);
    stackPush(stack, (i64)Y);
    
}



static inline void initDevices(Stack *stack){
    printf("Devices init\n");
   //INIT SDL
    devices[INIT_SDL].func_pointer = &initSdl;
    devices[INIT_SDL].stackSize = -2;
   //CHECK EVENT
    devices[CHECK_EVENT].func_pointer = &checkEventInLoop;
    devices[CHECK_EVENT].stackSize = -1;
   //PUSH EVENT FROM QUUES
    devices[PUSH_EVENT_FROM_QUEUE].func_pointer = &pushEventFromQueue;
    devices[PUSH_EVENT_FROM_QUEUE].stackSize = +2;
   //CLEAR RENDERER
    devices[RENDERER_CLEAR].func_pointer = &renderClear;
    devices[RENDERER_CLEAR].stackSize = +1;
   //SET DRAW COLOR
    devices[SET_RENDER_DRAW_COLOR].func_pointer = &drawColor;
    devices[SET_RENDER_DRAW_COLOR].stackSize = -4;
  //DRAW RECR
    devices[SDL_RENDER_FILL_RECT].func_pointer = &rednerRect;
    devices[SDL_RENDER_FILL_RECT].stackSize = -4;
  //RENDER *renderer
    devices[SDL_RENDER_PRESENT].func_pointer = &renderPresent;
    devices[SDL_RENDER_PRESENT].stackSize = 0;
  //DELAY Sdl_Delay
    devices[SDL_DELAY].func_pointer = &sdlDelay;
    devices[SDL_DELAY].stackSize = -1;

  //MOUSE Sdl_Delay
    devices[SDL_MOUSE].func_pointer = &sdlMouse;
    devices[SDL_MOUSE].stackSize = +2;
    
    
}
#endif
```


### Example: SDL Integration
```forth
0
word SDL_ERROR
    0 swap < if
    0 "SdlError" 10 exit
    end
endword

word w 119 endword
word a 97  endword
word s 115 endword 
word d 100 endword


word SDL_QUIT            256 endword
word SDL_KEYDOWN         768  endword
word SDL_MOUSEMOVED      1024 endword
word SDL_MOUSEBUTTONDOWN 1025 endword       
word SDL_MOUSEBUTTONUP   1026 endword       
word SDL_MOUSEWHEEL      1027 endword


100 ? x drop
100 ? y drop

10 
600 800 initSdl
while
    pushEvent dup SDL_QUIT = if
        0 10 "Quiting" printstring 10 exit
    end
    dup SDL_KEYDOWN = if
        swap 
        dup  w = if y -5 + ? y drop end
        dup  s = if y  5 + ? y drop end
        dup  a = if x -5 + ? x drop end
        dup  d = if x  5 + ? x drop end
    end
	dup SDL_MOUSEBUTTONDOWN = if
		sdlMouse
		? y drop
		? x drop
	end
    drop
    drop 
    255 0 0 0   drawColor 
    x y 30 30   renderRect 
    0 0 0 0     drawColor
    renderPresent
    renderClear SDL_ERROR
    
    

endloop
10 printstack
endscript .
```

### Important Notes
1. The FFI system is **experimental** and subject to change
2. Memory management between Blang and C is manual
3. Type conversions must be handled explicitly
4. Device IDs correspond to their registration order in `devices[]`

See the [device.h](device.h) implementation for more examples of FFI integration with SDL.


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
35 ? MAX_Y_POS 
word BACKGROUND_CHAR 32 endword  
word CELL_CHAR 35 endword   
word SLEEP_TIME 1 endword

arr checkCoords 100

arr mainMatrix 1800



arr neighborCountBuffer 1800



word increment 1 + endword

word matrixIndex  MAX_X_POS updateCell_y * updateCell_x +   endword
word matrixIndexP MAX_X_POS printCells_y * printCells_x +   endword
word matrixIndexC MAX_X_POS tempY        *        tempX +   endword

word matrixInit
	0 ? printCells_x
	0 ? printCells_y
	SP ? printCells_SP
	
	while MAX_Y_POS printCells_y loopBreak
		printCells_y 1 + ? printCells_y
		
		0 ? printCells_x
		
		while printCells_x MAX_X_POS  loopBreak
			printCells_x 1 + ? printCells_x
			BACKGROUND_CHAR
			& mainMatrix0 matrixIndexP + ??  
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
		0 10 10 9 9 "GameOfLife" 10 9 9 9 9 "By:" 32 "B.B." printstring
		printCells
		updateCell			
		SLEEP_TIME sleep
		
	endloop

	
endword



0 SET 


matrixInit

0 "color" 32 "3" system

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
