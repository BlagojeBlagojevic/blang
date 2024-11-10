#ifndef BVM_H
#define BVM_H
#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define LOG(...)         fprintf(stdout, __VA_ARGS__)
#define LOGSTACK()        LOG("Value %d\n", (i16)vm.stack[SP].asI64);
#define ERROR_BREAK(...) {fprintf(stdout, __VA_ARGS__); exit(-1);}
#define PAUSE()        	 system("pause")

#define DEBUG 1
#define TRUE  1
#define FALSE 0

//#define LOG_STACK
enum {i, f, u, ch};

//


typedef  uint8_t  u8;
typedef  uint16_t u16;
typedef  uint32_t u32;
typedef  uint64_t u64;
typedef  int16_t  i16;
typedef  int32_t  i32;
typedef  int64_t  i64;
typedef  float    f32;
typedef  double   f64;

typedef union {

	u64 _asU64;
	i64 _asI64;
	f64 _asF64;
	void *_asP;

	} Word;

typedef enum {

	PUSH,
	PUSHF,
	PUSHIP,
	POP,
	PRINT,
	ADD,
	MUL,
	DEC,
	DIV,
	DUP,
	IF,
	JMP,
	JMPT,
	JMPF,
	SETSP,
	COPY,
	SWAP,
	MEM,
	NOP,
	HALT,
	INC,
	NEWLINE,
	LABEL,
	END,


	} InstructionType;

const char* instructionNames[] = {

	"PUSH",
	"PUSHF",
	"PUSHIP",
	"POP",
	"PRINT",
	"ADD",
	"MUL",
	"DEC",
	"DIV",
	"DUP",
	"IF",
	"JMP",
	"JMPT",
	"JMPF",
	"SETSP",
	"COPY",
	"SWAP",
	"MEM",
	"NOP",
	"HALT",
	"INC",
	"\n",
	"LABEL",
	"END",
	};

//SIZE OF THE INSTRUCTION IS 8BYTE + 1BYTE
typedef struct {
	InstructionType type;
	Word operand;
	} Instruction;


#define STACK_CAPACITIY     102400
#define MAX_SIZE_OF_PROGRAM 10024
#define MAX_LINE 100
typedef struct {
	Word *stack;
	i64  SP;
	} Stack;

static inline void stackPush(Stack *stack,i64 value);
static inline void stackPushF64(Stack *stack,f64 value);
static inline void stackPushWord(Stack *stack, Word value);
static inline Word stackPop(Stack *stack);
static inline void initStack(Stack *stack);
typedef struct {
	u8 isRuning;
	Stack stack;
	Instruction instruction[MAX_SIZE_OF_PROGRAM];
	u64 numOfInstructions;
	u64 IP;
	i64 jumpReg;
	} Bvm;

static inline Bvm  initBVM(void);
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
	LOG("STACK PUSH %d\n", stack->stack[stack->SP - 1]._asI64);
#endif
	}



static inline void stackPushF64(Stack *stack,f64 value) {
	if(stack->SP > STACK_CAPACITIY) {
		ERROR_BREAK("STACK OVERFLOW!!!\n");
		}
	stack->stack[stack->SP++]._asF64 =  value;
#ifdef LOG_STACK
	LOG("STACK PUSH %f\n", stack->stack[stack->SP - 1]._asF64);
#endif
	}
static inline void stackPushWord(Stack *stack, Word value) {
	if(stack->SP > STACK_CAPACITIY) {
		ERROR_BREAK("STACK OVERFLOW!!!\n");
		}
#ifdef LOG_STACK
	LOG("STACK PUSH %d\n", stack->stack[stack->SP - 1]._asI64);
#endif
	stack->stack[stack->SP++] = value;
	}


static inline Word stackPop(Stack *stack) {
	if(stack->SP <= 0) {
		ERROR_BREAK("STACK UNDERFLOW!!!\n");

		}
	stack->SP--;
#ifdef LOG_STACK
	LOG("STACK POP %d\n", stack->stack[stack->SP]._asI64);
#endif
	return stack->stack[stack->SP];
	}

static inline void initStack(Stack *stack){
	stack->SP = 0;
	stack->stack = malloc(sizeof(Word) * STACK_CAPACITIY);
}



static inline Bvm initBVM(void) {
	Bvm bvm;

	//PROBOBLY DYNAMIC ALLOCATION OF STACK AND INSTRUCTION WHEN FAIL
	//ALL mem FUNCTIONS

	memset(&bvm, 0, sizeof(bvm));
	bvm.isRuning = TRUE;
	initStack(&bvm.stack);
	LOG("Init BVM\n");
	LOG("\n SP = %lu\n", bvm.stack.SP);
	LOG("\n IP = %lu\n", bvm.IP);
	bvm.jumpReg = 0;

	return bvm;


	}

