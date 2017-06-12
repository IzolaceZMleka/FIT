CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -g
PROGS=test

all: $(PROGS)

test: parser.o test.o lexer.o symboltable.o htable.o listParam.o strvars.o errors.o instructions.o
	$(CC) $(CFLAGS) $^ -o $@

test.o: test.c

parser.o: ../main-files/parser.c ../main-files/parser.h ../main-files/lexer.h
	$(CC) $(CFLAGS) -c $< -o $@

symboltable.o: ../main-files/symboltable.c ../main-files/symboltable.h
	$(CC) $(CFLAGS) -c $< -o $@

htable.o: ../main-files/htable.c ../main-files/htable.h ../main-files/listparam.h
	$(CC) $(CFLAGS) -c $< -o $@

listParam.o: ../main-files/listparam.c ../main-files/listparam.h
	$(CC) $(CFLAGS) -c $< -o $@

lexer.o: ../main-files/lexer.c ../main-files/lexer.h
	$(CC) $(CFLAGS) -c $< -o $@

strvars.o: ../main-files/strvars.c ../main-files/strvars.h
	$(CC) $(CFLAGS) -c $< -o $@

errors.o: ../main-files/errors.c ../main-files/errors.h
	$(CC) $(CFLAGS) -c $< -o $@

instructions.o: ../main-files/instructions.c ../main-files/instructions.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(PROGS) *.exe *.o