#ifndef LEX_H
#define LEX_H

#include "utils_string.h"
#include<stdint.h>
#define NUM_OF_TOKENS 5
#define ERROR(...)  {fprintf(stderr, __VA_ARGS__); exit(-1);}

#define MAX_TOKENS 50000
#define MAX_LENGHT  30
#define MAX_NUM_OF_TOKENS_IN_A_WORD 1000



typedef enum TokenType_t {
	TYPE_CONST = 0,
	TYPE_VAR,
	TYPE_KEYWORD,
	TYPE_USER_DEFINE_KEYWORD,
	TYPE_UNREGISTER,
	TYPE_OPERATION,
	TYPE_LOGICOPERATOR,
	TYPE_DEVICE,
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
	"DEVICE",
	"EOF",
	};

//static_assert(NUM_OF_TOKENS==TYPE_EOF, "we have more tokens then max");
enum {I, F, U, CH, STR};



typedef struct {
	TokenType type;
	char* value;     //IT EXIST TRUE ALL LIFTIME OF A PROGRAM
	uint8_t valType;
	}Token;

//DYNAMIC ARRAY FOR A TOKENS name dArr_Token




typedef struct {
	//TBD arena_alloc alocation
	Token *tokens; //TBD AS A DYNAMIC ARRAY
	int numOfTokens;
	char *name;
	} Words;

static int numOfUserDefiendWords = 0;


//DYNAIC ARRAY FOR A WORD name dArr_Words

Token* Tokeniser(char* input, Words *words, Arena *mainArena);
void DestroyTokens(Token* tokens);
void PrintTokens(Token* tokens);

#endif
