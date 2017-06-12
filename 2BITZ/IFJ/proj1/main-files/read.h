/* ***************************************************************** */
/*
	projekt: Implementace interpretu imperativniho jazyka IFJ15
	autori: Adam Piecek
	xvelec07: Jan Velecky
	xzahra24: Tomas Zahradnicek
	xpiece00: Adam Piecek
	xpavli78: Jan Pavlica
*/
/* ***************************************************************** */
#define RUNTIME_ENV
#include "builtin_func.h"

			
typedef enum readState {
    S_DIGIT,                // nacitani cislice
	S_DOT,					// nacteno '.'
    S_EXP,                  // nacteno 'e' nebo 'E'
	S_SIGN,                 // nacteno '+' nebo '-'
	S_DIG_OR_ER				// znak musi byt cislo, nebo nastane chyba
} readState;

int readInteger();
double readDouble();
Tstring * readString();
