

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string.h>
#include "lex.h"
//#define LOG_STACK
//#include "bvm.h"


//void Parser(Token *tokens);



#define WORD_COMPARE(KEYWORD) (!strcmp(keywords[KEYWORD], tokens[counterTokens].value))

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

	Stack ifStack;
	initStack(&ifStack);
	int stackSize = 0;
	//
	while(tokens[counterTokens].type != TYPE_EOF) {

		//printf("Num of token %d\n", counterTokens);
		//system("pause");

		//memset()
		if(counterTokens >= MAX_TOKENS) {
			ERROR("Error in max num of tokens\n");
			}
		//WE FOR NOW HAVA A NUMS PUSH THEM ONLY INT FOR NOW

		switch(tokens[counterTokens].type) {
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

								break;
								}
						case '-': {

								int valueNum = ValueToNum(tokens[counterTokens].value);
								bvm->instruction[counterInstruction].type = DEC;
								bvm->instruction[counterInstruction].operand._asI64 = valueNum;
								printf("\noperation %s, dec\n", tokens[counterTokens].value);
								counterTokens++;
								counterInstruction++;

								break;
								}
						case '*': {

								int valueNum = ValueToNum(tokens[counterTokens].value);
								bvm->instruction[counterInstruction].type = MUL;
								bvm->instruction[counterInstruction].operand._asI64 = valueNum;
								printf("\noperation %s, mul\n", tokens[counterTokens].value);
								counterTokens++;
								counterInstruction++;

								break;
								}
						case '/': {

								int valueNum = ValueToNum(tokens[counterTokens].value);
								bvm->instruction[counterInstruction].type = DIV;
								bvm->instruction[counterInstruction].operand._asI64 = valueNum;
								printf("\noperation %s, div\n", tokens[counterTokens].value);
								counterTokens++;
								counterInstruction++;

								break;
								}
							stackSize--;
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
						bvm->instruction[counterInstruction].type = DUP;
						bvm->instruction[counterInstruction].operand._asI64 = 0;
						printf("\n%s, dup\n", tokens[counterTokens].value);
						counterTokens++;
						counterInstruction++;
						stackSize++;
						}

					else if(WORD_COMPARE(KEYWORD_IF)) {
						stackSize-=2;
						stackPush(&ifStack, stackSize);
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

						}
					else if(WORD_COMPARE(KEYWORD_ELSE)) {
						bvm->instruction[counterInstruction].type = SETSP;
						bvm->instruction[counterInstruction].operand._asI64 = stackPop(&ifStack)._asI64;
						stackSize = bvm->instruction[counterInstruction].operand._asI64;
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
						}
					else if (!strcmp(tokens[counterTokens].value, logicOperators[LOGIC_G])) {
						bvm->instruction[counterInstruction].type = IF;
						bvm->instruction[counterInstruction].operand._asI64 = 0;
						printf("\nlogicoperator %s, IF\n", tokens[counterTokens].value);
						counterTokens++;
						counterInstruction++;
						}
					else if(!strcmp(tokens[counterTokens].value, logicOperators[LOGIC_L])) {
						bvm->instruction[counterInstruction].type = IF;
						bvm->instruction[counterInstruction].operand._asI64 = 1;
						printf("\nlogicoperator %s, IF\n", tokens[counterTokens].value);
						counterTokens++;
						counterInstruction++;
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

		//system("pause");
		}

	bvm->instruction[counterInstruction].type = END;
	bvm->instruction[counterInstruction].operand._asU64 = 0;
	bvm->numOfInstructions = (u64)counterTokens;


	}
