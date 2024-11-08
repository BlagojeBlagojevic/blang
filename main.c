#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.c"
#include "parser.c"

#define LOG_STACK
#define BVM_IMPLEMENTATION
#include "bvm.h"

int main()
{
	char *code = malloc(MAX_TOKENS * sizeof(char));

	//strcpy(code, "2 3 + print 5 * print");
	//code[strlen(code) - 2] = '\n';
	FILE *f = fopen("code.txt", "r");
	if(f == NULL)
		return 60;
	fread(code, sizeof(char), MAX_TOKENS, f);
	printf("\n%s\n\n", code);
	fclose(f);
	Token *t = Tokeniser(code);
	PrintTokens(t);
	Parser(t, "vm.txt");
	//system("pause");
	DestroyTokens(t);
	//return 0;
	Bvm vm = initBVM();
	vm.numOfInstructions = textToProgram("vm.txt", vm.instruction);
	printf("\n-------------Runtime-------------\n");
	loop(&vm);
	printf("\n---------------------------------\n");
	//system("pause");

	return 0;

}
