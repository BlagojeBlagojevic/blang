#ifndef PARSER_H
#define PARSER_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string.h>
#include<stdint.h>
#include "lex.h"
#define LOG_STACK
#define BVM_IMPLEMENTATION
#include "bvm.h"
#define MAX_VARS 100000
#define MAX_VARS_NAME 20

//TBD Dynamic arr
typedef struct {
	int *adress;
	char* *name;
	uint8_t *type;
	int sp;
	} VarStack;
	
void Parser(Token *tokens, Words *words , Bvm *bvm, Arena *mainArena);
#endif
