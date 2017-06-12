#include "../main-files/instructions.h"
#include <stdio.h>

struct byteProgram byteProgram = {NULL, NULL, NULL};

int main() {
	Toperand1 op1;
	Toperand op2;
	Tinstruction *ptr; // ukazatel na prave provadenou instrukci
	
	printf("pro testovani spravnosti skoku mezi bloky zmensi BLOCKSIZE v instructions.c\n");
	
	// vytvoreni programu
	for (int i = 0; i < 20; i++) {
		op1.integer = i;
		if (i==0) {
			ptr = generateInstr(BC_READ, op1, op2);
			if (ptr == NULL)
				return 1;
		}
		else
			if (generateInstr(BC_READ, op1, op2) == NULL)
				return 1;
	}
	generateInstr(BC_EOP, op1, op2);
	
	// vypis programu
	while (ptr->code != BC_EOP) {
		printf("instr: %hhd, op1: %d\n", ptr->code, ptr->op1.integer); // vzdy se vypise integer hodnota, ackoliv se muze jednat o hodnotu jineho typu (u BC_JMP pointer)
		if (ptr->code == BC_JMP)
			ptr = ptr->op1.ptr; // dalsi instrukce bude 1. operand skoku
		else {
			ptr++; // dalsi instrukce bude nasledujci v poli
		}
	}
	printf("instr: %hhd\n", ptr->code);
	return 0;
}