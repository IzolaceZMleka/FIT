/* ***************************************************************** */
/*
	projekt: Implementace interpretu imperativniho jazyka IFJ15
	autori: Jan Velecky, Tomas Zahradnicek
	xvelec07: Jan Velecky
	xzahra24: Tomas Zahradnicek
	xpiece00: Adam Piecek
	xpavli78: Jan Pavlica
*/
/* ***************************************************************** */
#include "parser.h"
#include "lexer.h"
#include "symboltable.h"
#include "instructions.h"
#include "errors.h"
#include "interpret.h"
#include <stdio.h>

extern FILE *sourceCodeFile;
struct byteProgram byteProgram;
symboltable_t symboltable;

int main(int argc, char **argv) {
	htab_data *mainData;
	
	tprint("Velikost size_t: %zu, ptr: %zu, int: %zu, double: %zu, Tinstr: %zu \n",
		sizeof(size_t)/DOUBLEWORD_S, sizeof(void *)/DOUBLEWORD_S, sizeof(int)/DOUBLEWORD_S, sizeof(double)/DOUBLEWORD_S, sizeof(Tinstruction)/DOUBLEWORD_S);

	if (argc != 2) {
		fprintf(stderr,"jako argument programu zadej nazev souboru se zdrojovym kodem v ifj15.\n");
		errored(E_INTER);
	}
	sourceCodeFile = fopen(argv[1],"r");
	if (sourceCodeFile == NULL)
		errored(E_INTER);
	
	lexerInit();
	parse();

	mainData = symboltable_symbolSearchGlobal(&symboltable, "main");
	if (mainData == NULL || mainData->defined == ISDECLARED) { // nemelo by nikdy nastat, toto resi parser
		fprintf(stderr, "chybi funkce main");
		return E_SEM_IN_PROG;
	}
	
	DOUBLEWORD fakeRet; // promenna pro navratovou hodnotu mainu, ktera vsak neni pouzita
	interpret(mainData, mainData->index, &fakeRet);

	// nakonec uklidime
	byteProgramDispose();
	symboltable_free(&symboltable);
	fclose(sourceCodeFile);
	lexerClean();
	
	return 0;
}
