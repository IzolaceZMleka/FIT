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
#include <stdlib.h>
#include "symboltable.h"

void symboltable_init(symboltable_t *s)
{
	s->first = NULL;
	s->active = NULL;
	return;
}

int symboltable_add(symboltable_t *s)
{
	struct symboltable_item *tmp;
	tmp = malloc(sizeof(struct symboltable_item));
	if (tmp == NULL) // selhala alokace dalsiho prvku v seznamu
		return 0;
	tmp->htab = htab_init();
	if (tmp->htab == NULL) { // selhala alokace hashovaci tabulky
		free(tmp);
		return 0;
	}
	tmp->prev = s->active; // doposud aktivni blok bude nadrazeny prave vytvorenemu bloku, muze byt NULL pokud se pridava prvni prvek
	if (s->first == NULL)
		s->first = tmp;
	s->active = tmp;
	return 1;
}
void symboltable_return(symboltable_t *s)
{
	struct symboltable_item *del;
	if (s->active != NULL) {
		del = s->active;
		s->active = s->active->prev;
		if (s->active == NULL)
			s->first = NULL;
		htab_free(del->htab);
		free(del);
	}
	return;
}
void symboltable_free(symboltable_t *s)
{
	struct symboltable_item *tmp = s->active;
	struct symboltable_item *del; // ukazatel na prvek ktery bude smazan
	while(tmp != NULL)
	{
		del = tmp;
		tmp = tmp->prev;
		htab_free(del->htab);
		free(del);
	}
	s->active = NULL;
	s->first = NULL;
	return;
}

htab_data* symboltable_symbolInsert(symboltable_t* s, const char* key)
{
	if (s->active == NULL)
		return NULL;
	return htab_insert(s->active->htab ,key);
}

htab_data* symboltable_symbolSearchLocal(symboltable_t* s, const char* key)
{
	if (s->active == NULL)
		return NULL;
	return htab_search(s->active->htab ,key);
}

htab_data* symboltable_symbolSearchGlobal(symboltable_t* s, const char* key)
{
	if (s->first == NULL)
		return NULL;
	return htab_search(s->first->htab , key);
}

htab_data* symboltable_symbolSearchAll(symboltable_t* s, const char* key)
{
	struct symboltable_item *act = s->active;
	htab_data *ret;
	while (act != NULL) {
		ret = htab_search(act->htab ,key);
		if (ret != NULL)
			return ret;
		act = act->prev;
	}
	return NULL;
}