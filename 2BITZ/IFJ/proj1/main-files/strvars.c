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
#include "strvars.h"
#include <stdlib.h>
#define MIN 10 // pocet prvku ktere se daji ulozit do nejmensiho alokovaneho pole

static int *last = NULL; // ukazatel posledniho upravovaneho pole. Pokud se upravuje stale stejne pole, pridavaj se indexy na konec. U noveho ukazatele se musi alokovat pole
static size_t count = 0; // index volneho mista v poli
static size_t max = 0; // delka alokovaneho pole

int strvars_add(int **strvars, int index) {
	if (*strvars != last || *strvars == NULL) { // zacinaji se zaznamenavat stringove promenne v nove funkci
		max = MIN;
		*strvars = malloc(sizeof(int) * (max+1));
		if (*strvars == NULL) // chyba alokace
			return 0;
		last = *strvars;
		count = 0;
	}
	if (count == max) { // neni misto pro dalsi index stringove promenne
		max = max * 2; // zdvojnasobi se pocet prvku ktere se daji ulozit do pole
		*strvars = realloc(*strvars, sizeof(int) * (max+1));
		if (*strvars == NULL) { // chyba alokace
			*strvars = last; // vraceni na predchozi hodnotu
			return 0; // 
		}
		last = *strvars;
	}
	(*strvars)[count] = index;
	count++;
	(*strvars)[count] = -1; // znaci konec pole
	return 1;
}