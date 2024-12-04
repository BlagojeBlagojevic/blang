#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<stdlib.h>
#include "utils_string.h"

#define NUM_OF_TOKENS 5
#define ERROR(...)  {fprintf(stderr, __VA_ARGS__); exit(-1);}
#define LOG_VAL
#include "lex.h"
//#include "utils_string.h"

enum {END_SCRIPT_TOKEN = 0, CONTINUE_TOKEN, START_WORD_TOKEN, END_WORD_TOKEN };

static inline int lexToken(char* input, int *left, int *right, int len,
                           int *counterTokens, int LIMIT_TOKENS, Token *tokens) {
	if(*counterTokens == LIMIT_TOKENS) {
		ERROR("We have excide max amount of tokens\n\n");
		}
	if (!isEndChar(input[*right]))
		*right = *right + 1;

	if (isEndChar(input[*right]) && *left == *right) {
		if (isOperator(input[*right])) {
			int next_right = *right+1;
			if(isDigit(input[next_right]) &&
			    ((input[*right] == '-') || (input[*right] == '+')) ) {
				while(isDigit(input[next_right]) && next_right <= len) {
					next_right++;
					}
				tokens[*counterTokens].value = returnSubstring(input, *right, next_right);
				tokens[*counterTokens].type = TYPE_CONST;
				tokens[*counterTokens].valType = I;
#ifdef LOG_VAL
				printf("Token: LogicOperator, Value: %s\n", tokens[*counterTokens].value);
				//system("pause");
#endif
				*right = next_right;
				*counterTokens = *counterTokens+1;
				}
			//TBD NEGATIVE FLOAT


			else if(isLogicOperator(input[*right])) {
				//IF OPERATOR CHECK FOR A = SIGN
					{

						{

						tokens[*counterTokens].type = TYPE_LOGICOPERATOR;
						tokens[*counterTokens].value = malloc(2*sizeof(char));
						tokens[*counterTokens].value[0] = input[*right];
						tokens[*counterTokens].value[1] = '\0';


#ifdef LOG_VAL
						printf("Token: LogicOperator, Value: %s\n", tokens[*counterTokens].value);
#endif
						*counterTokens = *counterTokens + 1;
						}
					}
				}
			else {
#ifdef LOG_VAL
				printf("Token: Operator, Value: %c\n", input[*right]);
#endif
				tokens[*counterTokens].type = TYPE_OPERATION;
				tokens[*counterTokens].value = malloc(2*sizeof(char));
				tokens[*counterTokens].value[0] = input[*right];
				tokens[*counterTokens].value[1] = '\0';
				*counterTokens = *counterTokens + 1;
				}

			}

		*right = *right + 1 ;
		*left = *right;
		}
	else if ((isEndChar(input[*right]) && (*left != *right)) || (*right == len
	         && (*left != *right))) {
		char* subStr = returnSubstring(input, *left, *right - 1);
		if(!strcmp(subStr, "endscript")) {
			free(subStr);
			return END_SCRIPT_TOKEN;
			}

		if (isKeyword(subStr)) {
#ifdef LOG_VAL
			printf("Token: Keyword, Value: %s\n", subStr);
#endif
			tokens[*counterTokens].value = subStr;
			tokens[*counterTokens].type = TYPE_KEYWORD;
			//printf("substr %s", tokens[counterTokens].value);
			*counterTokens = *counterTokens + 1;

			//IF KEYWORD IS ELSE ADD 1 IF = TOKENS
			if(!strcmp(subStr, keywords[KEYWORD_ELSE])) {
				//counterTokens--;
				//free(tokens[counterTokens].value);
				tokens[*counterTokens].value = malloc(2*sizeof(char));
				tokens[*counterTokens].value[0] = '0';
				tokens[*counterTokens].value[1] = '\0';
				tokens[*counterTokens].type = TYPE_CONST;
				*counterTokens = *counterTokens+1;
				tokens[*counterTokens].value = malloc(2*sizeof(char));
				tokens[*counterTokens].value[0] = '=';
				tokens[*counterTokens].value[1] = '\0';
				tokens[*counterTokens].type = TYPE_LOGICOPERATOR;
				*counterTokens = *counterTokens+1;
				tokens[*counterTokens].value = malloc(3*sizeof(char));
				strcpy(tokens[*counterTokens].value, "if");
				tokens[*counterTokens].type = TYPE_KEYWORD;
				*counterTokens = *counterTokens+1;

				}
			}
		else if(isStrEqual("word",subStr)) {
			printf("StartWord\n");
			free(subStr);
			*left = *right;
			return START_WORD_TOKEN;
			}

		else if(isStrEqual("endword", subStr)) {
			printf("EndWord\n");
			free(subStr);
			*left = *right;
			return END_WORD_TOKEN;
			}

		else if (isInteger(subStr)) {
			tokens[*counterTokens].value = subStr;
			tokens[*counterTokens].type = TYPE_CONST;
			tokens[*counterTokens].valType = U;
#ifdef LOG_VAL
			printf("Token: Integer, Value: %s\n", subStr);
#endif
			*counterTokens = *counterTokens+1;
			}
		else if(isFloat(subStr)) {
			tokens[*counterTokens].value = subStr;
			tokens[*counterTokens].type = TYPE_CONST;
			tokens[*counterTokens].valType = F;
#ifdef LOG_VAL
			printf("Token: Float, Value: %s\n", subStr);
#endif
			*counterTokens = *counterTokens+1;
			//system("pause");
			}
		else if (isValidString(subStr)) {
			int counter = strlen(subStr) - 2;
			while(subStr[counter] != '"') {
				tokens[*counterTokens].value  = malloc(2 * sizeof(char));
				tokens[*counterTokens].value[0] = subStr[counter];
				tokens[*counterTokens].value[1] = '\0';
				tokens[*counterTokens].type = TYPE_CONST;
				tokens[*counterTokens].valType = CH;
				*counterTokens = *counterTokens+1;
				counter--;
				}
			//TBD Maybe implement that we add a termainating string
			//tokens[*counterTokens].value  = malloc(2 * sizeof(char));
			//tokens[*counterTokens].value[0] = '0';
			//tokens[*counterTokens].value[1] = '\0';
			//tokens[*counterTokens].type = TYPE_CONST;
			//tokens[*counterTokens].valType = CH;
			//*counterTokens = *counterTokens+1;
#ifdef LOG_VAL
			printf("Token: String, Value: %s\n", subStr);
#endif
			}


		else if (isValidVar(subStr) && !isEndChar(input[*right - 1])) {
#ifdef LOG_VAL
			printf("Token: var, Value: %s\n", subStr);
#endif
			tokens[*counterTokens].value = subStr;
			tokens[*counterTokens].type = TYPE_VAR;
			tokens[*counterTokens].valType = tokens[*counterTokens-1].valType;
			*counterTokens = *counterTokens+1;
			}


		else if (!isValidVar(subStr)&& !isEndChar(input[*right - 1])) {
#ifdef LOG_VAL
			printf("\nToken: Unidentified, Value: %s\n", subStr);
#endif
			tokens[*counterTokens].value = subStr;
			tokens[*counterTokens].type = TYPE_UNREGISTER;
			*counterTokens = *counterTokens+1;
			}

		*left = *right;
		}
	return CONTINUE_TOKEN;
	}


