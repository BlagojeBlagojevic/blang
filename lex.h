#ifndef LEX_H
#define LEX_H

#include "utils_string.h"

#define NUM_OF_TOKENS 5
#define ERROR(...)  {fprintf(stderr, __VA_ARGS__); exit(-1);}

#define MAX_TOKENS 10000
#define MAX_LENGHT  100


typedef enum TokenType_t {
	TYPE_CONST = 0,
	TYPE_VAR,
	TYPE_KEYWORD,
	TYPE_UNREGISTER,
	TYPE_OPERATION,
	TYPE_LOGICOPERATOR,
	TYPE_EOF,

	} TokenType;

static const char* TokenString[] = {
	"CONST",
	"VAR",
	"KEYWORD",
	"UNREGISTER",
	"OPERATION",
	"LOGICOPERATOR",
	"EOF",
	};

//static_assert(NUM_OF_TOKENS==TYPE_EOF, "we have more tokens then max");

typedef struct {
	TokenType type;
	char* value;     //IT EXIST TRUE ALL LIFTIME OF A PROGRAM
	int jumpValue;
	} Token;



Token* Tokeniser(char* input);
void DestroyTokens(Token* tokens);
void PrintTokens(Token* tokens);

#endif
