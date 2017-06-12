CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -g
PROGS=test

all: $(PROGS)

test: test.o instructions.o
	$(CC) $(CFLAGS) $^ -o $@
	
test.o: test.c

instructions.o: ../main-files/instructions.c ../main-files/instructions.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(PROGS) *.o