CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -g
PROGS=test

all: $(PROGS)

test: test.o lexer.o
	$(CC) $^ -o $@
	
test.o: test.c

lexer.o: ../main-files/lexer.c ../main-files/lexer.h ../main-files/builtin_func.h
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o *.exe $(PROGS)