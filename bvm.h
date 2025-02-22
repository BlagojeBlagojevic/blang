#ifndef BVM_H
#define BVM_H
#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<stddef.h>
#include "device.h"

#define LOG(...)    fprintf(stdout, __VA_ARGS__)
//#define LOG(...) printf
#define LOGSTACK()        LOG("Value %d\n", (i16)vm.stack[SP].asI64);
#define ERROR_BREAK(...) {fprintf(stdout, __VA_ARGS__); exit(-1);}
#define PAUSE()        	 {char ch; scanf("%c", &ch);}

#define DEBUG 1
#define TRUE  1
#define FALSE 0

#define SYSCALLS
#define SYSTEM

#define FIRST(operand) (operand & 0x3f)
#define SECOND(operand) ((operand & 0xfc0) >> 6)



//#define LOG_STACK
enum {i = 0, f, u, ch};

//
typedef enum {
	POP,
	PUSH,
	PUSHF,
	PUSHIP,
	PUSHSP,
	PRINT,
	PRINTSTACK,
	PRINTSTRING,
	ADD,
	MUL,
	DEC,
	DIV,
	MOD,
	NOT,
	SHR,
	SHL,
	OR,
	AND,
	BNOT,
	DUP,
	OVER,
	ROT,
	SWAP_NO,
	IF,
	JMP,
	JMPT,
	JMPF,
	SETSP,
	SETSPSTACK,
	RESTORE,
	COPY,
	COPYSTACK,
	SWAP,
	MEM,
	MEMSTACK,
	NOP,
	HALT,
	INC,
#ifdef SYSCALLS
	WRITE,
	OPEN,
	CLOSE,
	DUPF,
	DUP2,
	EXIT,
	TRUNCATE,
	ISATTY,
	READ,
	SWAB,
	SLEEP,
#ifdef SYSTEM
	SYSTEMS,
#endif
#endif
	DRIVER, 
	NEWLINE,
	LABEL,
	FFI,
	END,


	} InstructionType;

static const char* instructionNames[] = {
	
	"POP",
	"PUSH",
	"PUSHF",
	"PUSHIP",
	"PUSHSP",
	"PRINT",
	"PRINTSTACK",
	"PRINTSTRING",
	"ADD",
	"MUL",
	"DEC",
	"DIV",
	"MOD",
	"NOT",
	"SHR",
	"SHL",
	"OR",
	"AND",
	"BNOT",
	"DUP",
	"OVER",
	"ROT",
	"SWAP_NO",
	"IF",
	"JMP",
	"JMPT",
	"JMPF",
	"SETSP",
	"SETSPSTACK",
	"RESTORE",
	"COPY",
	"COPYSTACK",
	"SWAP",
	"MEM",
	"MEMSTACK",
	"NOP",
	"HALT",
	"INC",
#ifdef SYSCALLS
	"WRITE",
	"OPEN",
	"CLOSE",
	"DUPF",
	"DUP2",
	"EXIT",
	"TRUNCATE",
	"ISATTY",
	"READ",
	"SWAB",
	"SLEEP",
#ifdef SYSTEM
	"SYSTEM",
#endif
#endif
	"DRIVER",

	"\n",
	"LABEL",
	"FFI",
	"END",
	};

//SIZE OF THE INSTRUCTION IS 8BYTE + 1BYTE
typedef struct {
	InstructionType type;
	Word operand;
	} Instruction;

#define MAX_SIZE_OF_PROGRAM 20024
#define MAX_LINE 100	




typedef struct {
	u8 isRuning;
	Stack stack;
	Instruction instruction[MAX_SIZE_OF_PROGRAM];
	u64 numOfInstructions;
	u64 IP;
	i64 jumpReg;
	} Bvm;

static inline Bvm  initBVM(void); //Dali da ide kao argument mozda
static inline void freeBvm(Bvm *bvm);
static inline void executeInstruction(Bvm *bvm);
static inline void loop(Bvm *bvm);


