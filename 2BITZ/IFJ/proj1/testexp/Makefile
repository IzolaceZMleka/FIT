CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -g

.PHONY: clean

test: test.o instructions.o expression.o lexer.o ../main-files/errors.c
	$(CC) $(CFLAGS) $^ -o $@
	
test.o: test.c

expression.o: ../main-files/expression.c ../main-files/expression*.*
	$(CC) $(CFLAGS) -c $< -o $@

lexer.o: ../main-files/lexer.c
	$(CC) $(CFLAGS) -c $< -o $@

instructions.o: ../main-files/instructions.c ../main-files/instructions.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(PROGS) *.o *.exe