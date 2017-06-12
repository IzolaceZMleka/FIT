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
#ifndef STRVARS_H_INCLUDED
#define STRVARS_H_INCLUDED

/* funkce prida na konec pole *strvars hodnotu index. Konec pole je vzdy oznacen hodnotou -1.
 * muze menit ukazatel strvars pri alokaci noveho nebo delsiho pole.
 * vraci 0 pri chybe, jinak 1.
 */
int strvars_add(int **strvars, int index); 

#endif // STRVARS_H_INCLUDED