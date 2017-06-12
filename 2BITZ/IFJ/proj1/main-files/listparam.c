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
#include "listparam.h"
#include <string.h> // strcpy a strlen
#include <stdlib.h> // malloc a free

void listParam_init(TlistParam *l) {
	l->first = NULL;
	l->act = NULL;
	l->last = NULL;
	return;
}

void listParam_dispose(TlistParam *l) {
	TlistParamItem *tmp = l->first; // ukazatel na zbytek seznamu
	TlistParamItem *del; //ukazatel na ruseny prvek
	while (tmp != NULL) {
		del = tmp;
		tmp = tmp->next;
		free(del->param.name);
		free(del);
	}
	l->first = NULL;
	l->act = NULL;
	l->last = NULL;
	return;
}

int listParam_insertLast(TlistParam *l, int datatype, const char* name) {
	TlistParamItem *tmp;
	tmp = malloc (sizeof(TlistParamItem));
	if (tmp == NULL)
		return 0;
	tmp->param.name = malloc(strlen(name)+1); // +1 pro '\0' byte
	if (tmp->param.name == NULL) {
		free(tmp);
		return 0;
	}
	
	// alokace probehla uspesne
	tmp->param.datatype = datatype;
	strcpy(tmp->param.name, name);
	tmp->next = NULL;
	if (l->first == NULL) // seznam je prazdny
		l->first = tmp;
	else
		l->last->next = tmp;
	l->last = tmp;
	return 1;
}

void listParam_succ(TlistParam *l) {
	if (l->act != NULL)
		l->act = l->act->next;
	return;
}

void listParam_first(TlistParam *l) {
	l->act = l->first;
	return;
}

bool listParam_isactive(TlistParam *l) {
	return l->act != NULL; 
}

Tparam* listParam_read(TlistParam *l) {
	if (l->act == NULL)
		return NULL;
	else
		return &(l->act->param);
}
