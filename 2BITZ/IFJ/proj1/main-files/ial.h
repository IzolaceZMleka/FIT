/* ***************************************************************** */
/*
	projekt: Implementace interpretu imperativniho jazyka IFJ15
	autori: Jan Velecky, Tomas Zahradnicek, Adam Piecek
	xvelec07: Jan Velecky
	xzahra24: Tomas Zahradnicek
	xpiece00: Adam Piecek
	xpavli78: Jan Pavlica
*/
/* ***************************************************************** */
#ifndef IAL_H_INCLUDED
#define IAL_H_INCLUDED

void QuickSort(char arr[], int left, int right);
int Bma(const char T[], int Tlen, const char P[], int Plen);


/* nasledujci definice jsou potreba pro hashovaci tabulky */

#include "listparam.h"
#include "strvars.h"
#include "instructions.h"

// prvociselna velikost pole kazde hashovaci tabulky
#define HTABSIZE 101

// konstanty pro slozku isfun struktury htab_data
#define ISVAR 0
#define ISFUN 1
#define ISBINFUN 2

// konstanty pro slozku defined struktury htab_data
#define ISNEW 0
#define ISDECLARED 1
#define ISDEFINED 2

typedef struct
{ // potrebne informace o identifikatorech
	int isfun; // 0 = promenna, 1 = funkce, 2 = vestavena funkce
	int type; // typ promenne, navratovy typ pro funkce
	int defined; // 0 = novy zaznam o identifikatoru, 1 = deklarovana funkce, 2 = definovana promenna/funkce
				// pri vytvoreni polozky inicializovano na 0
	size_t index; // pro promenne index do lokalniho ramce, pro funkce velikost lokalniho ramce promennych (parametry + lokalni promenne + docasne promenne)
	int *strvars;// pole indexu stringovych promennych (kvuli uvolnovani pri navratu z funkce). Pole je ukonceno hodnotou -1
	TlistParam listParam; // seznam parametru funkce
	Tinstruction *begin; // ukazatel na prvni instrukci funkce
}htab_data;

struct htab_listitem
{
	char* key;
	struct htab_listitem* next;
	htab_data data;
};

typedef	struct htab_listitem* htab_t[HTABSIZE];

htab_t* htab_init();
/* alokuje prazdnou hashovaci tabulku.
 * vraci ukazatel na hashovaci tabulku, nebo NULL pokdu selhala alokace
 */

htab_data* htab_search(htab_t* t, const char* key);
/* vyhleda polozku podle klice key v hashovaci tabulce. vrati ukazatel na data polozky pokud existuje, NULL pokud neexistuje
 */

htab_data* htab_insert(htab_t* t, const char* key);
/* vlozi polozku s klicem key do hashovaci tabulky. vrati ukazatel na data vlozenoe polozky nebo NULL pokud selhala alokace.
 * pokud polozka se zadanym klicem jiz v tabulce existovala, vrati ukazatel jeji data (stejne jako by byla vytvorena).
 * slozky defined, strvars, listParam jsou inicializovany, ostatni hodnoty jsou nedefinovane (pokud byla polozka vytvorena), nebo maji predchozi hodnotu (polozka jiz existovala).
 */

void htab_free(htab_t* t);
/* dealokuje vsechny polozky a samotnou hashovaci tabulku
 */

#endif // IAL_H_INCLUDED
