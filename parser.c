#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string.h>
#include "lex.h"
#define LOG_STACK
#include "bvm.h"


//void Parser(Token *tokens);
#define MAX_VARS 100
#define MAX_VARS_NAME 100
//asumed that a var has global lifespawn
typedef struct {
	int adress[MAX_VARS];
	char* name[MAX_VARS];
	int sp;

	} VarStack;


void VarStackPush(VarStack* varstack,  char* name) {
	if(varstack->sp > MAX_VARS) {
		ERROR_BREAK("We have get more var then max amount max num of vars is %d\n", MAX_VARS);
		}
	//int lenName = strlen(name);
	int adress = STACK_CAPACITIY - 1 - varstack->sp;
	varstack->name[varstack->sp] = name;//malloc(sizeof(char)*(lenName+1));
	//strcpy(varstack->name[varstack->sp], name);
	varstack->adress[varstack->sp] = adress;
	printf("Var %s", name);
	varstack->sp++;
	}

void StackMemMove(Stack *stack, int index) {
	if(index >=  stack->SP) {
		ERROR_BREAK("Stack acces violation memmove\n");
		}
	for(int i = index; i < stack->SP - 1; i++) {
		stack->stack[i]._asI64 = stack->stack[i + 1]._asI64;
		}
	stack->SP--;
	}


int VarStackGet(const char* name, VarStack *varstack) {
	for(int i = 0; i < varstack->sp; i++) {
		if(!strcmp(name, varstack->name[i])) {
			return i;
			}
		}
	return -1;
	}


//void VarStackSetAddress(const char* name, int adress, VarStack *varstack) {
//	for(int i = 0; i < varstack->sp; i++) {
//		if(!strcmp(name, varstack->name[i])) {
//			varstack->adress = adress;
//			return;
//			}
//		ERROR_BREAK("We have a errror in non existent name %s\n", name);
//		}
//	}

void PrintVarStack(VarStack v) {
	for(int i = 0; i <= v.sp; i++) {
		printf("Var %s addres %d\n", v.name[i], v.adress[i]);
		}
	}


void PrintStack(Stack v) {
	for(int i = 0; i < v.SP; i++) {
		printf("Stack(%d) = %d\n", i, v.stack[i]);
		}
	}


#define WORD_COMPARE(KEYWORD) (!strcmp(keywords[KEYWORD], tokens[counterTokens].value))
#define WORD_COMPARE_TOKEN(KEYWORD, NUM) (!strcmp(keywords[KEYWORD], tokens[NUM].value))
int ValueToNum(char *str) {
	long int num;
	char *end;
	num = strtol(str, &end, 10);
	//printf("Num is %d", (int)num);
	return (int)num;
	}


