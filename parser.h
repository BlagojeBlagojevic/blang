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
#define MAX_VARS 500
#define MAX_VARS_NAME 100

typedef struct {
	int adress[MAX_VARS];
	char* name[MAX_VARS];
	uint8_t type[MAX_VARS];
	int sp;
	} VarStack;
	
void Parser(Token *tokens, Bvm *bvm);
#endif
