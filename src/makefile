SHELL := /bin/bash
CC = gcc  
CFLAGS = -D_XOPEN_SOURCE=600 -O3 -Wall -Wextra -Werror -Wno-format -Wno-unused-variable -Wno-unused-function -std=c11 -ggdb


build: 
	$(CC) -static -o compiler main.c lex.c parser.c $(CFLAGS)


compile:
	./compiler -c code.blang code.vm

run: 
	./compiler -r code.vm

runtime:
	time ./compiler -r code.vm

dissasembly: 
	./compiler -d code.vm

writeHeader:
	./compiler -b code.vm

buildExeSdl: 
	$(CC) -o app buildexe.c -O3 -D_XOPEN_SOURCE=600  -lSDL2 

buildExe:
	$(CC) -o app buildexe.c -D_XOPEN_SOURCE=600 -O3 


all: build compile run



interpreter:
	./compiler -i code.vm

buildsdl:   
	$(CC) -o compiler main.c lex.c parser.c $(CFLAGS) -lSDL2 

allsdl: buildsdl compile run



clean :
	rm -rf compiler code.vm

 
