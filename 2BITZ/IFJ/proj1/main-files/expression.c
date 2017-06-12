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
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h> // pouze vypocet v kompilacnim case
#include "lexer.h"
#include "symboltable.h"
#include "expression.h"
#include "errors.h"

// POLOZKA ZASOBNIKU PRECEDENCNIHO ANALYZATORU
typedef struct {
	int flags;
	char term; // typ terminalu (nezaporne cislo), typ neterminalu (zaporne cislo)
	char type; // datovy typ (operandu a nonterminalu)
	Toperand operand;
} stackItemType;

enum Flags {
	LOCK = 1, // <a kde je (ne)terminal na zasobniku
	IMM = 2 // konstanta (operand bez toho priznaku je promenna)
};
// typy neterminalu
#define RES -1 // mezivysledek (ma index)
#define VAL -2 // prima hodnota (ma hodnotu - muze to byt mezivysledek operace mezi dvema nonterminaly tohoto typu)
#define VAR -3 // promenna (ma index)

// POMOCNA STRUKTURA ZASOBNIK

typedef struct stackItem {
	struct stackItem * next;
	stackItemType data;
} TSitem;

TSitem * Spush(TSitem * s) {
	TSitem * aux = malloc(sizeof(TSitem));
	if(aux) aux->next = s;
	return aux;
}

void Spop(TSitem ** s) {
	TSitem * aux = *s;
	if(*s) {
		*s = (*s)->next;
		free(aux);
	}
}
	
void Sdispose(TSitem * s) {
	TSitem * aux;
	while(s) {
		aux = s->next;
		free(s);
		s = aux;
	}
}

static TSitem * s = NULL; // globalni promenna pro zasobnik, aby bylo mozne uvolnovat 
void expessionClean(void) {
	Sdispose(s);
	s = NULL;
}


// semanticka analyza operace
size_t curStackIndex, maxStackSize = 0; // aktualni nejvyssi nevyuzivany index pro pomocne vypocty (pomocna promenna)
#define usenumeric(var) ((var)->type == DATATYPE_DOUBLE ? (var)->operand.real : (var)->operand.integer)
// makro pro generovani instrukci s kontrolou uspesnosti vytvoreni instrukce
#define generatecinstr(instr, flags, op1, type1, op2, type2) do {                                      \
		if(generateCInstr((instr), (flags), (op1), (type1), (op2), (type2)) == NULL) errored(E_INTER); \
	} while(0)
#include "expressionsemantic.inc.c"

#define OPERAND (15 + LEX_UO_NEG)
#define TERMINATING_SYMBOL (16 + LEX_UO_NEG)
const char _precedenceTable[] = 
	"<>>>>>>>>>>>><><>\
	 <>><<>>>>>>>><><>\
	 <>><<>>>>>>>><><>\
	 <>>>>>>>>>>>><><>\
	 <>>>>>>>>>>>><><>\
	 <<<<<>>>>>>>><><>\
	 <<<<<>>>>>>>><><>\
	 <<<<<>>>>>>>><><>\
	 <<<<<>>>>>>>><><>\
	 <<<<<>>>>>>>><><>\
	 <<<<<>>>>>>>><><>\
	 <<<<<<<<<<<>><><>\
	 <<<<<<<<<<<<><><>\
	 <<<<<<<<<<<<<<=<E\
	 E>>>>>>>>>>>>E>E>\
	 E>>>>>>>>>>>>E>E>\
	 <<<<<<<<<<<<<<E<E\
	";
/* Pravidla:
 * E -> i
 * E -> !E
 * E -> (E)
 * E -> E * E
 * E -> E / E
 * E -> E + E
 * E -> E - E
 * E -> E < E
 * E -> E > E
 * E -> E >= E
 * E -> E <= E
 * E -> E != E
 * E -> E == E
 */

// funkce najde posledni terminal na zasobniku (tj. preskoci non-terminaly)
stackItemType * findLastTerminal(TSitem * s) {
	while(s != NULL && s->data.term < 0)
		s = s->next;
	
	return s ? &s->data : NULL;
}

extern symboltable_t symboltable;

