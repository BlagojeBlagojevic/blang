SHELL := /bin/bash
CC = gcc  
CFLAGS = -Ofast -Wall -Wextra  -Wno-format -Wno-unused-variable -Wno-unused-function -std=c11 -ggdb

build: 
	$(CC) -o compiler main.c lex.c parser.c $(CFLAGS)


compile:
	./compiler -c code.blang code.vm

run: 
	./compiler -r code.vm

runtime:
	time ./compiler -r code.vm

dissasembly: 
	./compiler -d code.vm


all: build compile run



interpreter:
	./compiler -i code.vm

buildsdl: 
	$(CC) -o compiler main.c lex.c parser.c -s  $(CFLAGS) -lSDL2

allsdl: buildsdl compile run


clean :
	rm -rf compiler code.vm

 
