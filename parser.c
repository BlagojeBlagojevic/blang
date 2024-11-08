#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string.h>
#include "lex.h"
//#include "lex.c"

//void Parser(Token *tokens);


#define WRITE_TO_FILE(str)\
		{char buffer[100];\
		memset(buffer, '\0', sizeof(char)*100);\
		sprintf(buffer, str, tokens[counterTokens].value);\
		fwrite(buffer,sizeof(char), strlen(buffer), f);\
		}


#define WORD_COMPARE(KEYWORD) (!strcmp(keywords[KEYWORD], tokens[counterTokens].value))


void Parser(Token *tokens, const char* name) {
	int counterTokens = 0, counterInstruction = 0;
	FILE *f = fopen(name, "w");
	if(f == NULL) {
		ERROR("We have not init a file why who now\n");
		}
	while(tokens[counterTokens].type != TYPE_EOF) {
		printf("Num of token %d\n", counterTokens);
		if(counterTokens >= MAX_TOKENS) {
			ERROR("Error in max num of tokens\n");
			}
		//WE FOR NOW HAVA A NUMS PUSH THEM ONLY INT FOR NOW

		switch(tokens[counterTokens].type) {
			case TYPE_CONST: {
					//char buffer[100];
					WRITE_TO_FILE("PUSH %s\n");
					counterTokens++;
					counterInstruction++;
					break;
					}
			//MATH
			case TYPE_OPERATION: {
					switch(tokens[counterTokens].value[0]) {
						case '+': {
								//if add type checking heare change
								WRITE_TO_FILE("ADD 0\n");
								counterTokens++;
								counterInstruction++;
								break;
								}
						case '-': {
								WRITE_TO_FILE("DEC 0\n");
								counterTokens++;
								counterInstruction++;
								break;
								}
						case '*': {
								WRITE_TO_FILE("MUL 0\n");
counterInstruction++;
								counterTokens++;
								break;
								}
						case '/': {
								WRITE_TO_FILE("DIV 0\n");
								counterInstruction++;
								counterTokens++;
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
						WRITE_TO_FILE("PRINT 0\n");
						counterInstruction++;
						counterTokens++;

						}

					else {
						//counterInstruction++;
						counterTokens++;
						}
					break;
					}
			case TYPE_LOGICOPERATOR: {
					//GET A FIRST END TOKEN
					int endTokenPosition = 0;
					//IN MACRO PROBOBLY
					for(int i = counterTokens+2; tokens[i].type != TYPE_EOF; i++) {
						//printf("counter %d\n", i);
						if(tokens[i].type == TYPE_KEYWORD) {
							//printf("token %s keyword %s\n", tokens[i].value, keywords[KEYWORD_END]);
							if(!strcmp(tokens[i].value, keywords[KEYWORD_IF])){
								endTokenPosition+=2;   //IF HAS 3 INSTRUCTIONS
							}
							if(!strcmp(tokens[i].value, keywords[KEYWORD_END])) {
								//system("pause");
								endTokenPosition += i;
								printf("End token position %d\n", endTokenPosition);
								break;
								}
							}
						}

					if(endTokenPosition == 0) {
						ERROR("End Token not provided Token: %s\n", tokens[counterTokens].value);
						}

					if(strlen(tokens[counterTokens].value) == 1) {
						WRITE_TO_FILE("IF %s\n");
						char buffer[100];
						sprintf(buffer, "JMPF %d\n", endTokenPosition);
						fwrite(buffer, sizeof(char), strlen(buffer), f);
						counterTokens++;
						counterInstruction+=3;  //3 INSTRUCTION FOR IF STATMENT
						}
					else {
						//Todo implement other shit AKA >=, <=
						}
					break;
					}
			default: {
					printf("Not implemented : %s \n", TokenString[tokens[counterTokens].type]);
					counterTokens++;
					break;
					}
			}


		}
	WRITE_TO_FILE("END 0");
	fclose(f);

	}
