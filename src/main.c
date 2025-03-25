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

#define MAX_SIZE 10000000
#define SIZE_INTERPRETER 1000
#define MAX_NUM_OF_WORDS 1000
//C:> 


static Arena mainArena = {0};

int main(int argc, char **argv) {

	if(argc <= 1) {
		printf("Usage:  Compile      -c <path to program> <path to save>\n\tRun \t     -r <path to saved>\n\t");
		printf("Disassemble  -d <path to saved>\n\tWrite header -b <path to save>\n\tInterpreter  -i <none>\n");
		return 0 ;
		exit(EXIT_SUCCESS);
		}
	if(!strcmp(argv[1], "-c") && (argc > 3)) {
		char *code = arena_alloc(&mainArena, MAX_SIZE * sizeof(char));
		memset(code, '\0', sizeof(char) * MAX_SIZE);
		FILE *f = fopen(argv[2], "r");
		if(f == NULL)
			return 60;
		size_t s = fread(code, sizeof(char), MAX_SIZE, f);
		(void)s;
		fclose(f);
		Bvm vm = initBVM();
		Words *words = arena_alloc(&mainArena, MAX_NUM_OF_WORDS * sizeof(Words));
		for (size_t i = 0; i < MAX_NUM_OF_WORDS; i++){
			words[i].tokens = arena_alloc(&mainArena, sizeof(Token)*MAX_NUM_OF_TOKENS_IN_A_WORD);
		}
		Token *t = Tokeniser(code, words, &mainArena);
		Parser(t, words, &vm, 0, &mainArena);
		programToBin(argv[3], vm.instruction, vm.numOfInstructions);
		freeBvm(&vm);
		arena_free(&mainArena);
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
	else if(!strcmp(argv[1], "-d") && (argc > 2)) {
		Bvm vm = initBVM();
		//binToProgram(argv[2], vm.instruction);
		bitToDisasemly(argv[2], vm.instruction);
		freeBvm(&vm);
		return 0;
		}
	else if(!strcmp(argv[1], "-b") && (argc > 2)){
		FILE *input = fopen(argv[2], "r+");
		FILE *output  = fopen("generatedInst.h", "w");
		//char *code = arena_alloc(&mainArena, sizeof(char)*MAX_SIZE); 
		if(input == NULL || output == NULL){
			ERROR_BREAK("Error in file\n");
		}
		char buffer[2048];
		memset(buffer, '\0', 2048);
		snprintf(buffer, 2048, "Instruction inst[] = {\n");
		Bvm vm = initBVM();
		
		fprintf(output, "%s", buffer);
		//binToProgram(argv[2], vm.instruction);
		bitToDisasemly("code.vm", vm.instruction);
		for(u64 i = 0; vm.instruction[i].type != END; i++){
			fprintf(output, "{.type = %u, .operand = %lld},\n", 
			vm.instruction[i].type, vm.instruction[i].operand );
				
		}

		uint64_t counter = 0;
		freeBvm(&vm);
		fprintf(output, "{.type = %u, .operand = %lld}\n", 
			END, 123 );
		fprintf(output, "};");
		fclose(output);
		fclose(input);
	}
    else if(!strcmp(argv[1], "-i")){
		Bvm vm = initBVM();
		char *code = malloc((SIZE_INTERPRETER+1) * sizeof(char));
		memset(code, '\0', sizeof(char) * SIZE_INTERPRETER);
		Words *words = arena_alloc(&mainArena, MAX_NUM_OF_WORDS * sizeof(Words));
		for (size_t i = 0; i < MAX_NUM_OF_WORDS; i++){
			words[i].tokens = arena_alloc(&mainArena, sizeof(Token)*MAX_NUM_OF_TOKENS_IN_A_WORD*1000);
		}
		
		Token *t = NULL;
		printf("\n\n\nHeap Vars are not allowed Tokenizer resets VarStack\nUse a word to def a var\n");
		printf("Example:\nword ptrA 1234 endword\nword valA ptrA @ endword \n");
		printf("0 ptrA ?? drop  while valA 1 + ptrA ?? valA 10 = if breakloop end valA print endloop\n"); 

		printf("\n-------------Runtime-------------\n");
		
		while(1){
			printf("\n> ");
			memset(code, '\0', sizeof(char)*SIZE_INTERPRETER);
			code[0] = '\n';
			char *c =  fgets(&code[1], SIZE_INTERPRETER*sizeof(char), stdin);
			(void)c;
			//
			strcat(code, " endscript .");
			t = Tokeniser(code, words, &mainArena);
			Parser(t, words, &vm, vm.numOfInstructions, &mainArena);
			vm.isRuning = TRUE;
			loop(&vm);
			vm.IP = 0;
			vm.numOfInstructions = 0;

		}
		arena_free(&mainArena);
	  }
		
	}
