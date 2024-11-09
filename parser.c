
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string.h>
#include "lex.h"
//#include "lex.c"

//void Parser(Token *tokens);


#define WRITE_TO_FILE(str)\
		{char buffer[100]; \
		memset(buffer, '\0', sizeof(char)*100);\
		sprintf(buffer, str, tokens[counterTokens].value);\
		fwrite(buffer,sizeof(char), strlen(buffer), f); counterInstruction++;\
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
		//system("pause");
		if(counterTokens >= MAX_TOKENS) {
			ERROR("Error in max num of tokens\n");
			}
		//WE FOR NOW HAVA A NUMS PUSH THEM ONLY INT FOR NOW

		switch(tokens[counterTokens].type) {
			case TYPE_CONST: {
					//char buffer[100];
					WRITE_TO_FILE("PUSH %s\n");
					counterTokens++;
					break;
					}
			//MATH
			case TYPE_OPERATION: {
					switch(tokens[counterTokens].value[0]) {
						case '+': {
								//if add type checking heare change
								WRITE_TO_FILE("ADD 0\n");
								counterTokens++;

								break;
								}
						case '-': {
								WRITE_TO_FILE("DEC 0\n");
								counterTokens++;

								break;
								}
						case '*': {
								WRITE_TO_FILE("MUL 0\n");
								counterTokens++;
								break;
								}
						case '/': {
								WRITE_TO_FILE("DIV 0\n");
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
					int endTokenPosition = counterInstruction, numIf = 0, counterTemp = counterTokens;
					int numOfIf = 1;
					
					while(tokens[counterTemp].type != TYPE_EOF){
						printf("CI %d ETP %d NIF %d\n", counterInstruction, endTokenPosition, numOfIf);
						//system("pause");
						if(!strcmp(tokens[counterTemp].value, keywords[KEYWORD_IF]) 
							&& tokens[counterTemp].type == TYPE_KEYWORD){
								numOfIf++;
								endTokenPosition++;
							}
						
						else if(!strcmp(tokens[counterTemp].value, keywords[KEYWORD_END]) 
							&& tokens[counterTemp].type == TYPE_KEYWORD){
								//endTokenPosition++;
								numOfIf--;
								if(numOfIf == 0){
									endTokenPosition+=2;
									break;
								}
								
							}
						else{
							endTokenPosition++;
						}
						
						counterTemp++;
					}	
					
					if(tokens[counterTemp].type == TYPE_EOF) {
						ERROR("End Token not provided Token: %s\n", tokens[counterTokens].value);
						}

					if(strlen(tokens[counterTokens].value) == 1) {
						WRITE_TO_FILE("IF %s\n");
						char buffer[100];
						sprintf(buffer, "JMPF %d\n", endTokenPosition - 1);
						fwrite(buffer, sizeof(char), strlen(buffer), f);
						counterInstruction++;
						counterTokens++;

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
