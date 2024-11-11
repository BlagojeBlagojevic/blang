#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//#define LOG_VALUE
//#define LOG_STACK
#define BVM_IMPLEMENTATION
#include "bvm.h"
#include "lex.c"
#include "parser.c"

int main() {
	char *code = malloc(MAX_TOKENS * sizeof(char));
	memset(code, '\0', sizeof(char) * MAX_TOKENS);
	//strcpy(code, "2 3 + print 5 * print");
	//code[strlen(code) - 2] = '\n';
	FILE *f = fopen("code.txt", "r");
	if(f == NULL)
		return 60;
	fread(code, sizeof(char), MAX_TOKENS, f);
	printf("\n%s\n\n", code);
	fclose(f);
	Bvm vm = initBVM();
	Token *t = Tokeniser(code);
	PrintTokens(t);
	//system("pause");
	Parser(t, &vm);
	//programToBin("vm.bin", &vm.instruction, vm.numOfInstructions);
	
	//return 0;
	
	//return 0;
	//vm.numOfInstructions = textToProgram("vm.txt", vm.instruction);
	printf("\n-------------Runtime-------------\n");
	loop(&vm);
	printf("\n---------------------------------\n");
	//system("pause");
	DestroyTokens(t);
	return 0;

	}
