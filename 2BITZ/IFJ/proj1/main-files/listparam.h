/* ***************************************************************** */
/*
	projekt: Implementace interpretu imperativniho jazyka IFJ15
	autori: Tomas Zahradnicek
	xvelec07: Jan Velecky
	xzahra24: Tomas Zahradnicek
	xpiece00: Adam Piecek
	xpavli78: Jan Pavlica
*/
/* ***************************************************************** */
#ifndef LISTPARAM_H_INCLUDED
#define LISTPARAM_H_INCLUDED

#include <stdbool.h>

typedef struct {
	int datatype;
	char* name;
} Tparam;

typedef struct TlistParamItem {
	Tparam param;
	struct TlistParamItem *next;
} TlistParamItem;

typedef struct TlistParam {
	TlistParamItem *first;
	TlistParamItem *act;
	TlistParamItem *last;
} TlistParam;

void listParam_init(TlistParam *l);
/* inicializuje seznam pro parametry, funkce musi byt zavolana jako prvni funkce nad seznamem */

void listParam_dispose(TlistParam *l);
/* uvolni cely seznam a jeho polozky, nastavi seznam do stavu po inicializaci */

int listParam_insertLast(TlistParam *l, int datatype, const char* name);
/* vlozi prvek na konec seznamu, vraci 0 pri chybe */

void listParam_succ(TlistParam *l);
/* posune se na nasledujci polozku v seznamu */

void listParam_first(TlistParam *l);
/* posune se na prvni polozku v seznamu */

bool listParam_isactive(TlistParam *l);
/* vrati zda je seznam aktivni */

Tparam* listParam_read(TlistParam *l);
/* vrati ukazatel na aktivni prvek, NULL pokud zadny neni aktivni */

#endif // LISTPARAM_H_INCLUDED