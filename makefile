SHELL := /bin/bash
CC = gcc  
CFLAGS = -O3 -Wall -Wextra  -Wno-format -Wno-unused-variable -Wno-unused-function -std=c11 -ggdb

build: 
	$(CC) -o compiler main.c lex.c parser.c $(CFLAGS)

compile:
	./compiler -c code.blang code.vm

run: 
	./compiler -r code.vm

runtime:
	time ./compiler -r code.vm

all: build compile run

clean :
	rm -rf compiler code.vm

 

