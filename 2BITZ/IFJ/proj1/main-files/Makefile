#/* ***************************************************************** */
#
#	projekt: Implementace interpretu imperativniho jazyka IFJ15
#	autori: Jan Velecky, Tomas Zahradnicek, Adam Piecek
#	xvelec07: Jan Velecky
#	xzahra24: Tomas Zahradnicek
#	xpiece00: Adam Piecek
#	xpavli78: Jan Pavlica
#/* ***************************************************************** */
CC=gcc
LD=ld
LDFLAGS=-flto
ifdef debug
    CFLAGS=-std=c99 -Wall -Wextra -pedantic -g
else
    CFLAGS=-std=c99 -Wall -Wextra -pedantic -DNDEBUG -O3 -flto
endif
# pri debugovani spoustet jako 'make debug=<cokoliv>', napr. 'make debug=1' nebo 'make debug=yes'
# 'make' bez promenne debug vytvori ostrou verzi
CCF=$(CC) $(CFLAGS) -c

.PHONY: clean hand_in
# spustitelny soubor interpretu
main: main.o parser.o expression.o instructions.o symboltable.o lexer.o builtin_func.o errors.o listparam.o strvars.o interpret.o ial.o read.o

builtin_func.o: builtin_func.c ial.h builtin_func.h errors.h
errors.o: errors.c errors.h instructions.h lexer.h symboltable.h expression.h
read.o: read.c read.h builtin_func.h errors.h
expression.o: expression.c lexer.h builtin_func.h instructions.h \
 symboltable.h ial.h listparam.h strvars.h expression.h errors.h \
 expressionsemantic.inc.c expressionop.inc.c expressioncmp.inc.c
expressioncmp.inc.o: expressioncmp.inc.c
expressionop.inc.o: expressionop.inc.c
expressionSemantic.inc.o: expressionsemantic.inc.c expressionop.inc.c \
 expressioncmp.inc.c
ial.o: ial.c listparam.h strvars.h instructions.h
instructions.o: instructions.c instructions.h errors.h
interpret.o: interpret.c interpret.h instructions.h ial.h listparam.h \
 strvars.h builtin_func.h errors.h read.h
lexer.o: lexer.c lexer.h builtin_func.h instructions.h
listparam.o: listparam.c listparam.h
main.o: main.c parser.h symboltable.h ial.h listparam.h strvars.h \
 instructions.h errors.h interpret.h
parser.o: parser.c symboltable.h ial.h listparam.h strvars.h \
 instructions.h lexer.h builtin_func.h parser.h errors.h expression.h
strvars.o: strvars.c strvars.h
symboltable.o: symboltable.c symboltable.h ial.h listparam.h strvars.h \
 instructions.h

clean:
	rm -f *.o *.exe main
	
hand_in:
	zip ../xvelec07.zip *.c *.h rozdeleni rozsireni Makefile