static inline u64 textToProgram(const char* name, Instruction *instrucion);
//static inline void dissasembler(Bvm *bvm);
static inline void programToBin(const char* name, Instruction *instruction, u64 numOfInstruction);
static inline void binToProgram(const char* name, Instruction *instruction);


//#define BVM_IMPLEMENTATION

#ifdef BVM_IMPLEMENTATION
//STACK OPERATIONS
//LETS DEFULT BE u64
static inline void stackPush(Stack *stack,i64 value) {
	if(stack->SP > STACK_CAPACITIY) {
		ERROR_BREAK("STACK OVERFLOW!!!\n");
		}
	stack->stack[stack->SP++]._asI64 =  value;
#ifdef LOG_STACK
	//LOG("STACK PUSH %d\n", stack->stack[stack->SP - 1]._asI64);
#endif
	}



static inline void stackPushF64(Stack *stack,f64 value) {
	if(stack->SP > STACK_CAPACITIY) {
		ERROR_BREAK("STACK OVERFLOW!!!\n");
		}
	stack->stack[stack->SP++]._asF64 =  value;
#ifdef LOG_STACK
	//LOG("STACK PUSH %f\n", stack->stack[stack->SP - 1]._asF64);
#endif
	}
static inline void stackPushWord(Stack *stack, Word value) {
	if(stack->SP > STACK_CAPACITIY) {
		ERROR_BREAK("STACK OVERFLOW!!!\n");
		}
#ifdef LOG_STACK
	//LOG("STACK PUSH %d\n", stack->stack[stack->SP - 1]._asI64);
#endif
	stack->stack[stack->SP++] = value;
	}


static inline Word stackPop(Stack *stack) {
	if(stack->SP <= 0) {
		ERROR_BREAK("STACK UNDERFLOW!!!\n");

		}
	stack->SP--;
#ifdef LOG_STACK
	//LOG("STACK POP %d\n", stack->stack[stack->SP]._asI64);
#endif
	return stack->stack[stack->SP];
	}

static inline void initStack(Stack *stack) {
	stack->SP = 0;
	stack->stack = malloc(sizeof(Word) * STACK_CAPACITIY);
	}

static inline void freeBvm(Bvm *bvm){
	free(bvm->stack.stack);	
}


static inline Bvm initBVM(void) {
	Bvm bvm;
	//PROBOBLY DYNAMIC ALLOCATION OF STACK AND INSTRUCTION WHEN FAIL
	//ALL mem FUNCTIONS
	memset(&bvm, 0, sizeof(bvm));
	bvm.isRuning = TRUE;
	initStack(&bvm.stack);
	//LOG("Init BVM\n");
	//LOG("\n SP = %lu\n", bvm.stack.SP);
	//LOG("\n IP = %lu\n", bvm.IP);
	bvm.numOfInstructions = 0;
    initDevices(&bvm.stack);

	return bvm;
	}