void Parser(Token *tokens, Bvm *bvm) {

	//system("pause");

	int counterTokens = 0, counterInstruction = 0;
	//VarStack label;
	//label.sp = 0;
	Stack ifStack;
	Stack whileStack;
	Stack endloopStack;
	VarStack varStack;
	//memset(&varStack, 0, sizeof(VarStack)*MAX_VARS);
	varStack.sp = 0;
	initStack(&ifStack);
	initStack(&whileStack);
	initStack(&endloopStack);
	int stackSize = 0;
	while(tokens[counterTokens].type != TYPE_EOF) {

		//printf("Num of token %d\n", counterTokens);
		//system("pause");
		//PrintVarStack(varStack);
		//memset()
		if(counterTokens >= MAX_TOKENS) {
			ERROR("Error in max num of tokens\n");
			}
		//WE FOR NOW HAVA A NUMS PUSH THEM ONLY INT FOR NOW

		switch(tokens[counterTokens].type) {
			case TYPE_VAR: {
					if(counterTokens < 0)
						break;
					if(tokens[counterTokens-1].type == TYPE_KEYWORD
					    && !strcmp(tokens[counterTokens - 1].value, keywords[KEYWORD_LET])) {
						int is = VarStackGet(tokens[counterTokens].value, &varStack);
						if(is == -1) {
							VarStackPush(&varStack, tokens[counterTokens].value);
							is = varStack.sp - 1;
							}
						else {
							is =  VarStackGet(tokens[counterTokens].value, &varStack);
							}
						bvm->instruction[counterInstruction].type = MEM;
						bvm->instruction[counterInstruction].operand._asI64 = varStack.adress[is];
						printf("\nstore var %s addres %d, mem\n", tokens[counterTokens].value, varStack.adress[is]);
						counterTokens++;
						counterInstruction++;
						}
					else {
						int is = VarStackGet(tokens[counterTokens].value, &varStack);
						if(is == -1) {
							ERROR_BREAK("We have not declaread a var %s\n", tokens[counterTokens].value);
							}
						bvm->instruction[counterInstruction].type = COPY;
						bvm->instruction[counterInstruction].operand._asI64 = varStack.adress[is];
						printf("\nload var %s addres %d, mem\n", tokens[counterTokens].value, varStack.adress[is]);
						counterTokens++;
						counterInstruction++;
						stackSize++;
						}
					break;
					}

			case TYPE_CONST: {
					//char buffer[100];
					int valueNum = ValueToNum(tokens[counterTokens].value);
					bvm->instruction[counterInstruction].type = PUSH;
					bvm->instruction[counterInstruction].operand._asI64 = valueNum;
					printf("\nconst %d, push\n", valueNum);
					counterTokens++;
					counterInstruction++;
					stackSize++;
					break;
					}
			//MATH
			case TYPE_OPERATION: {
					switch(tokens[counterTokens].value[0]) {
						case '+': {
								int valueNum = ValueToNum(tokens[counterTokens].value);
								bvm->instruction[counterInstruction].type = ADD;
								bvm->instruction[counterInstruction].operand._asI64 = valueNum;
								printf("\noperation %s, add\n", tokens[counterTokens].value);
								counterTokens++;
								counterInstruction++;
								stackSize--;
								break;
								}
						case '-': {

								int valueNum = ValueToNum(tokens[counterTokens].value);
								bvm->instruction[counterInstruction].type = DEC;
								bvm->instruction[counterInstruction].operand._asI64 = valueNum;
								printf("\noperation %s, dec\n", tokens[counterTokens].value);
								counterTokens++;
								counterInstruction++;
								stackSize--;
								break;
								}
						case '*': {

								int valueNum = ValueToNum(tokens[counterTokens].value);
								bvm->instruction[counterInstruction].type = MUL;
								bvm->instruction[counterInstruction].operand._asI64 = valueNum;
								printf("\noperation %s, mul\n", tokens[counterTokens].value);
								counterTokens++;
								counterInstruction++;
								stackSize--;
								break;
								}
						case '/': {

								int valueNum = ValueToNum(tokens[counterTokens].value);
								bvm->instruction[counterInstruction].type = DIV;
								bvm->instruction[counterInstruction].operand._asI64 = valueNum;
								printf("\noperation %s, div\n", tokens[counterTokens].value);
								counterTokens++;
								counterInstruction++;
								stackSize--;
								break;
								}


						default: {
								ERROR("Not implemented math operation or somthing\n");
								break;
								}
						}
					break;
					}
			case TYPE_KEYWORD: {
					//search thru all keywords somw fensi prob maybe hash
					//PRINT FOR NOW ONLY INTEGERS
					if(WORD_COMPARE(KEYWORD_PRINT)) {
						//int valueNum = ValueToNum(tokens[counterTokens].value); HANDLING DEPEND ON TYPE OF A FILE
						bvm->instruction[counterInstruction].type = PRINT;
						bvm->instruction[counterInstruction].operand._asI64 = 0;
						printf("\n%s, print\n", tokens[counterTokens].value);
						counterTokens++;
						counterInstruction++;

						}
					else if (WORD_COMPARE(KEYWORD_DUP)) {
						//stackSize++;
						bvm->instruction[counterInstruction].type = DUP;
						bvm->instruction[counterInstruction].operand._asI64 = 0;
						printf("\n%s, dup\n", tokens[counterTokens].value);
						counterTokens++;
						counterInstruction++;
						stackSize++;
						}

					else if(WORD_COMPARE(KEYWORD_IF)) {
						//stackSize-=2;

						//stackSize = 0;
						int endTokenPos = counterInstruction, numIF = 0;
						while(tokens[endTokenPos].type != TYPE_EOF) {
							if(!strcmp(tokens[endTokenPos].value, keywords[KEYWORD_IF])) {
								numIF++;
								}
							if(!strcmp(tokens[endTokenPos].value, keywords[KEYWORD_END])) {
								numIF--;
								if(numIF == 0) {
									break;
									}
								}

							endTokenPos++;
							}

						//system("pause");
						if(tokens[endTokenPos].type == TYPE_EOF) {
							ERROR_BREAK("End token not provided\n");
							}
						bvm->instruction[counterInstruction].type = JMPF;
						bvm->instruction[counterInstruction].operand._asI64 = (i64)(endTokenPos);
						printf("\n%d, JUMPF\n", endTokenPos);
						counterTokens++;
						counterInstruction++;
						stackSize--;
						printf("StackSize %d\n", stackSize);
						stackPush(&ifStack, stackSize);


						}
					else if(WORD_COMPARE(KEYWORD_ELSE)) {
						bvm->instruction[counterInstruction].type = SETSP;
						bvm->instruction[counterInstruction].operand._asI64 = stackPop(&ifStack)._asI64;
						stackSize =  bvm->instruction[counterInstruction].operand._asI64;
						printf("\nreturn sp = %d, else\n", bvm->instruction[counterInstruction].operand._asI64);
						counterTokens++;
						counterInstruction++;
						}
					else if(WORD_COMPARE(KEYWORD_END)) {
						bvm->instruction[counterInstruction].type = NOP;
						bvm->instruction[counterInstruction].operand._asI64 = 0;
						counterTokens++;
						counterInstruction++;
						}
					else if(WORD_COMPARE(KEYWORD_LET)) {
						bvm->instruction[counterInstruction].type = NOP;
						bvm->instruction[counterInstruction].operand._asI64 = 0;
						counterTokens++;
						counterInstruction++;
						}
					else if(WORD_COMPARE(KEYWORD_DROP)) {
						//stackSize--;
						bvm->instruction[counterInstruction].type = POP;
						bvm->instruction[counterInstruction].operand._asI64 = 0;
						counterTokens++;
						counterInstruction++;
						stackSize--;
						}
					else if(WORD_COMPARE(KEYWORD_ENDLOOP)) {
						bvm->instruction[counterInstruction].type = JMP;
						bvm->instruction[counterInstruction].operand._asI64 = stackPop(&whileStack)._asI64;
						//stackSize =  bvm->instruction[counterInstruction].operand._asI64;
						printf("\nendwhile token = %d, end\n", bvm->instruction[counterInstruction].operand._asI64);
						stackPush(&endloopStack, (i64)counterInstruction);
						counterTokens++;
						counterInstruction++;
						}

					else if(WORD_COMPARE(KEYWORD_WHILE)) {
						bvm->instruction[counterInstruction].type = SETSP;
						bvm->instruction[counterInstruction].operand._asI64 = stackSize;
						stackPush(&whileStack, counterTokens);
						stackSize =  bvm->instruction[counterInstruction].operand._asI64;
						printf("\nwhile sp = %d, while\n", bvm->instruction[counterInstruction].operand._asI64);
						counterTokens++;
						counterInstruction++;
						}
					else if(WORD_COMPARE(KEYWORD_BREAKLOOP)) {
						//if(counterTokens <= 1){
						int endTokenPos = counterTokens, numWhile = 1;
						while(tokens[endTokenPos].type != TYPE_EOF) {
							if(!strcmp(tokens[endTokenPos].value, keywords[KEYWORD_WHILE])) {
								numWhile++;
								printf("Num of while %d\n", numWhile);
								//system("pause");
								}
							if(!strcmp(tokens[endTokenPos].value, keywords[KEYWORD_ENDLOOP])) {
								numWhile--;
								if(numWhile <= 0) {
									break;
									}
								}
							endTokenPos++;
							}
						bvm->instruction[counterInstruction].type = JMP;
						bvm->instruction[counterInstruction].operand._asI64 = (i64)(endTokenPos+1);
						printf("\n%d, JUMP OUT OF LOOP\n", endTokenPos);
						counterTokens++;
						counterInstruction++;

						}
					else {
						//counterInstruction++;
						counterTokens++;
						}
					break;
					}
			case TYPE_LOGICOPERATOR: {
					//IF EQUAL
					if(!strcmp(tokens[counterTokens].value, logicOperators[LOGIC_E])) {
						bvm->instruction[counterInstruction].type = IF;
						bvm->instruction[counterInstruction].operand._asI64 = 2;
						printf("\nlogicoperator %s, IF\n", tokens[counterTokens].value);
						counterTokens++;
						counterInstruction++;
						stackSize--;
						}
					else if (!strcmp(tokens[counterTokens].value, logicOperators[LOGIC_G])) {
						bvm->instruction[counterInstruction].type = IF;
						bvm->instruction[counterInstruction].operand._asI64 = 0;
						printf("\nlogicoperator %s, IF\n", tokens[counterTokens].value);
						counterTokens++;
						counterInstruction++;
						stackSize--;
						}
					else if(!strcmp(tokens[counterTokens].value, logicOperators[LOGIC_L])) {
						bvm->instruction[counterInstruction].type = IF;
						bvm->instruction[counterInstruction].operand._asI64 = 1;
						printf("\nlogicoperator %s, IF\n", tokens[counterTokens].value);
						counterTokens++;
						counterInstruction++;
						stackSize--;
						}
					//TD add all other logic operators
					else {};
					break;
					}
			default: {
					printf("Not implemented : %s \n", TokenString[tokens[counterTokens].type]);
					counterTokens++;
					break;
					}
			}
		}

	bvm->instruction[counterInstruction].type = END;
	bvm->instruction[counterInstruction].operand._asU64 = 0;
	bvm->numOfInstructions = (u64)counterTokens;
	}
