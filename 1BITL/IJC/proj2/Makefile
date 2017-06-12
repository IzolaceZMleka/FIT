#Projekt c.2 pro predmet IJC - Jazyk C
#Autor: Jan Pavlica - 1BIT, VUT FIT
#       xpavli78@stud.fit.vutbr.cz
#Datum: 27.4.2015

CFLAGS=-std=c99 -pedantic -Wall -Wextra
CPPFLAGS=-std=c++11 -pedantic -Wall
CC=gcc
CPP=g++

all: tail
tail: tail.c
	$(CC) -O2 -std=c99 -Wall -Wextra -pedantic tail.c -o tail

all: tail2
tail2: tail2.cc
	$(CPP) -O2 -std=c++11 -Wall -Wextra -pedantic tail2.cc -o tail2

all: libhtable.a
hash_function.o: hash_function.c htable.h
	$(CC) -O2 -std=c99 -Wall -Wextra -pedantic hash_function.c -c -o hash_function.o
htab_foreach.o: htab_foreach.c htable.h
	$(CC) -O2 -std=c99 -Wall -Wextra -pedantic htab_foreach.c -c -o htab_foreach.o
htab_statistics.o: htab_statistics.c htable.h
	$(CC) -O2 -std=c99 -Wall -Wextra -pedantic htab_statistics.c -c -o htab_statistics.o
htab_free.o: htab_free.c htable.h
	$(CC) -O2 -std=c99 -Wall -Wextra -pedantic htab_free.c -c -o htab_free.o
htab_clear.o: htab_clear.c htable.h
	$(CC) -O2 -std=c99 -Wall -Wextra -pedantic htab_clear.c -c -o htab_clear.o
htab_remove.o: htab_remove.c htable.h
	$(CC) -O2 -std=c99 -Wall -Wextra -pedantic htab_remove.c -c -o htab_remove.o
htab_lookup.o: htab_lookup.c htable.h
	$(CC) -O2 -std=c99 -Wall -Wextra -pedantic htab_lookup.c -c -o htab_lookup.o
htab_init.o: htab_init.c htable.h
	$(CC) -O2 -std=c99 -Wall -Wextra -pedantic htab_init.c -c -o htab_init.o

libhtable.a:hash_function.o htab_foreach.o htab_statistics.o htab_free.o htab_clear.o htab_remove.o htab_lookup.o htab_init.o htable.h
	ar -cvq libhtable.a hash_function.o htab_foreach.o htab_statistics.o htab_free.o htab_clear.o htab_remove.o htab_lookup.o htab_init.o
	ar -t libhtable.a

all: wordcount
wordcount: wordcount.o io.o libhtable.a
	$(CC) -O2 -o wordcount wordcount.o io.o libhtable.a
io.o: io.c
	$(CC) -O2 -std=c99 -Wall -Wextra -pedantic io.c -c -o io.o
wordcount.o: wordcount.c htable.h
	$(CC) -O2 -std=c99 -Wall -Wextra -pedantic wordcount.c -c -o wordcount.o


all: libhtable.so
htab_init-dynamic.o: htab_init.c htable.h
	$(CC) -O2 -std=c99 -Wall -Wextra -pedantic -c -fpic htab_init.c -o htab_init-dynamic.o
hash_function-dynamic.o: hash_function.c htable.h
	$(CC) -O2 -std=c99 -Wall -Wextra -pedantic -c -fpic hash_function.c -o hash_function-dynamic.o
htab_lookup-dynamic.o: htab_lookup.c  htable.h
	$(CC) -O2 -std=c99 -Wall -Wextra -pedantic -c -fpic htab_lookup.c -o htab_lookup-dynamic.o
htab_foreach-dynamic.o: htab_foreach.c htable.h
	$(CC) -O2 -std=c99 -Wall -Wextra -pedantic -c -fpic htab_foreach.c -o htab_foreach-dynamic.o
htab_remove-dynamic.o: htab_remove.c htable.h
	$(CC) -O2 -std=c99 -Wall -Wextra -pedantic -c -fpic htab_remove.c -o htab_remove-dynamic.o
htab_clear-dynamic.o: htab_clear.c htable.h
	$(CC) -O2 -std=c99 -Wall -Wextra -pedantic -c -fpic htab_clear.c -o htab_clear-dynamic.o
htab_free-dynamic.o: htab_free.c htable.h
	$(CC) -O2 -std=c99 -Wall -Wextra -pedantic -c -fpic htab_free.c -o htab_free-dynamic.o
htab_statistics-dynamic.o: htab_statistics.c htable.h
	$(CC) -O2 -std=c99 -Wall -Wextra -pedantic -c -fpic htab_statistics.c -o htab_statistics-dynamic.o

libhtable.so: htab_init-dynamic.o htab_lookup-dynamic.o htab_foreach-dynamic.o htab_remove-dynamic.o htab_clear-dynamic.o htab_free-dynamic.o htab_statistics-dynamic.o hash_function-dynamic.o 
	$(CC) -O2 -std=c99 -Wall -Wextra -pedantic -shared -fpic htab_init-dynamic.o htab_lookup-dynamic.o htab_foreach-dynamic.o htab_remove-dynamic.o htab_clear-dynamic.o htab_free-dynamic.o htab_statistics-dynamic.o hash_function-dynamic.o -o libhtable.so


all: wordcount-dynamic
wordcount-dynamic: wordcount.o io.o htable.h
	$(CC) -O2 -std=c99 -Wall -Wextra -pedantic -o wordcount-dynamic wordcount.o io.o -L. -lhtable

clean: 
	rm *.o "tail" "wordcount" "tail2" "wordcount-dynamic" *.so *.a

