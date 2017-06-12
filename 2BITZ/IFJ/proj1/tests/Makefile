CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic 
CCF=$(CC) $(CFLAGS)

all: interpret tests

interpret:
	$(MAKE) -C ../main-files
	
tests: quick-sort-test booyer-moore-test other-func-test read-func-test

quick-sort: ../main-files/ial.c ../main-files/errors.c quick-sort.o
	$(MAKE) -C ../main-files ial.o errors.o
	$(CC) -lm ../main-files/ial.o ../main-files/errors.o quick-sort.o -o $@
quick-sort.o: quick-sort.c
	$(CCF) -c $? -o $@

booyer-moore: ../main-files/builtin_func.c ../main-files/ial.c ../main-files/errors.c booyer-moore.o
	$(MAKE) -C ../main-files builtin_func.o errors.o ial.o
	$(CC) ../main-files/builtin_func.o ../main-files/ial.o ../main-files/errors.o booyer-moore.o -o $@
booyer-moore.o: booyer-moore.c
	$(CCF) -c $? -o $@

other-func: ../main-files/builtin_func.c ../main-files/ial.c ../main-files/errors.c other-func.o
	$(MAKE) -C ../main-files builtin_func.o errors.o ial.o
	$(CC) ../main-files/builtin_func.o ../main-files/ial.o ../main-files/errors.o other-func.o -o $@
other-func.o: other-func.c
	$(CCF) -c $? -o $@

read-func: ../main-files/read.c ../main-files/errors.c read-func.o
	$(MAKE) -C ../main-files read.o errors.o
	$(CC) ../main-files/read.o ../main-files/errors.o read-func.o -o $@
read-func.o: read-func.c
	$(CCF) -c $? -o $@

# TESTS
# test by nemel vratit neserazene pole
quick-sort-test: quick-sort
	./quick-sort

booyer-moore-test: booyer-moore
	./booyer-moore

other-func-test: other-func
	./other-func

read-func-test: read-func
	./read-func

clean:
	rm -f *.o *.exe
