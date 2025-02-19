#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define LOG_VAL
#define BVM_IMPLEMENTATION


#include "utils_string.h"
#include "bvm.h"
#include "lex.h"
#include "parser.h"

#define ARENA_IMPLEMENTATION
#include "arena.h"

//#include<windows.h>

#define MAX_SIZE 100000000
#define MAX_NUM_OF_WORDS 10000
//> 
// https://www.tutorialspoint.com/c_standard_library/string_h.htm

static Arena mainArena = {0};

int main(int argc, char **argv) {
	//printf("STDOUT %d", (int)stdout);
	//write(1, "Nesto", 4);
	if(argc <= 1) {
		printf("Usage:  Compile -c <path to program> <path to save>\n\tRun -r <path to saved>\n");
		return 0 ;
		exit(EXIT_SUCCESS);
		}
	if(!strcmp(argv[1], "-c") && (argc > 3)) {
		char *code = arena_alloc(&mainArena, MAX_SIZE * sizeof(char));
		memset(code, '\0', sizeof(char) * MAX_SIZE);
		FILE *f = fopen(argv[2], "r");
		if(f == NULL)
			return 60;
		fread(code, sizeof(char), MAX_SIZE, f);
	//	printf("\n%s\n\n", code);
		fclose(f);
		Bvm vm = initBVM();
		Words *words = arena_alloc(&mainArena, MAX_NUM_OF_WORDS * sizeof(Words));
		for (size_t i = 0; i < MAX_NUM_OF_WORDS; i++){
			words[i].tokens = arena_alloc(&mainArena, sizeof(Token)*MAX_NUM_OF_TOKENS_IN_A_WORD);
			//words[i].name = NULL;
		}
		Token *t = Tokeniser(code, words, &mainArena);
		PrintTokens(t);
		Parser(t, words, &vm, 0, &mainArena);
		//system("pause");
		//TBD rewrite to a dynamic arrays 
		//DestroyTokens(t);
		programToBin(argv[3], vm.instruction, vm.numOfInstructions);
		//system("pause");
		freeBvm(&vm);
		arena_free(&mainArena);
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
		freeBvm(&vm);
		return 0;
		}
      else if(!strcmp(argv[1], "-i")){
		Bvm vm = initBVM();
		char *code = malloc(MAX_SIZE * sizeof(char));
		//char *stored = malloc(MAX_SIZE * sizeof(char));
		//memset(stored, '\0', MAX_SIZE*sizeof(char));
		memset(code, '\0', sizeof(char) * MAX_SIZE);
		//int counter = 0;
		//FILE *f = fopen("inter.blang", "w+");
		Words *words = arena_alloc(&mainArena, MAX_NUM_OF_WORDS * sizeof(Words));
		for (size_t i = 0; i < MAX_NUM_OF_WORDS; i++){
			words[i].tokens = arena_alloc(&mainArena, sizeof(Token)*MAX_NUM_OF_TOKENS_IN_A_WORD*1000);
		}
		
		Token *t = NULL;
		printf("\n\n\nHeap Vars are not allowed Tokenizer resets VarStack\nUse a word to def a var\n");
		printf("Example:\nword ptrA 1234 endword\nword valA ptrA @ endword \n");
		printf("0 ptrA ?? drop  while valA 1 + ptrA ?? valA 10 = if breakloop end valA print endloop\n");
		//printf("0 ptrA ?? drop");  
		//printf("while valA 1 + ptrA ??\n"); 
		//printf("valA 10 = if\n"); 
    	//printf("\tbreakloop\n"); 
		//printf("end\nvalA\nprint\nendloop"); 

		printf("\n-------------Runtime-------------\n");
		while(1){
			printf("\n> ");
			//code[0] = '0';
			memset(code, '\0', sizeof(char)*MAX_SIZE);
			code[0] = '\n';
			//strcpy(&code[2], stored);
			gets(&code[1]);
			//
			strcat(code, " endscript .");
			//printf("\ncode: %s\n", code);
			fflush(stdin);
			t = Tokeniser(code, words, &mainArena);
			Parser(t, words, &vm, vm.numOfInstructions, &mainArena);
			//printf("\nnum of instructions %lld ip = %d\n", 
			//vm.numOfInstructions, vm.IP);
			//printf("\n-------------Runtime-------------\n");
			vm.isRuning = TRUE;
			loop(&vm);
			//vm.numOfInstructions += vm.IP;
			//vm.IP-= 2;
			vm.IP = 0;
			vm.numOfInstructions = 0;
		///	vm.IP--;
			//printf("\n---------------------------------\n");
		
			//arena_free(&mainArena);
		}
		arena_free(&mainArena);
	  }
		
	}
