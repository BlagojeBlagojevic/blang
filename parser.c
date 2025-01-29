#include "parser.h"

static void VarStackPush(VarStack* varstack,  char* name, uint8_t type) {
	if(varstack->sp > MAX_VARS) {
		ERROR_BREAK("We have get more var then max amount max num of vars is %d\n", MAX_VARS);
		}
	//int lenName = strlen(name);
	int adress = STACK_CAPACITIY - 1 - varstack->sp;
	varstack->name[varstack->sp] = name;//arena_alloc(sizeof(char)*(lenName+1);
	//strcpy(varstack->name[varstack->sp], name);
	varstack->adress[varstack->sp] = type;
	varstack->adress[varstack->sp] = adress;
	//printf("Var %s", name);
	varstack->sp++;
	}

static void StackMemMove(Stack *stack, int index) {
	if(index >=  stack->SP) {
		ERROR_BREAK("Stack acces violation memmove\n");
		}
	for(int i = index; i < stack->SP - 1; i++) {
		stack->stack[i]._asI64 = stack->stack[i + 1]._asI64;
		}
	stack->SP--;
	}


static int VarStackGet(const char* name, VarStack *varstack) {
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

static void PrintVarStack(VarStack v) {
	for(int i = 0; i <= v.sp; i++) {
		printf("Var %s addres %d\n", v.name[i], v.adress[i]);
		}
	}


static void PrintStack(Stack v) {
	for(int i = 0; i < v.SP; i++) {
		printf("Stack(%d) = %d\n", i, v.stack[i]);
		}
	}


#define WORD_COMPARE(KEYWORD) (!strcmp(keywords[KEYWORD], tokens[counterTokens].value))
#define WORD_COMPARE_TOKEN(KEYWORD, NUM) (!strcmp(keywords[KEYWORD], tokens[NUM].value))


void Parser(Token *tokens, Words *words, Bvm *bvm, Arena *mainArena) {

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
	int numOfTokens = 0;
	while(tokens[numOfTokens].type != TYPE_EOF) {
		printf("num of token %d defiend words %d\n", numOfTokens, numOfUserDefiendWords);
		numOfTokens++;
		}
	numOfTokens++;//take EOF
	//INITAL PASS TO CHANGE A TOKENS BASED OF A NAME IN A WORDS TABLE
	//WE DO A SHIFT RIGHT FOR A NUM OF A TOKENS IN A WORD
	//THEN WE COPY CONTENT IN A WORD STARTING FROM TOKEN+1 OR TIL word[i].token[j].value == NULL

	while(tokens[counterTokens++].type != TYPE_EOF) {
		if(tokens[counterTokens].type == TYPE_VAR) {
			for(int i = 0; words[i].name != NULL; i++) {
				if(!strcmp(tokens[counterTokens].value, words[i].name)) {
					const int endShift = counterTokens+1;
					const int shiftOfset = words[i].numOfTokens - 2;  //CUZZ OF FIRST IS A NAME

					for(int j = numOfTokens; j >= endShift; j--) {
						memcpy(&tokens[j + shiftOfset], &tokens[j], sizeof(Token));
						//	printf("shifting\n");
						//tokens[i + shiftOfset] = tokens[i];
						}
					numOfTokens+=shiftOfset;
					//	PrintTokens(tokens);
					//	printf("Num of %d Curent %d End %d ShiftOfset %d\n",numOfTokens, counterTokens,
					//																											endShift, shiftOfset);
					//	system("pause");
					int counter = 1;
					for(int j = endShift - 1; words[i].tokens[counter].value != NULL; j++) {
						memmove(&tokens[j], &words[i].tokens[counter], sizeof(Token));
						counter++;
						}
					counterTokens--;
					//	PrintTokens(tokens);
					//	printf("Num of %d Curent %d End %d ShiftOfset %d\n",numOfTokens, counterTokens,
					//	       endShift, shiftOfset);
					//system("pause");
					}
				}
			}
		}
	counterTokens = 0;
	PrintTokens(tokens);
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
							VarStackPush(&varStack, tokens[counterTokens].value, tokens[counterTokens].type);
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
					else if(tokens[counterTokens-1].type == TYPE_KEYWORD
					        && !strcmp(tokens[counterTokens - 1].value, keywords[KEYWORD_PTR])) {
						int is = VarStackGet(tokens[counterTokens].value, &varStack);
						if(is == -1) {
							ERROR_BREAK("We have not declared a var %s", tokens[counterTokens].value);
							}
						printf("\nptr var %s addres %d, mem\n", tokens[counterTokens].value, varStack.adress[is]);
						bvm->instruction[counterInstruction].type = PUSH;
						bvm->instruction[counterInstruction].operand._asI64 = varStack.adress[is];
						counterTokens++;
						counterInstruction++;
						stackSize++;
						}
					else if(tokens[counterTokens-1].type == TYPE_KEYWORD
					        && !strcmp(tokens[counterTokens - 1].value, keywords[KEYWORD_ARR])
					        && (tokens[counterTokens + 1].type == TYPE_CONST)) {
						int numOfVars = ValueToNum(tokens[counterTokens + 1].value);

						for(int i = numOfVars; i >= 0; i--) {
							char *name = arena_alloc(mainArena, 100*sizeof(char));
							sprintf(name, "%s%d", tokens[counterTokens].value, i);
							//printf("\nname %s\n", name);
							VarStackPush(&varStack, name, tokens[counterTokens].valType);
							}
						//PrintVarStack(varStack);
						//system("pause");
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
					if(tokens[counterTokens].valType == F) {
						printf("%s", tokens[counterTokens].value);
						float valueNum = strtof(tokens[counterTokens].value,NULL);
						bvm->instruction[counterInstruction].type = PUSHF;
						bvm->instruction[counterInstruction].operand._asF64 = valueNum;
						printf("\nconst %f, push\n", valueNum);
						counterTokens++;
						counterInstruction++;
						stackSize++;
						break;
						}
					else if (tokens[counterTokens].valType == CH) {
						//int valueNum = ValueToNum(tokens[counterTokens].value);
						bvm->instruction[counterInstruction].type = PUSH;
						bvm->instruction[counterInstruction].operand._asI64 = (i64)tokens[counterTokens].value[0];
						printf("\nconst %d, push\n", tokens[counterTokens].value[0]);
						counterTokens++;
						counterInstruction++;
						stackSize++;
						break;
						}
					else {
						int valueNum = ValueToNum(tokens[counterTokens].value);
						bvm->instruction[counterInstruction].type = PUSH;
						bvm->instruction[counterInstruction].operand._asI64 = valueNum;
						printf("\nconst %d, push\n", valueNum);
						counterTokens++;
						counterInstruction++;
						stackSize++;
						break;
						}



					}
			//MATH
			case TYPE_OPERATION: {
					switch(tokens[counterTokens].value[0]) {
						case '+': {
								//int valueNum = ValueToNum(tokens[counterTokens].value);
								bvm->instruction[counterInstruction].type = ADD;
								bvm->instruction[counterInstruction].operand._asI64 = (int64_t)tokens[counterTokens-1].valType;
								printf("\noperation %s, add\n", tokens[counterTokens].value);
								counterTokens++;
								counterInstruction++;
								stackSize--;
								break;
								}
						case '-': {

								//int valueNum = ValueToNum(tokens[counterTokens].value);
								bvm->instruction[counterInstruction].type = DEC;
								bvm->instruction[counterInstruction].operand._asI64 = (int64_t)tokens[counterTokens-1].valType;
								printf("\noperation %s, dec\n", tokens[counterTokens].value);
								counterTokens++;
								counterInstruction++;
								stackSize--;
								break;
								}
						case '*': {

								//int valueNum = ValueToNum(tokens[counterTokens].value);
								bvm->instruction[counterInstruction].type = MUL;
								bvm->instruction[counterInstruction].operand._asI64 = (int64_t)tokens[counterTokens-1].valType;
								printf("\noperation %s, mul\n", tokens[counterTokens].value);
								counterTokens++;
								counterInstruction++;
								stackSize--;
								break;
								}
						case '/': {

								//int valueNum = ValueToNum(tokens[counterTokens].value);
								bvm->instruction[counterInstruction].type = DIV;
								bvm->instruction[counterInstruction].operand._asI64 = (int64_t)tokens[counterTokens-1].valType;
								printf("\noperation %s, div\n", tokens[counterTokens].value);
								counterTokens++;
								counterInstruction++;
								break;
								}
						case '!': {
								//int valueNum = ValueToNum(tokens[counterTokens].value);
								bvm->instruction[counterInstruction].type = NOT;
								bvm->instruction[counterInstruction].operand._asI64 = 0;
								printf("\noperation %s, notLogic\n", tokens[counterTokens].value);
								counterTokens++;
								counterInstruction++;
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
					else if(WORD_COMPARE(KEYWORD_PRINTCHAR)) {
						//int valueNum = ValueToNum(tokens[counterTokens].value); HANDLING DEPEND ON TYPE OF A FILE
						bvm->instruction[counterInstruction].type = PRINT;
						bvm->instruction[counterInstruction].operand._asI64 = 3;
						printf("\n%s, print\n", tokens[counterTokens].value);
						counterTokens++;
						counterInstruction++;
						stackSize--;
						}

					else if(WORD_COMPARE(KEYWORD_PRINTFLOAT)) {
						//int valueNum = ValueToNum(tokens[counterTokens].value); HANDLING DEPEND ON TYPE OF A FILE
						bvm->instruction[counterInstruction].type = PRINT;
						bvm->instruction[counterInstruction].operand._asI64 = F;
						printf("\n%s, print\n", tokens[counterTokens].value);
						counterTokens++;
						counterInstruction++;

						}
					else if(WORD_COMPARE(KEYWORD_PRINTSTACK)) {
						//int valueNum = ValueToNum(tokens[counterTokens].value); HANDLING DEPEND ON TYPE OF A FILE
						bvm->instruction[counterInstruction].type = PRINTSTACK;
						bvm->instruction[counterInstruction].operand._asI64 = 0;
						printf("\n%s, printstack\n", tokens[counterTokens].value);
						counterTokens++;
						counterInstruction++;
						}

					else if(WORD_COMPARE(KEYWORD_PRINTSTRING)) {
						//int valueNum = ValueToNum(tokens[counterTokens].value); HANDLING DEPEND ON TYPE OF A FILE
						bvm->instruction[counterInstruction].type = PRINTSTRING;
						bvm->instruction[counterInstruction].operand._asI64 = 456654;
						printf("\n%s, printstring\n", tokens[counterTokens].value);
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

					else if (WORD_COMPARE(KEYWORD_PUSHSPMEM)) {
						//stackSize++;
						bvm->instruction[counterInstruction].type = PUSHSP;
						bvm->instruction[counterInstruction].operand._asI64 = 123;
						printf("\n%s, SP push\n", tokens[counterTokens].value);
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

					else if(WORD_COMPARE(KEYWORD_SET)) {
						bvm->instruction[counterInstruction].type = SETSPSTACK;
						bvm->instruction[counterInstruction].operand._asI64 = 123;
						counterTokens++;
						counterInstruction++;
						stackSize--;
						}

					else if(WORD_COMPARE(KEYWORD_END)) {
						bvm->instruction[counterInstruction].type = NOP;
						bvm->instruction[counterInstruction].operand._asI64 = 0;
						counterTokens++;
						counterInstruction++;
						}

					else if(WORD_COMPARE(KEYWORD_HALT)) {
						bvm->instruction[counterInstruction].type = HALT;
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
					else if(WORD_COMPARE(KEYWORD_ARR)) {
						bvm->instruction[counterInstruction].type = NOP;
						bvm->instruction[counterInstruction].operand._asI64 = 0;
						counterTokens++;
						counterInstruction++;
						}

					else if(WORD_COMPARE(KEYWORD_LETARR)) {
						bvm->instruction[counterInstruction].type = MEMSTACK;
						bvm->instruction[counterInstruction].operand._asI64 = 0;
						stackSize--;
						counterTokens++;
						counterInstruction++;
						}
					else if(WORD_COMPARE(KEYWORD_PTR)) {
						bvm->instruction[counterInstruction].type = NOP;
						bvm->instruction[counterInstruction].operand._asI64 = 0;
						counterTokens++;
						counterInstruction++;
						}

					else if(WORD_COMPARE(KEYWORD_PTRVAL)) {
						bvm->instruction[counterInstruction].type = COPYSTACK;
						bvm->instruction[counterInstruction].operand._asI64 = 0;
						counterTokens++;
						counterInstruction++;
						}

					else if(WORD_COMPARE(KEYWORD_DROP)) {
						//stackSize--;
						bvm->instruction[counterInstruction].type = POP;
						bvm->instruction[counterInstruction].operand._asI64 = U;
						counterTokens++;
						counterInstruction++;
						stackSize--;
						}
					else if(WORD_COMPARE(KEYWORD_OVER)) {
						bvm->instruction[counterInstruction].type = OVER;
						bvm->instruction[counterInstruction].operand._asI64 = 0;
						counterTokens++;
						counterInstruction++;
						stackSize++;
						}
					else if(WORD_COMPARE(KEYWORD_ROT)) {
						bvm->instruction[counterInstruction].type = ROT;
						bvm->instruction[counterInstruction].operand._asI64 = 0;
						counterTokens++;
						counterInstruction++;
						}

					else if(WORD_COMPARE(KEYWORD_SWAP)) {
						bvm->instruction[counterInstruction].type = SWAP_NO;
						bvm->instruction[counterInstruction].operand._asI64 = 123;
						counterTokens++;
						counterInstruction++;
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
					else if(WORD_COMPARE(KEYWORD_SHL)) {
						bvm->instruction[counterInstruction].type = SHL;
						bvm->instruction[counterInstruction].operand._asI64 = 0;
						counterTokens++;
						counterInstruction++;
						stackSize--;
						}
					else if(WORD_COMPARE(KEYWORD_SHR)) {
						bvm->instruction[counterInstruction].type = SHR;
						bvm->instruction[counterInstruction].operand._asI64 = 0;
						counterTokens++;
						counterInstruction++;
						stackSize--;
						}
					else if(WORD_COMPARE(KEYWORD_AND)) {
						bvm->instruction[counterInstruction].type = AND;
						bvm->instruction[counterInstruction].operand._asI64 = 0;
						counterTokens++;
						counterInstruction++;
						stackSize--;
						}
					else if(WORD_COMPARE(KEYWORD_OR)) {
						bvm->instruction[counterInstruction].type = OR;
						bvm->instruction[counterInstruction].operand._asI64 = 0;
						counterTokens++;
						counterInstruction++;
						stackSize--;
						}
					else if(WORD_COMPARE(KEYWORD_BNOT)) {
						bvm->instruction[counterInstruction].type = BNOT;
						bvm->instruction[counterInstruction].operand._asI64 = 0;
						counterTokens++;
						counterInstruction++;
						stackSize--;
						}
					else if(WORD_COMPARE(KEYWORD_SETSP)) {
						bvm->instruction[counterInstruction].type = SETSPSTACK;
						bvm->instruction[counterInstruction].operand._asI64 = 0;
						stackSize =  bvm->instruction[counterInstruction].operand._asI64;
						printf("\nreturn sp = %d, else\n", bvm->instruction[counterInstruction].operand._asI64);
						counterTokens++;
						counterInstruction++;
						stackSize--;
						}
					//SYSCALLS posix
#ifdef SYSCALLS
					else if(WORD_COMPARE(KEYWORD_WRITE)) {
						bvm->instruction[counterInstruction].type = WRITE;
						bvm->instruction[counterInstruction].operand._asI64 = 123;
						stackSize =  bvm->instruction[counterInstruction].operand._asI64;
						printf("\nwrite = %d,\n", bvm->instruction[counterInstruction].operand._asI64);
						counterTokens++;
						counterInstruction++;
						stackSize-=2;
						}
					else if(WORD_COMPARE(KEYWORD_CLOSE)) {
						bvm->instruction[counterInstruction].type = CLOSE;
						bvm->instruction[counterInstruction].operand._asI64 = 123;
						stackSize =  bvm->instruction[counterInstruction].operand._asI64;
						printf("\nclose = %d,\n", bvm->instruction[counterInstruction].operand._asI64);
						counterTokens++;
						counterInstruction++;
						}

					else if(WORD_COMPARE(KEYWORD_DUPF)) {
						bvm->instruction[counterInstruction].type = DUPF;
						bvm->instruction[counterInstruction].operand._asI64 = 123;
						stackSize =  bvm->instruction[counterInstruction].operand._asI64;
						printf("\ndupFd = %d,\n", bvm->instruction[counterInstruction].operand._asI64);
						counterTokens++;
						counterInstruction++;
						}

					else if(WORD_COMPARE(KEYWORD_DUP2)) {
						bvm->instruction[counterInstruction].type = DUPF;
						bvm->instruction[counterInstruction].operand._asI64 = 123;
						stackSize =  bvm->instruction[counterInstruction].operand._asI64;
						printf("\ndupFd2 = %d,\n", bvm->instruction[counterInstruction].operand._asI64);
						counterTokens++;
						counterInstruction++;
						stackSize--;
						}
					else if(WORD_COMPARE(KEYWORD_EXIT)) {
						bvm->instruction[counterInstruction].type = EXIT;
						bvm->instruction[counterInstruction].operand._asI64 = 123;
						stackSize =  bvm->instruction[counterInstruction].operand._asI64;
						printf("\nexit = %d,\n", bvm->instruction[counterInstruction].operand._asI64);
						counterTokens++;
						counterInstruction++;
						stackSize--;
						}
					else if(WORD_COMPARE(KEYWORD_TRUNC)) {
						bvm->instruction[counterInstruction].type = TRUNCATE;
						bvm->instruction[counterInstruction].operand._asI64 = 123;
						stackSize =  bvm->instruction[counterInstruction].operand._asI64;
						printf("\ntrunc = %d,\n", bvm->instruction[counterInstruction].operand._asI64);
						counterTokens++;
						counterInstruction++;
						stackSize--;
						}
					else if(WORD_COMPARE(KEYWORD_ISATTY)) {
						bvm->instruction[counterInstruction].type = ISATTY;
						bvm->instruction[counterInstruction].operand._asI64 = 123;
						stackSize =  bvm->instruction[counterInstruction].operand._asI64;
						printf("\ntisatty = %d,\n", bvm->instruction[counterInstruction].operand._asI64);
						counterTokens++;
						counterInstruction++;
						stackSize--;
						}

					else if(WORD_COMPARE(KEYWORD_READ)) {
						bvm->instruction[counterInstruction].type = READ;
						bvm->instruction[counterInstruction].operand._asI64 = 123;
						stackSize =  bvm->instruction[counterInstruction].operand._asI64;
						printf("\nread = %d,\n", bvm->instruction[counterInstruction].operand._asI64);
						counterTokens++;
						counterInstruction++;
						stackSize-=2;
						}

					else if(WORD_COMPARE(KEYWORD_SLEEP)) {
						bvm->instruction[counterInstruction].type = SLEEP;
						bvm->instruction[counterInstruction].operand._asI64 = 123;
						stackSize =  bvm->instruction[counterInstruction].operand._asI64;
						printf("\nsleep = %d,\n", bvm->instruction[counterInstruction].operand._asI64);
						counterTokens++;
						counterInstruction++;
						}
#ifdef SYSTEM
					else if(WORD_COMPARE(KEYWORD_SYSTEM)) {
						bvm->instruction[counterInstruction].type = SYSTEMS;
						bvm->instruction[counterInstruction].operand._asI64 = 123;
						stackSize =  bvm->instruction[counterInstruction].operand._asI64;
						printf("\nsystem = %d,\n", bvm->instruction[counterInstruction].operand._asI64);
						counterTokens++;
						counterInstruction++;
						stackSize++;
						}
#endif
#endif


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
					else if(!strcmp(tokens[counterTokens].value, logicOperators[LOGIC_I])) {
						bvm->instruction[counterInstruction].type = IF;
						bvm->instruction[counterInstruction].operand._asI64 = 2;
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
