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
#include <string.h>

// semanticka analyza operace
void semantic(TSitem * s) { // E -> E o E
	stackItemType * op2 = &s->data, // pravy operand
	              * op1 = &s->next->next->data, // levy operand
				  * nonTerm = &s->next->data, // vysledny non-terminal (a operace)
				  * auxOp; // pro prohozeni ukazatelu (vyuziva makro auxOp)
	int swapped = 0;
	size_t newStackIndex = curStackIndex;
	#define swap(op1, op2) (auxOp = op1, op1 = op2, op2 = auxOp, swapped = !swapped)
	#define computeimms(_op_)                     \
		usenumeric(op1) _op_ usenumeric(op2)
	
	char op = nonTerm->term,
		resultType = 0; // inicializace na nulu pouze, aby kompiler nesilel, ze (ne)muze byt resultType pouzita neinicializovana
	char promotedType;
	int strCmp;
	nonTerm->term = 0; // z trech polozek na zasobniku zustane ta prostredni
	switch(op) { // typ operace
		case LEX_BO_PLUS:
			#define _OP_ +
			#define _INSTR_ BC_ADD
			#define _IDENTITY_ 0
			#define _COMMUTATIVE_
			#include "expressionop.inc.c"
			#undef _IDENTITY_
			#undef _INSTR_
			#undef _OP_
			break;
		case LEX_BO_MULTIPLY:
			#define _OP_ *
			#define _INSTR_ BC_MUL
			#define _IDENTITY_ 1
			#define _AGGRESSIVE_ 0
			#include "expressionop.inc.c"
			#undef _AGGRESSIVE_
			#undef _COMMUTATIVE_
			#undef _IDENTITY_
			#undef _INSTR_
			#undef _OP_
			break;
		case LEX_BO_MINUS:
			#define _OP_ -
			#define _INSTR_ BC_SUB
			#define _IDENTITY_ 0
			#include "expressionop.inc.c"
			#undef _IDENTITY_
			#undef _INSTR_
			#undef _OP_
			break;
		case LEX_BO_DIVIDE:
			#define _OP_ /
			#define _INSTR_ BC_DIV
			#define _IDENTITY_ 1
			#define _UNDEFINED_ 0
			#include "expressionop.inc.c"
			#undef _UNDEFINED_
			#undef _IDENTITY_
			#undef _INSTR_
			#undef _OP_
			break;
		#define _COMPARING_ // operatory porovnavani - vysledek typu integer
		case LEX_BO_EQUAL:
			#define _OP_ ==
			#define _INSTR_ BC_CMPEQ
			#define _COMMUTATIVE_
			#include "expressioncmp.inc.c"
			#undef _COMMUTATIVE_
			#undef _INSTR_
			#undef _OP_
			break;
		case LEX_BO_NOT_EQUAL:
			#define _OP_ !=
			#define _INSTR_ BC_CMPNE
			#define _COMMUTATIVE_
			#include "expressioncmp.inc.c"
			#undef _COMMUTATIVE_
			#undef _INSTR_
			#undef _OP_
			break;
		case LEX_BO_LESS_EQUAL:
			#define _OP_ <=
			#define _INSTR_ BC_CMPLE
			#include "expressioncmp.inc.c"
			#undef _INSTR_
			#undef _OP_
			break;
		case LEX_BO_GREATER:
			#define _OP_ >
			#define _INSTR_ BC_CMPG
			#include "expressioncmp.inc.c"
			#undef _INSTR_
			#undef _OP_
			break;
		case LEX_BO_GREATER_EQUAL:
			#define _OP_ >=
			#define _INSTR_ BC_CMPGE
			#include "expressioncmp.inc.c"
			#undef _INSTR_
			#undef _OP_
			break;
		case LEX_BO_LESS:
			#define _OP_ <
			#define _INSTR_ BC_CMPL
			#include "expressioncmp.inc.c"
			#undef _INSTR_
			#undef _OP_
			break;
		#undef _COMPARING_
		case LEX_BO_AND:
			#define _OP_ &&
			#define _INSTR_ BC_AND
			#define _IDENTITY_ 1
			#define _AGGRESSIVE_ 0
			#include "expressionlog.inc.c"
			#undef _AGGRESSIVE_
			#undef _IDENTITY_
			#undef _INSTR_
			#undef _OP_
			break;
		case LEX_BO_OR:
			#define _OP_ ||
			#define _INSTR_ BC_OR
			#define _IDENTITY_ 0
			#define _AGGRESSIVE_ 1
			#include "expressionlog.inc.c"
			#undef _AGGRESSIVE_
			#undef _IDENTITY_
			#undef _INSTR_
			#undef _OP_
			break;
		default:
			errored(E_SYN);
	}
	// pokud neni vysledek nonterminalu stale stanoven, jedna se o mezivysledek
	if(!nonTerm->term) nonTerm->term = RES;
	nonTerm->type = resultType; // ulozeni vysledneho typu
	
	curStackIndex = newStackIndex;
	if(curStackIndex > maxStackSize) maxStackSize = newStackIndex;
}

void semanticUnary(TSitem * s) { // E -> !E
	stackItemType * op = &s->data, // operand (vysledny non-terminal)
	              * nonTerm = op; // alias
	size_t newStackIndex = curStackIndex;
	
	char oper = s->next->data.term,
		resultType = DATATYPE_BOOL;
	switch(oper) { // typ operace
		case LEX_UO_NEG:
			if(op->term == VAL) { // !imm
				if(op->type == DATATYPE_STRING) // nelze negovat string
					errored(E_SEM_COMPAT_TYP);
				else if(op->type == DATATYPE_DOUBLE)
					op->operand.integer = !op->operand.real;
				else
					op->operand.integer = !op->operand.integer;
			} else { // !RES/VAR
				if(op->term == RES) newStackIndex = op->operand.index_size;
				generatecinstr(BC_NEG, 0,
					(Toperand1){.index_size = newStackIndex}, resultType,
					op->operand, op->type
				);
				nonTerm->operand.index_size = newStackIndex;
				newStackIndex += datatypesize(resultType);
				// muze dojit k pretypovani na double, vysledek ale stejne bude jako int
			}
			break;
		default:
			errored(E_SYN);
	}
	// je to bud konstanta, nebo mezivysledek
	if(nonTerm->term != VAL) nonTerm->term = RES;
	nonTerm->type = resultType; // ulozeni vysledneho typu
	
	curStackIndex = newStackIndex;
	if(curStackIndex > maxStackSize) maxStackSize = newStackIndex;
}