static inline void executeInstruction(Bvm *bvm) {

	Word a, b, c;
#ifdef LOG_STACK
	printf("\n- %s\n", instructionNames[bvm->instruction[bvm->IP].type]);
#endif
	//system("pause");
	switch(bvm->instruction[bvm->IP].type) {

		case POP: {
				a = stackPop(&bvm->stack);
				bvm->IP++;
				break;
				}

		case PUSH: {
				stackPushWord(&bvm->stack, bvm->instruction[bvm->IP].operand);
				bvm->IP++;
				break;
				}

		case PUSHF: {
				stackPushF64(&bvm->stack, bvm->instruction[bvm->IP].operand._asF64);
				bvm->IP++;
				break;
				}

		case PUSHIP: {
				stackPush(&bvm->stack, (i64)bvm->IP);
				bvm->IP++;
				break;
				}

		case PUSHSP: {
				stackPush(&bvm->stack, (i64)bvm->stack.SP);
				bvm->IP++;
				break;
				}

		case ADD: {
				a = stackPop(&bvm->stack);
				b = stackPop(&bvm->stack);
				c = bvm->instruction[bvm->IP].operand;
				i64 firstOperand = FIRST(c._asI64);
				i64 secondOperand = SECOND(c._asI64);
				if(firstOperand == f && secondOperand == f) {
					stackPushF64(&bvm->stack, (a._asF64 + b._asF64));
					}
				else if(firstOperand == f && secondOperand != f) {
					stackPushF64(&bvm->stack, (a._asF64 + (float)b._asI64));
					}
				else if(firstOperand != f && secondOperand == f) {
					stackPushF64(&bvm->stack, ((float)a._asI64 + b._asF64));
					}
				else {
					stackPush(&bvm->stack, (a._asI64 + b._asI64));
					};
				bvm->IP++;
				break;
				}
		case PRINT: {
				c = bvm->instruction[bvm->IP].operand;
				if(c._asU64 == u)
					LOG("%u", bvm->stack.stack[bvm->stack.SP - 1]._asU64);
				else if(c._asU64 == i) {
					LOG("%d", bvm->stack.stack[bvm->stack.SP - 1]._asI64);
					}
				else if(c._asU64 == f) {
					LOG("%lf", (float)bvm->stack.stack[bvm->stack.SP - 1]._asF64);
					}
				else if(c._asU64 == ch) {
					a = stackPop(&bvm->stack);
					LOG("%c", (char)a._asU64);

					//write(2, &bvm->stack.stack[bvm->stack.SP - 1]._asU64, 1);
					}

				//system("pause");
				bvm->IP++;
				break;
				}

		case PRINTSTACK: {
				LOG("\n-----------------------------------\n");
				LOG("Stack : \n");
				for(int i = bvm->stack.SP-1; i >= 0 ; i--) {
					LOG("%u\n", bvm->stack.stack[i]);
					}
				LOG("\n-----------------------------------\n");
				bvm->IP++;
				break;
				}

		case PRINTSTRING: {
				int counter = 0;
				for(counter = bvm->stack.SP - 1;  bvm->stack.stack[counter]._asU64 != 0 && counter > 0; counter--) {
					//printf("counter %d\n", counter);
					}
				//
				for(int i = counter; i < bvm->stack.SP; i++) {
					LOG("%c", bvm->stack.stack[i]);
					}
				bvm->IP++;
				break;
				}

		case MUL: {
				a = stackPop(&bvm->stack);
				b = stackPop(&bvm->stack);
				c = bvm->instruction[bvm->IP].operand;
				i64 firstOperand = FIRST(c._asI64);
				i64 secondOperand = SECOND(c._asI64);
				if(firstOperand == f && secondOperand == f) {
					stackPushF64(&bvm->stack, (float)((float)a._asF64 * (float)b._asF64));
					}
				else if(firstOperand == f && secondOperand != f) {
					stackPushF64(&bvm->stack, (float)((float)a._asF64 * (float)b._asI64));
					}
				else if(firstOperand != f && secondOperand == f) {
					stackPushF64(&bvm->stack, (float)((float)a._asI64 * b._asF64));
					}
				else {
					stackPush(&bvm->stack, (a._asI64 * b._asI64));
					};
				bvm->IP++;
				break;
				}

		case DEC: {
				a = stackPop(&bvm->stack);
				b = stackPop(&bvm->stack);
				c = bvm->instruction[bvm->IP].operand;
				i64 firstOperand = FIRST(c._asI64);
				i64 secondOperand = SECOND(c._asI64);
				if(firstOperand == f && secondOperand == f) {
					stackPushF64(&bvm->stack, (float)((float)a._asF64 - (float)b._asF64));
					}
				else if(firstOperand == f && secondOperand != f) {
					stackPushF64(&bvm->stack, (float)((float)a._asF64 - (float)b._asI64));
					}
				else if(firstOperand != f && secondOperand == f) {
					stackPushF64(&bvm->stack, (float)((float)a._asI64 - b._asF64));
					}
				else {
					stackPush(&bvm->stack, (a._asI64 - b._asI64));
					};
				bvm->IP++;
				break;
				}
		case DIV: {
				a = stackPop(&bvm->stack);
				b = stackPop(&bvm->stack);
				if(b._asI64 == 0) {
					ERROR_BREAK("ERROR DIVISION BY 0 !!!\n");
					}
				c = bvm->instruction[bvm->IP].operand;
				i64 firstOperand = FIRST(c._asI64);
				i64 secondOperand = SECOND(c._asI64);
				if(firstOperand == f && secondOperand == f) {
					stackPushF64(&bvm->stack, (float)((float)a._asF64 / (float)b._asF64));
					}
				else if(firstOperand == f && secondOperand != f) {
					stackPushF64(&bvm->stack, (float)((float)a._asF64 / (float)b._asI64));
					}
				else if(firstOperand != f && secondOperand == f) {
					stackPushF64(&bvm->stack, (float)((float)a._asI64 / b._asF64));
					}
				else {
					stackPush(&bvm->stack, (a._asI64 / b._asI64));
					};
				bvm->IP++;
				break;
				}
		case MOD: {
				a = stackPop(&bvm->stack);
				b = stackPop(&bvm->stack);
				c = bvm->instruction[bvm->IP].operand;
				i64 firstOperand = FIRST(c._asI64);
				i64 secondOperand = SECOND(c._asI64);
				if(firstOperand == f && secondOperand == f) {
					ERROR_BREAK("ERROR MOD TYPE FLOAT NOT ALLOWED !!!\n");
					}

				if(b._asI64 == 0 || a._asI64 == 0) {
					ERROR_BREAK("ERROR MOD BY 0 !!!\n");
					}
				stackPush(&bvm->stack, (i64)(a._asU64 % b._asU64));
				bvm->IP++;
				break;
				}
		case NOT: {
				a = stackPop(&bvm->stack);
				a._asI64 = !a._asI64;
				stackPushWord(&bvm->stack, a);
				bvm->IP++;
				break;
				}
		case SHR: {
				a = stackPop(&bvm->stack);
				b = stackPop(&bvm->stack);
				a._asU64 = a._asU64 >> b._asU64;
				stackPushWord(&bvm->stack, a);
				bvm->IP++;
				break;
				}
		case SHL: {
				a = stackPop(&bvm->stack);
				b = stackPop(&bvm->stack);
				a._asU64 = a._asU64 << b._asU64;
				stackPushWord(&bvm->stack, a);
				bvm->IP++;
				break;
				}
		case OR: {
				a = stackPop(&bvm->stack);
				b = stackPop(&bvm->stack);
				a._asU64 = a._asU64 | b._asU64;
				stackPushWord(&bvm->stack, a);
				bvm->IP++;
				break;
				}
		case AND: {
				a = stackPop(&bvm->stack);
				b = stackPop(&bvm->stack);
				a._asU64 = a._asU64 & b._asU64;
				stackPushWord(&bvm->stack, a);
				bvm->IP++;
				break;
				}
		case BNOT: {
				a = stackPop(&bvm->stack);
				a._asU64 = ~a._asU64;
				stackPushWord(&bvm->stack, a);
				bvm->IP++;
				break;
				}

		case DUP: {
				a = stackPop(&bvm->stack);
				stackPushWord(&bvm->stack, a);
				stackPushWord(&bvm->stack, a);
				bvm->IP++;
				break;
				}

		case OVER: {
				a = stackPop(&bvm->stack);
				b = stackPop(&bvm->stack);
				stackPushWord(&bvm->stack, b);
				stackPushWord(&bvm->stack, a);
				stackPushWord(&bvm->stack, b);
				bvm->IP++;
				break;
				}
		case ROT: {
				a = stackPop(&bvm->stack);
				b = stackPop(&bvm->stack);
				c = stackPop(&bvm->stack);
				stackPushWord(&bvm->stack, a);
				stackPushWord(&bvm->stack, c);
				stackPushWord(&bvm->stack, b);
				bvm->IP++;
				break;
				}

		case SWAP_NO: {
				a = stackPop(&bvm->stack);
				b = stackPop(&bvm->stack);
				stackPushWord(&bvm->stack, a);
				stackPushWord(&bvm->stack, b);
				bvm->IP++;
				break;
				}

		case IF: {

				a = stackPop(&bvm->stack);
				b = stackPop(&bvm->stack);
				c = bvm->instruction[bvm->IP].operand;
				if(c._asU64 == 0) {

					if(a._asI64 > b._asI64) {
						stackPush(&bvm->stack, 1);
						}

					else {
						stackPush(&bvm->stack, 0);
						}

					}

				else if(c._asU64 == 1) {

					if(a._asI64 < b._asI64) {
						stackPush(&bvm->stack, 1);
						}

					else {
						stackPush(&bvm->stack, 0);
						}

					}
				else if(c._asU64 == 2) {

					if(a._asI64 == b._asI64) {
						stackPush(&bvm->stack, 1);
						}

					else {
						stackPush(&bvm->stack, 0);
						}

					}
				else {};

				bvm->IP++;
				break;
				}


		case JMP: {
				//a = stackPop(&bvm->stack);
				b = bvm->instruction[bvm->IP].operand;
				bvm->IP = b._asU64;
				break;
				}

		case JMPT: {
				a = stackPop(&bvm->stack);
				c =  bvm->instruction[bvm->IP].operand;

				if(a._asU64) {
					bvm->IP = c._asU64;
					bvm->jumpReg = 1;
					//stackPush(&bvm->stack, a._asI64);
					}
				else {
					//stackPush(&bvm->stack, a._asI64);
					bvm->IP++;
					}
				break;
				}

		case JMPF: {
				a = stackPop(&bvm->stack);
				c =  bvm->instruction[bvm->IP].operand;

				if(!a._asU64) {
					bvm->IP = c._asU64;
					//stackPush(&bvm->stack, a._asI64);
					}
				else {
					//stackPush(&bvm->stack, a._asI64);
					bvm->IP++;
					}
				break;
				}


		case SETSP: {
				a = bvm->instruction[bvm->IP].operand;
				if(a._asU64 > STACK_CAPACITIY) {
					ERROR_BREAK("STACK OVERFLOW ACCES!!!\n");
					}
				bvm->stack.SP = a._asI64;
				bvm->IP++;
				break;
				}

		case SETSPSTACK: {
				a = stackPop(&bvm->stack);
				if(a._asU64 >= STACK_CAPACITIY) {
					ERROR_BREAK("STACK OVERFLOW ACCES!!!\n");
					}
				bvm->stack.SP = a._asI64;
				bvm->IP++;
				break;
				}


		case RESTORE: {
				//s[sp] -> sp, operand ->  sp
				a = stackPop(&bvm->stack);
				b = bvm->instruction[bvm->IP].operand;
				if(b._asU64 > STACK_CAPACITIY) {
					ERROR_BREAK("STACK OVERFLOW ACCES!!!\n");
					}
				bvm->stack.SP = b._asI64;
				bvm->IP = a._asU64;
				break;
				}


		case COPY: {
				//a = stackPop(&bvm->stack);
				b = bvm->instruction[bvm->IP].operand;
				c = bvm->stack.stack[b._asU64];
				stackPushWord(&bvm->stack, c);
				//bvm->stack.stack[b._asU64] = a;
				bvm->IP++;
				break;

				}

		case COPYSTACK: {
				a = stackPop(&bvm->stack); //memadress
				//b = bvm->instruction[bvm->IP].operand;
				c = bvm->stack.stack[a._asU64]; //value of memadress
				//printf("%f", c._asF64);
				//system("pause");
				stackPushWord(&bvm->stack, c);
				bvm->IP++;
				break;

				}
		case SWAP: {
				a = stackPop(&bvm->stack);
				b = bvm->instruction[bvm->IP].operand;
				c = bvm->stack.stack[b._asU64];
				stackPushWord(&bvm->stack, c);
				bvm->stack.stack[b._asU64] = a;
				bvm->IP++;
				break;
				}

		case MEM: {
				a = stackPop(&bvm->stack);
				b = bvm->instruction[bvm->IP].operand;
				c = bvm->stack.stack[b._asU64];
				stackPushWord(&bvm->stack, a);
				memcpy(&bvm->stack.stack[b._asU64], &a, sizeof(Word));
				bvm->IP++;
				break;
				}
		case MEMSTACK: {
				a = stackPop(&bvm->stack); // memaddres
				b = stackPop(&bvm->stack); // value to print
				stackPushWord(&bvm->stack, b);
				//bvm->stack.stack[a._asI64]._asI64 = 100;
				memcpy(&bvm->stack.stack[a._asI64], &b, sizeof(Word));
				bvm->IP++;
				break;

				}

		case NOP: {
				bvm->IP++;
				break;
				}

		case HALT: {
				PAUSE();
				bvm->IP++;
				break;
				}

		case INC: {
				a = stackPop(&bvm->stack);
				b = bvm->instruction[bvm->IP].operand;
				stackPush(&bvm->stack, (a._asI64 + b._asI64));
				bvm->IP++;
				break;
				}

			//SYSCALL
#ifdef SYSCALLS
		case WRITE: {
				a = stackPop(&bvm->stack);//size
				b = stackPop(&bvm->stack);//data
				c = stackPop(&bvm->stack);//fd
				//printf("str: %d\n", &bvm->stack.stack[b._asI64]);
				u8 bytes[a._asI64];
				memset(bytes, 0, sizeof(u8)*a._asI64);
				int counter = 0;

				//TBD Order of a strings when pushed on stack
				for(int i = 0; i < a._asI64 ; i++) {
					bytes[i] = (u8)((bvm->stack.stack[b._asI64+i])._asU64);
					}
				//printf("str: %s\n", bytes);
				const i64 temp = write((int)c._asI64, bytes, (u32)a._asU64);
				stackPush(&bvm->stack, temp);
				bvm->IP++;
				break;
				}
		//WINDOWS PROBLEM SO I KINDA MAKE THIS TYPE OF A HACK
		//USE fopen and then i get a fd of a file it writes to
		// 
		case OPEN:{
				a = stackPop(&bvm->stack);//mode
				//LOG("\nflags : %d  \n", a._asI64);
				char bytes[100];
				memset(bytes, '\0', sizeof(u8)*100);
				//LOG("path : ");
				int counter = 0, counterB = 0;
					for(counter = bvm->stack.SP;  bvm->stack.stack[counter]._asU64 != 0 && counter > 0; counter--) {
					//printf("counter %d\n", counter);
					}
				for(int i = counter; i < bvm->stack.SP; i++) {
					bytes[counterB++] = (char)bvm->stack.stack[i]._asU64;
					if(counterB == 99){
						ERROR_BREAK("We have excide a size of a path in open syscall!!!\n");
					}
					}
				//for(int i = 1; i < counterB; i++) {
				//		LOG("%c", bytes[i]);
				//	}
				//LOG("\n");
				//char mode[3];
				//memset(mode, '\0', sizeof(char)*3);
				bvm->stack.SP-=counterB;
				
				if(a._asU64 == 2){
					FILE	*f = fopen(&bytes[1], "r");
					stackPush(&bvm->stack, fileno(f));
				}
				else if(a._asU64 == 1){
					FILE *f = fopen(&bytes[1],  "w");
					stackPush(&bvm->stack, fileno(f));
				}
				
				 
				bvm->IP++;
				break;
		
		}
		case CLOSE: {
				a = stackPop(&bvm->stack);//fd
				const int temp = close(a._asI64);
				stackPush(&bvm->stack, (i64)temp);
				bvm->IP++;
				break;
				}

		case DUPF: {
				a = stackPop(&bvm->stack);//size
				const int temp = dup(a._asI64);
				stackPush(&bvm->stack, (i64)temp);
				bvm->IP++;
				break;
				}
		case DUP2: {
				a = stackPop(&bvm->stack);//size
				b = stackPop(&bvm->stack);
				const int temp = dup2(b._asI64, a._asI64);
				stackPush(&bvm->stack, (i64)temp);
				bvm->IP++;
				break;
				}
		case EXIT: {
				a = stackPop(&bvm->stack);
				_exit(a._asI64);
				bvm->IP++;
				break;
				}
		case TRUNCATE: {
				a = stackPop(&bvm->stack);//ofset
				b = stackPop(&bvm->stack);//fd
				const i64 temp = ftruncate(b._asI64, a._asI64);
				stackPush(&bvm->stack, temp);
				bvm->IP++;
				break;
				}

		case ISATTY: {
				a = stackPop(&bvm->stack);//ofset
				const i64 temp = isatty(a._asI64);
				stackPush(&bvm->stack, temp);
				bvm->IP++;
				break;
				}
		case READ: {
				a = stackPop(&bvm->stack);//size
				b = stackPop(&bvm->stack);//data
				c = stackPop(&bvm->stack);//fd
				//printf("str: %d\n", &bvm->stack.stack[b._asI64]);
				u8 bytes[a._asI64];
				//memset(bytes, 0, sizeof(u8)*a._asI64);
				const i64 temp = read((int)c._asI64, bytes, (u32)a._asU64);
				int counter = 0;

				//TBD Order of a strings when pushed on stack
				for(int i = 0; i < a._asI64 ; i++) {
					bvm->stack.stack[b._asI64+i]._asU64 = (u64)bytes[i];
					}
				//printf("str: %s\n", bytes);

				stackPush(&bvm->stack, temp);
				bvm->IP++;
				break;
				}

		case SWAB: {
				a = stackPop(&bvm->stack);//size
				b = stackPop(&bvm->stack);//dest
				c = stackPop(&bvm->stack);//source
				//printf("str: %d\n", &bvm->stack.stack[b._asI64]);
				swab((char*)&bvm->stack.stack[b._asI64]._asU64, (char*)&bvm->stack.stack[a._asI64], a._asI64 * sizeof(Word));
				bvm->IP++;
				break;
				}

		case SLEEP: {
				a = stackPop(&bvm->stack);
				const u64 temp = sleep(a._asU64);
				stackPush(&bvm->stack, (i64)temp);
				bvm->IP++;
				break;
				}
#ifdef SYSTEM
		case SYSTEMS: {

				int counter = 0, counter1 = 0;
				char bytes[100] = {'\0'};
				for(counter = bvm->stack.SP - 1;  bvm->stack.stack[counter]._asU64 != 0 && counter > 0; counter--) {

					}
				for(int i = counter; i < bvm->stack.SP; i++) {
					bytes[counter1++] = (char)bvm->stack.stack[i]._asU64;
					//printf("%c\n", bytes[counter1-1]);
					if(counter1 == 99) {
						break;
						}
					}
				bytes[counter1] = '\0';
				const int temp = system(&bytes[1]);
				stackPush(&bvm->stack, temp);
				bvm->IP++;
				break;
				}
#endif
#endif

		case DRIVER:{
				b = bvm->instruction[bvm->IP].operand;
				devices[b._asI64].func_pointer(&bvm->stack);
				bvm->IP++;
				break;
		}

		case END: {
				bvm->isRuning = FALSE;
				//LOG("\n\nExiting VM\n\n!!!");
				bvm->IP++;
				break;
				}

		default: {
				LOG("Instruction is not implemeted\n");
				bvm->IP++;
				break;
				}

		}


	//return bvm;
	}

