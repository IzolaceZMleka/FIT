CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic

all: interpret bencs

interpret:
	$(MAKE) -C ../main-files
	
bencs: sort string-cpy

sort: sort.c ../main-files/builtin_func.c
	$(MAKE) -C ../main-files builtin_func.o ial.o
	$(CC) -O0 -DBENC_TYPE=\"$<\" benchmark.c ../main-files/builtin_func.o ../main-files/ial.o -o $@

string-cpy: string-cpy.c
	$(CC) -O3 -DBENC_TYPE=\"$<\" benchmark.c -o $@




clean:
	rm -f *.o *.exe