// SAMOTNY ANALYZATOR VYRAZU
size_t expressionAnalyse(size_t memOffset, int * resultType, size_t resultIndex, int * lexem, char saveBC) {
	const char (* precedenceTable)[19] = (void *) _precedenceTable;
	curStackIndex = maxStackSize = memOffset;
	/*for(int i = 0; i < 16 * 14; i++)
		printf("i=%d: %c %c \n", i, (*precedenceTable)[i], (*precedenceTable)[i]);
	printf("%c %c\n", precedenceTable[1][13], precedenceTable[2][13]);*/

	if((s = Spush(s)))
		s->data.term = TERMINATING_SYMBOL;
	else
		errored(E_INTER);
	
	stackItemType item, * lastTerm;
	int nesting = 0;
	int lex = *lexem == LEX_EOF ? lexerGetLexem() : *lexem; // pokud prvni znak byl jiz nacten nadrazenym analyzatorem, pouzit jej jako pocatek vyrazu
	do { // jedna se o vyraz, ne o volani funkce
		item.flags = 0; // resetovani polozky item (ostatni polozky jsou prirazovany)
	
		if(lex >= LEX_UO_NEG && lex <= LEX_R_PARENTHESIS) { // lexemy, ktere se muzou vyskytovat ve vyrazu
			item.term = lex;
		
			// prava zavorka se muze vyskytnou i ve vyrazu i jako ukoncujici symbol
			if(lex == LEX_L_PARENTHESIS) nesting++;
			else if(lex == LEX_R_PARENTHESIS) {
				if(nesting == 0) item.term = TERMINATING_SYMBOL;
				else nesting--;
			}
		} else if(lex == LEX_ID) {		
			// overeni existence promenne
			htab_data * var = symboltable_symbolSearchAll(&symboltable, identifier);
			
			if(var == NULL /*|| !var->intialized nemozno zjistit pri analyze! */) {
				errored(E_SEM_IN_PROG); // nedeklarovana promenna!
			} else if(var->type == NO_DATATYPE) {
				errored(E_SEM_DERIV_TYP); // Promenna nema datovy typ. To tedy nevim, co s ni!
			} else if(var->isfun) {
				errored(E_SEM_IN_PROG); // Neni to promenna, nybrz funkce!
			} else {
				item.type = var->type;
				item.operand.index_size = var->index;
				item.term = OPERAND;
			}
		} else if(lex >= LEX_BOOL_F && lex <= LEX_LIT_STRING) {
			item.flags |= IMM;
			item.type = lex - LEX_LIT_INT;
			if(lex == LEX_BOOL_F || lex == LEX_BOOL_T) {
				item.operand.integer = lex - LEX_BOOL_F; // tj. 0 nebo 1
				item.type = DATATYPE_BOOL;
			} if(lex == LEX_LIT_INT)       item.operand.integer = integer;
			else if(lex == LEX_LIT_REAL)   item.operand.real = real;
			else if(lex == LEX_LIT_STRING) item.operand.ptr = string;
			item.term = OPERAND;
		} else if(lex == LEX_ERROR_LEX || lex == LEX_ERROR_IN) {
			*lexem = lex; // nastala chyba pri nacitani lexemu
			return 0;
		} else {
			item.term = TERMINATING_SYMBOL; // Neocekavany lexem ve vyrazu
		}
			
		
		repeat: // opakovani bez cteni a zpracovani dalsiho symbolu
		lastTerm = findLastTerminal(s);
		/*{
			printf("%d |", item.term);
			for(TSitem * i = s; i; i = i->next) {
				printf("%c", '>' * (i->data.flags & LOCK));
				if(i->data.term < 0) printf("E");
				else printf("%d", i->data.term);
			}
			puts("");
		};*/
		if(item.term == TERMINATING_SYMBOL && lastTerm->term == TERMINATING_SYMBOL)
			break;
		switch(precedenceTable[(unsigned char) (lastTerm->term - LEX_UO_NEG)][(unsigned char) (item.term - LEX_UO_NEG)]) {
			case '<':
				lastTerm->flags |= LOCK;
			case '=':	
				if((s = Spush(s)))
					s->data = item;
				else
					errored(E_INTER);
				break;
			case '>':
				// preklad pomoci pravidel A -> y
				if(s->next->data.flags & LOCK) {
					if(s->data.term == OPERAND) { // E -> i (operand)
						tprint("E -> i");
						s->data.term = s->data.flags & IMM ? VAL : VAR; // typ operandu (nonterminalu)
						s->next->data.flags ^= LOCK; // odebrani zarazky
						goto repeat;
					} else
						errored(E_SYN);
				} else if(s->next) {
					if(s->next->next->data.flags & LOCK) { // !E
						if(!(s->data.term < 0 && s->next->data.term > 0)) // kontrola zda na vrcholu je nonterminal a za nim terminal
							errored(E_SYN);
						semanticUnary(s); // analyza unarni operace
						Spop(&s->next); // smazani neterminalu
						s->next->data.flags ^= LOCK; // odebrani zarazky
						goto repeat;
					} else if(s->next->next) {
						if(s->next->next->next->data.flags & LOCK) {
							if(s->data.term < 0 && s->next->next->data.term < 0 /* && s->data.term < 11*/) { // E -> E o E
								tprint("E -> E o E");
								semantic(s); // analyza operace
								Spop(&s); // smazani druheho neterminalu
								Spop(&s->next); // smazani prvniho neterminalu
								s->next->data.flags ^= LOCK; // odebrani zarazky
								goto repeat;
							} else if(s->data.term == LEX_R_PARENTHESIS && s->next->next->data.term == LEX_L_PARENTHESIS) { // E -> (E)
								tprint("E -> (E)");
								Spop(&s); // smazani ukoncujici zavorky
								Spop(&s->next); // smazani pocatecni zavorky
								s->next->data.flags ^= LOCK; // odebrani zarazky
								goto repeat;
							}
						}
					}			
				}
			default: 
				errored(E_SYN); // Spatna syntaxe vyrazu
		}
	} while((lex = lexerGetLexem()));
	
	if(&s->data == lastTerm) // na zasobniku chybi jakkykoliv terminal
		errored(E_SYN);
	
	// zpracovani vysledku vyrazu
	if(*resultType == NO_DATATYPE) { // tj. jedna se o vyskyt ve vyrazu, nebo inicializaci s auto detekci typu
		*resultType = s->data.type;
		if(s->data.term != RES) {
			generatecinstr(saveBC, s->data.term == VAL,
				(Toperand1){.index_size = memOffset}, *resultType,
				s->data.operand, *resultType
			);
			// v pripade, ze jsme dosud nepotrebovali pomocne misto, ale potrebujem nyni na vysledek
			if(maxStackSize - memOffset < datatypesize(s->data.type)) maxStackSize = memOffset + datatypesize(s->data.type);
		}
	} else {
		if((*resultType == DATATYPE_STRING || s->data.type == DATATYPE_STRING) && *resultType != s->data.type)
			errored(E_SEM_COMPAT_TYP);
		else {
			 // jeste je treba pripadne prevest konstantni vyraz do odpovidajiciho typu
			if(s->data.term == VAL && s->data.type != *resultType) {
				if(*resultType == DATATYPE_DOUBLE)
					s->data.operand.real = s->data.operand.integer;
				else if(*resultType == DATATYPE_INT && s->data.type != DATATYPE_BOOL) // prirazeni (int) <- (bool) je kompatibilni
					s->data.operand.integer = s->data.operand.real;
				else if(*resultType == DATATYPE_BOOL) { // pokud se uklada jako bool, musi se ukladat int 1 nebo 0
					if(s->data.type == DATATYPE_DOUBLE)
						s->data.operand.integer = !!s->data.operand.real;
					else
						s->data.operand.integer = !!s->data.operand.integer;
				}
				s->data.type = *resultType;
			}
			// a vytvorime instrukci
			generatecinstr(saveBC, s->data.term == VAL,
				(Toperand1){.index_size = resultIndex}, *resultType,
				s->data.operand, s->data.type
			);
		}
	}
	
	*lexem = lex; // vratit lexem, na kterem se skoncilo
	Sdispose(s);
	s = NULL;
	return maxStackSize - memOffset;
}
