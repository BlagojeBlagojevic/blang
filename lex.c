#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<stdlib.h>
#include "utils_string.h"

#define NUM_OF_TOKENS 5
#define ERROR(...)  {fprintf(stderr, __VA_ARGS__); exit(-1);}

#define MAX_TOKENS 10000
#define MAX_LENGHT  100
#include "lex.h"
//#include "utils_string.h"


Token* Tokeniser(char* input) {

	Token* tokens = malloc(MAX_TOKENS*sizeof(char));

	int left = 0, right = 0;
	int len = strlen(input);
	int counterTokens = 0;
	while (right <= len && left <= right) {

		if(counterTokens == MAX_TOKENS) {
			ERROR("We have excide max amount of tokens\n\n");
			}
		if (!isEndChar(input[right]))
			right++;

		if (isEndChar(input[right]) && left == right) {
			if (isOperator(input[right])) {
				//CHECK FOR A LOGICAL OPERATOR
				if(isLogicOperator(input[right])) {
					//IF OPERATOR CHECK FOR A = SIGN
					if(input[right+1] == '=') {

						tokens[counterTokens].type = TYPE_LOGICOPERATOR;
						tokens[counterTokens].value = malloc(3*sizeof(char));
						tokens[counterTokens].value[0] = input[right];
						tokens[counterTokens].value[1] = input[right + 1];//CHECK
						tokens[counterTokens].value[2] = '\0';

#ifdef LOG_VAL
						printf("Token: LogicOperator, Value: %s\n", tokens[counterTokens].value);
#endif
						counterTokens++;
						right++;

						}
					else {

						tokens[counterTokens].type = TYPE_LOGICOPERATOR;
						tokens[counterTokens].value = malloc(2*sizeof(char));
						tokens[counterTokens].value[0] = input[right];
						tokens[counterTokens].value[1] = '\0';
#ifdef LOG_VAL
						printf("Token: LogicOperator, Value: %s\n", tokens[counterTokens].value);
#endif
						counterTokens++;
						}
					}

				else {
#ifdef LOG_VAL
					printf("Token: Operator, Value: %c\n", input[right]);
#endif


					tokens[counterTokens].type = TYPE_OPERATION;
					tokens[counterTokens].value = malloc(2*sizeof(char));
					tokens[counterTokens].value[0] = input[right];
					tokens[counterTokens].value[1] = '\0';

					counterTokens++;
					}

				}

			right++;
			left = right;
			}
		else if ((isEndChar(input[right]) && (left != right)) || (right == len
		         && left != right)) {
			char* subStr = returnSubstring(input, left, right - 1);
			if(!strcmp(subStr, "endscript")) {
				break;
				}

			if (isKeyword(subStr)) {
#ifdef LOG_VAL
				printf("Token: Keyword, Value: %s\n", subStr);
#endif
				tokens[counterTokens].value = subStr;
				tokens[counterTokens].type = TYPE_KEYWORD;
				//printf("substr %s", tokens[counterTokens].value);
				counterTokens++;

				//IF KEYWORD IS ELSE ADD 1 IF = TOKENS
				if(!strcmp(subStr, keywords[KEYWORD_ELSE])) {
					//counterTokens--;
					//free(tokens[counterTokens].value);
					tokens[counterTokens].value = malloc(2*sizeof(char));
					tokens[counterTokens].value[0] = '0';
					tokens[counterTokens].value[1] = '\0';
					tokens[counterTokens].type = TYPE_CONST;
					counterTokens++;
					tokens[counterTokens].value = malloc(2*sizeof(char));
					tokens[counterTokens].value[0] = '=';
					tokens[counterTokens].value[1] = '\0';
					tokens[counterTokens].type = TYPE_LOGICOPERATOR;
					counterTokens++;
					tokens[counterTokens].value = malloc(3*sizeof(char));
					strcpy(tokens[counterTokens].value, "if");
					tokens[counterTokens].type = TYPE_KEYWORD;
					counterTokens++;

					}
				}


			else if (isInteger(subStr)) {
#ifdef LOG_VAL
				printf("Token: Integer, Value: %s\n", subStr);
#endif
				tokens[counterTokens].value = subStr;
				tokens[counterTokens].type = TYPE_CONST;
				counterTokens++;
				}


			else if (isValidVar(subStr) && !isEndChar(input[right - 1])) {
#ifdef LOG_VAL
				printf("Token: var, Value: %s\n", subStr);
#endif
				tokens[counterTokens].value = subStr;
				tokens[counterTokens].type = TYPE_VAR;
				counterTokens++;
				}


			else if (!isValidVar(subStr)&& !isEndChar(input[right - 1])) {
#ifdef LOG_VAL
				printf("\nToken: Unidentified, Value: %s\n", subStr);
#endif
				tokens[counterTokens].value = subStr;
				tokens[counterTokens].type = TYPE_UNREGISTER;
				counterTokens++;
				}

			left = right;
			}
		}
	tokens[counterTokens].type = TYPE_EOF;

	return tokens;
	}

void PrintTokens(Token *tokens) {
	int counter = 0;
	printf("\n_________________________\n");
	while(tokens[counter].type != TYPE_EOF) {
		printf("TOKEN%d [%s] : %s\n\n", counter, TokenString[tokens[counter].type],
		       tokens[counter].value);
		counter++;
		}

	printf("Num of tokens %d\n", counter - 1);
	printf("\n_________________________\n");
	}



void DestroyTokens(Token* tokens) {
	int counter = 0;
	while(tokens[counter].type != TYPE_EOF) {
		free(tokens[counter].value);
		counter++;

		}
	free(tokens);
	}
