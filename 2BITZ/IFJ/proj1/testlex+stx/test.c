#include "../main-files/parser.h"
#include "../main-files/symboltable.h"
#include "../main-files/instructions.h"
#include <stdio.h>

extern FILE *sourceCodeFile;
struct byteProgram byteProgram;
symboltable_t symboltable;

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("jako argument programu zadej nazev souboru se zdrojovym kodem v ifj15.\n");
		return 1;
	}
	sourceCodeFile = fopen(argv[1],"r");
	printf("parser returns %d\n", parse());
	return 0;
	fclose(sourceCodeFile);
}