static inline void loop(Bvm *bvm) {

	while(bvm->isRuning) {
		executeInstruction(bvm);
		//	PAUSE();
		}


	}


//PARSER WORD BY WORD
//DEPRICATED See blang repo for how to program this vm
static inline u64 textToProgram(const char* name, Instruction *instrucion) {
	LOG("Depricated : SEE https://github.com/BlagojeBlagojevic/blang/\n");
	PAUSE();
	LOG("%s\n\n", name);
	FILE *f = fopen(name, "rb");
	u64 counterInstruction = 0;
	//if(f == NULL)
	//	ERROR_BREAK("FILE NOT EXISTING!!!\n");
	while(!feof(f)) {
		char instructionText[60];
		char token[60], textOperand[10];
		//memset(textOperand, 0, sizeof(char)*10);
		u8 counter = 0, counterOperand = 0;

		fgets(instructionText, 60, f);
		//sprintf()
		//GET WORD BY WORD

		while(instructionText[counter] != ' ' && counter < 59  ) {
			token[counter] = instructionText[counter];
			counter++;
			}
		token[counter] = '\0';
		LOG("Instruction %s\n", token);
		//system("pause");
		counter++;
		while(instructionText[counter] != ' '  && counter < 59
		      && instructionText[counter] != '\0' &&  counterOperand < 10) {
			textOperand[counterOperand] = instructionText[counter];
			counter++;
			counterOperand++;
			}
		//LOG("Operand %s \n", textOperand);

		//INSTRUCTION HANDLING BETHER PARSER
		for(InstructionType inst = PUSH; inst <= END; inst++) {
			if(!strcmp(instructionNames[inst], token)) {
				instrucion[counterInstruction].type = inst;
				if(inst == PUSHF) {
					instrucion[counterInstruction].operand._asF64 = (f64)atof(textOperand);
					LOG("operand %f\n", instrucion[counterInstruction].operand._asF64);
					}

				else if(inst == NEWLINE || inst == LABEL) {
					//DO NOTING IF NEW LINE
					continue;
					}
				//Todo ADD >=, <=
				else if(inst == IF) {
					if(textOperand[0] == '>') {
						instrucion[counterInstruction].operand._asU64 = 0;
						LOG("operand >\n");
						}
					else if(textOperand[0] == '<') {
						instrucion[counterInstruction].operand._asU64 = 1;
						LOG("operand <\n");
						}
					else if(textOperand[0] == '=') {
						instrucion[counterInstruction].operand._asU64 = 2;
						LOG("operand ==\n");
						}

					}


				else {
					instrucion[counterInstruction].operand._asI64 = (i64)atoi(textOperand);
					LOG("operand %d\n", instrucion[counterInstruction].operand._asI64);
					}

				counterInstruction++;
				//LOG("counterInstruction %u", counterInstruction);
				break;
				}

			}

		}

	fclose(f);
	return counterInstruction;
	}




