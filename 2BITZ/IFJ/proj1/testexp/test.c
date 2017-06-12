#include <stdio.h>
#include "../main-files/expression.h"
#include "../main-files/symboltable.h"
#include "../main-files/lexer.h"

struct byteProgram byteProgram = {NULL, NULL};

htab_data * symboltable_symbolSearchLocal(symboltable_t *s, const char * identifier) {
	static htab_data arr[] = {
		{.index = 1, .type = DATATYPE_INT},
		{.index = 2, .type = DATATYPE_DOUBLE},
		{.index = 0, .type = DATATYPE_STRING}
	};
	
	return &arr[identifier[0] - 'a'];
}

int main() {
	Tinstruction *ptr = generateInstr(BC_EOP, (Toperand1){.integer=0}, (Toperand){.integer=0}) + 1; // ukazatel na prave provadenou instrukci
	int type[] = {NO_DATATYPE, NO_DATATYPE, NO_DATATYPE, NO_DATATYPE, NO_DATATYPE, NO_DATATYPE, NO_DATATYPE, NO_DATATYPE, NO_DATATYPE, NO_DATATYPE,
	NO_DATATYPE, NO_DATATYPE, NO_DATATYPE, 
	DATATYPE_DOUBLE, DATATYPE_INT,
	NO_DATATYPE, NO_DATATYPE, NO_DATATYPE,
	NO_DATATYPE, NO_DATATYPE, NO_DATATYPE, NO_DATATYPE, NO_DATATYPE, NO_DATATYPE, NO_DATATYPE,
	NO_DATATYPE, NO_DATATYPE, NO_DATATYPE, NO_DATATYPE, NO_DATATYPE, NO_DATATYPE, 
	NO_DATATYPE, NO_DATATYPE, NO_DATATYPE, 
	NO_DATATYPE, NO_DATATYPE, NO_DATATYPE, NO_DATATYPE, NO_DATATYPE, NO_DATATYPE,
	NO_DATATYPE, NO_DATATYPE, NO_DATATYPE, NO_DATATYPE, 
	NO_DATATYPE};
	int vysl, lex;
	
	printf("Velikost size_t: %zu, ptr: %zu, int: %zu, double: %zu \n",
		sizeof(size_t)/DOUBLEWORD_S, sizeof(void *)/DOUBLEWORD_S, sizeof(int)/DOUBLEWORD_S, sizeof(double)/DOUBLEWORD_S);
	
	sourceCodeFile = fopen("add.ifj15", "r");
	for(int i = 0; (unsigned) i < sizeof(type); i++) {
		printf("_________Test %d_________\n", i+1);
		lex = LEX_EOF;
		vysl = expressionAnalyse(10, &type[i], 5, &lex);
		printf("vysledek: %d typ: %d ukon. lex.: %d\n", vysl, type[i], lex);
		
		for (int i = 0; i < 20 && ptr != byteProgram.nextInstr; i++, ptr++) {
			printf("instr: %d, op1,2: %zu %zu %g\n", (unsigned char) ptr->code, ptr->op1.index_size, ptr->op2.index_size, ptr->op2.real); // vzdy se vypise integer hodnota
		}
	}
	puts("------------------------");
	
	return 0;
}