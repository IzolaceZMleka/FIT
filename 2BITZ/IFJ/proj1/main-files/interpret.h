/* ***************************************************************** */
/*
	projekt: Implementace interpretu imperativniho jazyka IFJ15
	autori: Jan Velecky, Tomas Zahradnicek 
	xvelec07: Jan Velecky
	xzahra24: Tomas Zahradnicek
	xpiece00: Adam Piecek
	xpavli78: Jan Pavlic
*/
/* ***************************************************************** */
#include <stdlib.h>
#include "instructions.h"
#include "ial.h"

/* Interpret vykonavani funkce.
 * func: Odkaz do tabulky symbolu na funkci, kterou ma interpret vykonavat.
 * frameSize: Velikost ramce v DOUBLEWORD_S pro veskere lokalni promenne a vypocty.
 * callerFrame: Odkaz do ramce volajici funkce pro pristup k parametrum a vraceni navratove hodnoty funkce.
*/
void interpret(htab_data * func, size_t frameSize, DOUBLEWORD * callerFrame);