Token* Tokeniser(char* input, Words *words) {

	Token* tokens = malloc(MAX_TOKENS*sizeof(char));

	int left = 0, right = 0;
	int len = strlen(input);
	int counterTokens = 0, counterWordTokens = 0;
	uint8_t isWordTokens = 0;
	while (right <= len && left <= right) {
		//printf("CounterTokens %d, left %d right %d\n", counterTokens, left, right);
		//system("pause");
		if(isWordTokens == 0) {
			int ret = lexToken(input, &left, &right, len, &counterTokens, MAX_TOKENS, tokens);
			if(ret == END_SCRIPT_TOKEN) {
				tokens[counterTokens].type = TYPE_EOF;
				break;
				}
			else if(ret == START_WORD_TOKEN) {
				right++;
				words[numOfUserDefiendWords].name = malloc(20);
				int count = 0;
				while(!isEndChar(input[right])) {
					words[numOfUserDefiendWords].name[count++] = input[right];
					//printf("Name %s\n", words[numOfUserDefiendWords].name);
					right++;
					}
				words[numOfUserDefiendWords].name[count++] = '\0';
				//left = right;
				//left = right-1;
				printf("Word %s\n", words[numOfUserDefiendWords].name);
				//system("pause");
				isWordTokens = 1;

				}
			else if(ret == END_WORD_TOKEN) {
				ERROR("End word token\n");

				}
			}
		else if(isWordTokens) {
			int ret = lexToken(input, &left, &right, len, &counterWordTokens,
			                   MAX_NUM_OF_TOKENS_IN_A_WORD, words[numOfUserDefiendWords].tokens);
			if(ret == END_SCRIPT_TOKEN) {
				counterWordTokens = 0;
				break;
				}
			else if(ret == START_WORD_TOKEN) {
				ERROR("We have end word token before start word token\n");
				}
			else if(ret == END_WORD_TOKEN) {
				//right--;
				//left = right;
				printf("EndWordToken\n");
				words[numOfUserDefiendWords].numOfTokens = counterWordTokens; //TAKE FIRST OUT
				counterWordTokens = 0;
				numOfUserDefiendWords++;
				isWordTokens = 0;
				}
			}
		}
	//PrintTokens(words[0].tokens);
	tokens[counterTokens].type = TYPE_EOF;
	return tokens;
	}

void PrintTokens(Token *tokens) {
	int counter = 0;
	printf("\n_________________________\n");
	while(tokens[counter].type != TYPE_EOF) {
		if(tokens[counter].value == NULL)
			return;
		printf("TOKEN%d [%s] : %s\n\n", counter, TokenString[tokens[counter].type],
		       tokens[counter].value);
		//system("pause");
		counter++;
		}

	printf("Num of tokens %d\n", counter - 1);
	printf("\n_________________________\n");
	}



void DestroyTokens(Token* tokens) {
	int counter = 0;
	while(tokens[counter].type != TYPE_EOF) {
		if(tokens[counter].value!=NULL)
			free(tokens[counter].value);
		printf("counter %d", counter);
		counter++;

		}
	free(tokens);
	}
