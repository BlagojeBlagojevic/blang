#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define LOG_VAL
#define BVM_IMPLEMENTATION
#include "bvm.h"
#include "lex.h"
#include "parser.h"


int main(int argc, char **argv) {


	if(argc <= 1) {
		printf("Usage:  Compile -c <path to program> <path to save>\n\tRun -r <path to saved>\n");
		return 0 ;
		exit(EXIT_SUCCESS);
		}
	if(!strcmp(argv[1], "-c") && (argc > 3)) {
		char *code = malloc(MAX_TOKENS * sizeof(char));
		memset(code, '\0', sizeof(char) * MAX_TOKENS);
		FILE *f = fopen(argv[2], "r");
		if(f == NULL)
			return 60;
		fread(code, sizeof(char), MAX_TOKENS, f);
		printf("\n%s\n\n", code);
		fclose(f);
		Bvm vm = initBVM();
		Words *words = malloc(100 * sizeof(Words));
		Token *t = Tokeniser(code, words);
		PrintTokens(t);
		Parser(t, words, &vm);
		//system("pause");
		//TBD rewrite to a dynamic arrays 
		//DestroyTokens(t);
		programToBin(argv[3], vm.instruction, vm.numOfInstructions);
		//system("pause");
		return 0;
		}
	else if(!strcmp(argv[1], "-r") && (argc > 2)) {
		Bvm vm = initBVM();
		binToProgram(argv[2], vm.instruction);
		printf("\n-------------Runtime-------------\n");
		loop(&vm);
		printf("\n---------------------------------\n");
		//system("pause");
		return 0;

		}

	}
