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
#ifndef SYMBOLTABLE_H_INCLUDED
#define SYMBOLTABLE_H_INCLUDED
/* Celkova tabulka symbolu je implementovana jako zasobnik tabulek symbolu jednotlivych bloku - hashovacich tabulek.
 * Pokud neni symbol nalezen v hashovaci tabulce odpovidajicimu vnorenemu bloku, je mozne vyhledavat postupne
 * v nadrazenych blocich az do globalniho.
 */

#include "ial.h"

struct symboltable_item
{
	struct symboltable_item *prev; // ukazatel na prvek patrici nadrazenemu bloku, je NULL pokud se jedna o prvni tedy globalni blok
	htab_t *htab;
};

typedef struct
{
	struct symboltable_item *first;
	struct symboltable_item *active;
}symboltable_t;

void symboltable_init(symboltable_t *s);
/* Inicializuje strukturu symboltable_t pred jejim prvnim pouzitim.
 * Tato funkce nesmi byt volana jindy nez jako prvni fuknce nad strukturou a
 * zadna jina funkce nesmi byt volana pred inicializaci.
 */

int symboltable_add(symboltable_t *s);
/* Vytvori lokalni tabulku pro novy blok, jehoz nadrazeny blok bude aktivni blok.
 * Po vraceni z funkce je aktivni nove vytvoreny blok.
 * Pokud nastala chyba pri alokaci, vraci 0, jinak 1.
 */

void symboltable_return(symboltable_t *s);
/* Vrati se do bloku ktery je nadrazeny prave aktivnimu.
 * Lokalni tabulka aktivniho bloku se uvloni.
 * Pokud zadny blok neni aktivni (tedy tabulka symbolu je prazdna), nic se nedeje.
 */

void symboltable_free(symboltable_t *s);
/* Uvolni vsechny tabulky a strukturu symboltable_t uvede zpet do stavu po inicializaci.
 */

htab_data* symboltable_symbolInsert(symboltable_t* s, const char* key);
/* Vlozi symbol key do prave aktivni tabulky. Vrati ukazatel na datovou slozku vytvoreneho prvku.
 * Pokud nastala chyba pri alokaci, vraci NULL.
 */

htab_data* symboltable_symbolSearchLocal(symboltable_t* s, const char* key);
/* Vyhleda symbol key v prave aktivni tabulce a vrati ukazatel na jeho datovou slozku.
 * Pokud symbol v aktivni tabulce neni, vraci NULL.
 */

htab_data* symboltable_symbolSearchGlobal(symboltable_t* s, const char* key);
/* Vyhleda symbol key v globalni tabulce (tedy ta, ktera byla vytvorena jako prvni), vrati ukazatel na jeho datovou slozku.
 * Pokud symbol v globalni tabulce neni, vraci NULL.
 */
 
htab_data* symboltable_symbolSearchAll(symboltable_t* s, const char* key);
/* Vyhleda symbol key v prave aktivni tabulce a vrati ukazatel na jeho datovou slozku.
 * Pokud symbol v aktivni tabulce neni, opakuje hledani v postupne nadrazenych blocich,
 * potom vraci datovou slozku symbolu nalezeneho v bloku nejblize aktivnimu.
 * Pokud symbol neni nalezen v aktivni a zadne z nadrazenych tabulek, vraci NULL.
 */

#endif // SYMBOLTABLE_H_INCLUDED