static inline void executeInstruction(Bvm *bvm) {

	static Word a, b, c;


	//system("pause");
	switch(bvm->instruction[bvm->IP].type) {

		case PUSH: {
				stackPush(&bvm->stack, bvm->instruction[bvm->IP].operand._asI64);
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


		case POP: {
				a = 	stackPop(&bvm->stack);
				bvm->IP++;
				break;
				}
		case ADD: {
				a = stackPop(&bvm->stack);
				b = stackPop(&bvm->stack);
				c = bvm->instruction[bvm->IP].operand;
				if(c._asU64 == u)
					stackPush(&bvm->stack, (a._asU64 + b._asU64));
				else if(c._asU64 == i) {
					stackPush(&bvm->stack, (a._asI64 + b._asI64));
					}
				else if(c._asU64 == f) {
					stackPushF64(&bvm->stack, (a._asF64 + b._asF64));
					}
				else {}; //PTR MAYBE
				bvm->IP++;
				break;

				}
		case PRINT: {
				c = bvm->instruction[bvm->IP].operand;
				if(c._asU64 == u)
					LOG("PRINT %u\n\n", bvm->stack.stack[bvm->stack.SP - 1]._asU64);
				else if(c._asU64 == i) {
					LOG("PRINT %d\n\n", bvm->stack.stack[bvm->stack.SP - 1]._asI64);
					}
				else if(c._asU64 == f) {
					LOG("PRINT %f\n\n", bvm->stack.stack[bvm->stack.SP - 1]._asF64);
					}
				else if(c._asU64 == ch) {
					LOG("%c", (char)bvm->stack.stack[bvm->stack.SP - 1]._asU64);
					}


				bvm->IP++;
				break;
				}


		case MUL: {

				a = stackPop(&bvm->stack);
				b = stackPop(&bvm->stack);
				c = bvm->instruction[bvm->IP].operand;
				if(c._asU64 == u)
					stackPush(&bvm->stack, (a._asU64 * b._asU64));
				else if(c._asU64 == i) {
					stackPush(&bvm->stack, (a._asI64 * b._asI64));
					}
				else if(c._asU64 == f) {
					stackPushF64(&bvm->stack, (a._asF64 * b._asF64));
					}
				else {}; //PTR MAYBE
				bvm->IP++;


				break;
				}

		case DEC: {
				a = stackPop(&bvm->stack);
				b = stackPop(&bvm->stack);
				c = bvm->instruction[bvm->IP].operand;
				if(c._asU64 == u)
					stackPush(&bvm->stack, (a._asU64 - b._asU64));
				else if(c._asU64 == i) {
					stackPush(&bvm->stack, (a._asI64 - b._asI64));
					}
				else if(c._asU64 == f) {
					stackPushF64(&bvm->stack, (a._asF64 - b._asF64));
					}
				else {}; //PTR MAYBE
				bvm->IP++;
				break;
				}
		case DIV: {
				a = stackPop(&bvm->stack);
				b = stackPop(&bvm->stack);
				if(b._asI64 == 0) {
					ERROR_BREAK("ERROR DIVISION BY 0 !!!\n");
					}
				Word c = bvm->instruction[bvm->IP].operand;
				if(c._asU64 == u)
					stackPush(&bvm->stack, (a._asU64 / b._asU64));
				else if(c._asU64 == i) {
					stackPush(&bvm->stack, (a._asI64 / b._asI64));
					}
				else if(c._asU64 == f) {
					stackPushF64(&bvm->stack, (a._asF64 / b._asF64));
					}
				else {}; //PTR MAYBE
				bvm->IP++;
				break;
				}

		case DUP: {
				a = stackPop(&bvm->stack);
				stackPush(&bvm->stack, a._asI64);
				stackPush(&bvm->stack, a._asI64);
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
				a = bvm->instruction[bvm->IP].operand;

				bvm->IP = a._asU64;
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
					bvm->jumpReg = 1;
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

		case COPY: {
				//a = stackPop(&bvm->stack);
				b = bvm->instruction[bvm->IP].operand;
				c = bvm->stack.stack[b._asU64];
				stackPush(&bvm->stack, c._asI64);
				//bvm->stack.stack[b._asU64] = a;
				bvm->IP++;
				break;
			
				}

		case SWAP: {
				a = stackPop(&bvm->stack);
				b = bvm->instruction[bvm->IP].operand;
				c = bvm->stack.stack[b._asU64];
				stackPush(&bvm->stack, c._asI64);
				bvm->stack.stack[b._asU64] = a;
				bvm->IP++;
				break;
				}
		case MEM:{
				a = stackPop(&bvm->stack);
				b = bvm->instruction[bvm->IP].operand;
				c = bvm->stack.stack[b._asU64];
				stackPush(&bvm->stack, a._asI64);
				memcpy(&bvm->stack.stack[b._asU64], &a, sizeof(Word));
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

		case END: {
				bvm->isRuning = FALSE;
				LOG("\n\nExiting VM\n\n!!!");
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

static inline u64 textToProgram(const char* name, Instruction *instrucion) {

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

	FILE *f = fopen(name, "wb");
	if(f == NULL) {
		ERROR_BREAK("FILE ERROR !!!\n");
		}
	fwrite(instruction, sizeof(Instruction), numOfInstruction, f);
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

#endif
#endif