static inline void programToBin(const char* name, Instruction *instruction, u64 numOfInstruction) {

	FILE *f = fopen(name, "wb+");
	if(f == NULL) {
		ERROR_BREAK("FILE ERROR !!!\n");
		}
	fwrite(instruction, sizeof(Instruction), numOfInstruction+1, f);
	fclose(f);


	}


static inline void binToProgram(const char* name, Instruction *instruction) {

	FILE *f = fopen(name, "rb");
	if(f == NULL) {
		ERROR_BREAK("FILE ERROR !!!\n");
		}
	fread(instruction, sizeof(Instruction), MAX_SIZE_OF_PROGRAM, f);
	fclose(f);
	}

static inline void bitToDisasemly(const char* name, Instruction *instruction){
	FILE *f = fopen(name, "rb");
	fread(instruction, sizeof(Instruction), MAX_SIZE_OF_PROGRAM, f);

	fclose(f);
	f = fopen("dissasembled.txt", "w");
	fseek(f, 0, SEEK_SET);
	i64 counter = 0;
	while(instruction[counter].type != END){
		if(instruction[counter].type == DRIVER){
			//device_name[]
			const i64 driverNum = instruction[counter].operand._asI64;
			fwrite("EXTERN  ", sizeof(char), strlen("EXTERN  "), f);
			fwrite(device_name[driverNum], sizeof(char), strlen(device_name[driverNum]), f);
			fwrite("\n", sizeof(char), 1, f);
		}
		//TBD FLOATS
		else{
		const i64 instNum = instruction[counter].type;
		fwrite(instructionNames[instNum], sizeof(char), strlen(instructionNames[instNum]), f);
		char num[20];
		//meset(num, '\0', sizeof(char) * 20);
		snprintf(num, 20, "  %d\n",instruction[counter].operand._asI64);
		fwrite(num, sizeof(char), strlen(num), f);
		
	}
	counter++;
	}
	fclose(f);
}


#endif

#endif
