SHELL := /bin/bash
CC = gcc  
CFLAGS = -D_XOPEN_SOURCE=600 -O0 -Wall -Wextra -Werror -Wno-format -Wno-unused-variable -Wno-unused-function -std=c11 -ggdb


build: 
	$(CC) -static -o compiler src/main.c src/lex.c src/parser.c $(CFLAGS)


compile:
	./compiler -c src/code.blang code.vm

run: 
	./compiler -r code.vm

runtime:
	time ./compiler -r code.vm

dissasembly: 
	./compiler -d code.vm

writeHeader:
	./compiler -b code.vm

buildExeSdl: 
	$(CC) -o app src/buildexe.c -O3 -D_XOPEN_SOURCE=600  -lSDL2 

buildExe:
	$(CC) -o app src/buildexe.c -D_XOPEN_SOURCE=600 -O3 

emitAssembly:
	 $(CC) -O3 -S -fverbose-asm -O src/buildexe.c


all: build compile run

interpreter:
	./compiler -i code.vm

buildsdl:   
	$(CC) -o compiler src/main.c src/lex.c src/parser.c $(CFLAGS) -lSDL2 

allsdl: buildsdl compile run


buildNativeC:
	$(CC)  -o buildNative src/compiler/buildNative.c 

executeNative:
	./buildNative

clean :
	rm -rf compiler code.vm

 

