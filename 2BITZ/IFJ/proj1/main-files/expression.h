/* ***************************************************************** */
/*
	projekt: Implementace interpretu imperativniho jazyka IFJ15
	autori: Jan Velecky
	xvelec07: Jan Velecky
	xzahra24: Tomas Zahradnicek
	xpiece00: Adam Piecek
	xpavli78: Jan Pavlica
*/
/* ***************************************************************** */
// hodnoty pro vyrazy
/*#define ALWAYS_TRUE -2
#define ALWAYS_FALSE -3*/

/* Provede syntaktickou a semantickou analyzu vyrazu a vygeneruje potrebne instrukce.
 * expressionAnalyse: Vraci velikost mista potrebneho pro vypocet operace.
 * memOffset: Urcuje aktualni index v ramci, kde mohou probihat pomocne vypocty.
 * resultType: Specifikuje vysledny datovy typ vyrazu. Pokud nelze takoveho typu
 *             dosahnout, nastava chyba. Konstanta NO_DATATYPE znaci, ze vysledny typ
 *             neni znam. Analyza v tomto pripade vrati pomoci teto promenne vysledny typ vyrazu.
 * resultIndex: Urcuje aktualni index v ramci, kam umistit vysledek vyrazu.
 *              Bere se v potaz, pokud je specifikovan vysledny typ.
 * lex: Prvni lexem samotneho vyrazu, ktery uz byl nacten syntaktickym analyzatorem.
 *      LEX_EOF predesila, ze pocatek vyrazu jeste nebyl nacten.
 *      Analyza zaroven do teto promenne vraci
 * saveBC: bajtkod instrukce, ktera ma finalni vysledek ulozit na resultIndex
 */
size_t expressionAnalyse(size_t memOffset, int * resultType, size_t resultIndex, int * lex, char saveBC);

/* Dealokuje dynamickou pamet pouzivanou analyzou vyrazou
 */
void expessionClean(void);
