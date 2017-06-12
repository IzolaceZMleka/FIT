CLAGS=-g -O2 
PFLAGS=-lsimlib -lm -Wextra -pedantic -Wall
 
CC = g++
AUTHOR = 05_xpiece00_xpavli78
FILES = hasici.cpp Makefile dokumentace.pdf
NUMBERS = {16..18}

all: hasici

hasici: hasici.cpp
	$(CC) $(CFLAGS) -o $@ hasici.cpp $(PFLAGS)

run:
	rm -f *.out
	
	./hasici 25 1 5 experiment1.out		#defaultni hodnoty
	./hasici 25 2 5 experiment1_detailed.out		#detailni vypis hodnot
	
	 #zkoumani poctu stanic
	for number in 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 ; do \
		./hasici $$number 3 5 >> experiment2.out; \
	done
	 #zkoumani vyuziti nejblizsich stanic, dojezdu pod 20 minut
	for number in 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 ; do \
		./hasici $$number 4 5 >> experiment3.out; \
	done
	
clean:
	rm -f hasici *.tar.gz *.zip *.out

zip:
	zip $(AUTHOR).zip $(FILES)
