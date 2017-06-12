#include "../main-files/parser.h"
#include "../main-files/symboltable.h"
#include "../main-files/instructions.h"
#include "../main-files/errors.h"
#include "../main-files/lexer.h"
#include "../main-files/instructions.h"
#include <stdio.h>

extern FILE *sourceCodeFile;
struct byteProgram byteProgram;
symboltable_t symboltable;

int main(int argc, char **argv) {
	Tinstruction *instrptr;
	htab_data *mainData;

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
	if (mainData == NULL || mainData->defined == ISDECLARED) {
		fprintf(stderr, "chybi funkce main");
		return E_SEM_IN_PROG;
	}
	instrptr = mainData->begin;
	printf("adresa : instrukce operandy\n");
	while (instrptr->code != BC_EOP) {
		printf("%d:   %hhu\t   %d, %d\n",instrptr, instrptr->code, instrptr->op1.integer, instrptr->op2.integer);
		instrptr++;
	}

	fclose(sourceCodeFile);
	lexerClean();
	return 0;
}