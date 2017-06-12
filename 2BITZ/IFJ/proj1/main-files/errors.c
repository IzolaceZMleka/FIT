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
#include "errors.h"
#include "instructions.h"
#include "lexer.h"
#include "symboltable.h"
#include "expression.h"

extern symboltable_t symboltable;
extern FILE *sourceCodeFile;

int (errored)(int errorCode){

	switch(errorCode){
		case E_LEX:
			fprintf(stderr, "CHYBA(%d E_LEX): chybna struktura aktualniho lexemu.\n", E_LEX);
			break;
		case E_SYN:
			fprintf(stderr, "CHYBA(%d E_SYN): chybna syntaxe struktury programu.\n", E_SYN); 
			break;
		case E_SEM_IN_PROG:
			fprintf(stderr, "CHYBA(%d E_SEM_IN_PROG): chybna semantika v programu.\n", E_SEM_IN_PROG); 
			break;
		case E_SEM_COMPAT_TYP:
			fprintf(stderr, "CHYBA(%d E_SEM_COMPAT_TYP): chybna semantika typove kompatibility, ci spatny pocet parametru u volani funkce.\n", E_SEM_COMPAT_TYP);
			break;
		case E_SEM_DERIV_TYP:
			fprintf(stderr, "CHYBA(%d E_SEM_DERIV_TYP): chyba pri odvozovani datoveho typu promenne.\n", E_SEM_DERIV_TYP); 
			break;
		case E_SEM_OTH:
			fprintf(stderr, "CHYBA(%d E_SEM_OTH): nespecifikovana chyba semanticka.\n", E_SEM_OTH);
			break;
		case E_RUN_LOAD_NUM:
			fprintf(stderr, "CHYBA(%d E_RUN_LOAD_NUM): chyba za behu pri nacitani ciselne hodnoty ze vstupu.\n", E_RUN_LOAD_NUM);
			break;
		case E_RUN_UNINIT_VAR:
			fprintf(stderr, "CHYBA(%d E_RUN_UNINIT_VAR): chyba za behu zpusobena neinicializovanou promennou.\n", E_RUN_UNINIT_VAR); 
			break;
		case E_RUN_ZERO_DIV:
			fprintf(stderr, "CHYBA(%d E_RUN_ZERO_DIV): chyba za behu zpusobena deleni nulou.\n", E_RUN_ZERO_DIV); 
			break;
		case E_RUN_OTH:
			fprintf(stderr, "CHYBA(%d E_RUN_OTH): nespecifikovana chyba za behu.\n", E_RUN_OTH); 
			break;
		case E_INTER:
			fprintf(stderr, "CHYBA(%d E_INTER): chyba interni, neovlivnena vstupnim programem.\n", E_INTER); 
			break;
	}

	byteProgramDispose();
	symboltable_free(&symboltable);
	if(sourceCodeFile != NULL)
		fclose(sourceCodeFile);
	lexerClean();
	expessionClean();
	exit(errorCode);
}


