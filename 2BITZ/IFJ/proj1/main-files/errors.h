/* ***************************************************************** */
/*
	projekt: Implementace interpretu imperativniho jazyka IFJ15
	autori: Adam Piecek, Jan Velecky
	xvelec07: Jan Velecky
	xzahra24: Tomas Zahradnicek
	xpiece00: Adam Piecek
	xpavli78: Jan Pavlica
*/
/* ***************************************************************** */
#ifndef ERRORS_H_INCLUDED
#define ERRORS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define E_LEX 1
// chyba v programu v ramci lexikalni analyzy (chybna struktura aktualniho lexemu).
#define E_SYN 2
// chyba v programu v ramci syntakticke analyzy (chybna syntaxe struktury programu).
#define E_SEM_IN_PROG 3
// semanticka chyba v programu – nedefinovana funkce/promenna, pokus o redefinici funkce/promenne, atd.
#define E_SEM_COMPAT_TYP 4
/*	semanticka chyba typove kompatibility v aritmetickych, retezcovych a relacnich
 *	vyrazech, prip. spatny pocet ci typ parametru u volani funkce. */
#define E_SEM_DERIV_TYP 5 
//  chyba pri odvozovani datoveho typu proměnné.
#define E_SEM_OTH 6
// ostatni semanticke chyby.
#define E_RUN_LOAD_NUM 7
// behova chyba pri nacitani ciselne hodnoty ze vstupu.
#define E_RUN_UNINIT_VAR 8
// behova chyba pri praci s neinicializovanou promennou.
#define E_RUN_ZERO_DIV 9
// behova chyba deleni nulou.
#define E_RUN_OTH 10
// ostatni behove chyby.
#define E_INTER 99
/* interni chyba interpretu tj. neovlivnena vstupnim programem (napr. chyba a-
 * lokace pameti, chyba pri otvirani souboru s ridicim programem, spatne parametry
 * prikazove radky atd.).
 */

int (errored)(int errorCode);



// makra pro vypisovani chyb a ladicich informaci
#ifdef NDEBUG
	#define debug(...)
	#define tprint(...)
	#define errored(erC) (errored)(erC)
#else
	#define __debug(type, ...) do {                   \
		fprintf(stderr, "__" type ": " __VA_ARGS__);  \
		fprintf(stderr, "\n");                        \
	} while(0)
	#define debug(...) do {\
		fprintf(stderr, "__DEBUG %s:%d: ", __FILE__, __LINE__); \
		fprintf(stderr, __VA_ARGS__);                           \
		fprintf(stderr, "\n");                                  \
	} while(0)
	#define tprint(...) __debug("LOG", __VA_ARGS__)
	#define errored(erC) do {  \
		debug(" ");            \
		(errored)(erC);        \
	} while(0)
#endif

#endif // ERRORS_H_INCLUDED