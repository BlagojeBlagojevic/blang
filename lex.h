#ifndef LEX_H
#define LEX_H

#include "utils_string.h"
#include<stdint.h>
#define NUM_OF_TOKENS 5
#define ERROR(...)  {fprintf(stderr, __VA_ARGS__); exit(-1);}

#define MAX_TOKENS 5000
#define MAX_LENGHT  20
#define MAX_NUM_OF_TOKENS_IN_A_WORD 30

typedef enum TokenType_t {
	TYPE_CONST = 0,
	TYPE_VAR,
	TYPE_KEYWORD,
	TYPE_USER_DEFINE_KEYWORD,
	TYPE_UNREGISTER,
	TYPE_OPERATION,
	TYPE_LOGICOPERATOR,
	TYPE_EOF,

	} TokenType;

static const char* TokenString[] = {
	"CONST",
	"VAR",
	"KEYWORD",
	"TYPE_USER_DEFINE_KEYWORD",
	"UNREGISTER",
	"OPERATION",
	"LOGICOPERATOR",
	"EOF",
	};

//static_assert(NUM_OF_TOKENS==TYPE_EOF, "we have more tokens then max");
enum {I , F, U, CH, STR};



typedef struct {
	TokenType type;
	char* value;     //IT EXIST TRUE ALL LIFTIME OF A PROGRAM
	uint8_t valType;
	} Token;

typedef struct {
	//TBD malloc alocation
	Token tokens[MAX_NUM_OF_TOKENS_IN_A_WORD];
	int numOfTokens;
	char *name;
}Words;

static int numOfUserDefiendWords = 0;



Token* Tokeniser(char* input, Words *words);
void DestroyTokens(Token* tokens);
void PrintTokens(Token* tokens);

#endif
