#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string.h>
#include "lex.h"
//#include "lex.c"

//void Parser(Token *tokens);


#define WRITE_TO_FILE(str)\
	char buffer[100];\
	memset(buffer, '\0', sizeof(char)*100);\
	sprintf(buffer, str, tokens[counterTokens].value);\
	fwrite(buffer,sizeof(char), strlen(buffer), f);\
	counterTokens++;



void Parser(Token *tokens, const char* name)
{
	int counterTokens = 0;
	FILE *f = fopen(name, "w");
	if(f == NULL) {
		ERROR("We have not init a file why who now");
	}
	while(tokens[counterTokens].type != TYPE_EOF) {
		if(counterTokens >= MAX_TOKENS) {
			ERROR("Error in max num of tokens");
		}
		//WE FOR NOW HAVA A NUMS PUSH THEM ONLY INT FOR NOW

		switch(tokens[counterTokens].type) {
			case TYPE_CONST: {
				//char buffer[100];
				WRITE_TO_FILE("PUSH %s\n");
				break;
			}
			//MATH
			case TYPE_OPERATION: {
				switch(tokens[counterTokens].value[0]) {
					case '+': {
						//if add type checking heare change
						WRITE_TO_FILE("ADD 0\n");
						break;
					}
					case '-': {
						WRITE_TO_FILE("DEC 0\n");
						break;
					}
					case '*': {
						WRITE_TO_FILE("MUL 0\n");
						break;
					}
					case '/': {
						WRITE_TO_FILE("DIV 0\n");
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
				if(!strcmp(keywords[KEYWORD_PRINT], tokens[counterTokens].value)) {
					WRITE_TO_FILE("PRINT 0\n");

				} else {

				}
				break;
			}

			default: {
				printf("Not implemented : %s ", TokenString[tokens[counterTokens].type]);
				counterTokens++;
				break;
			}
		}


	}
	WRITE_TO_FILE("END 0");
	fclose(f);

}