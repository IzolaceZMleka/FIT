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
// Semanticke zpracovani logickeho souctu a soucinu a jeji optimalizace

	if(op1->term == VAL && op2->term != VAL) // imm o xxx -> xxx o imm
		swap(op1, op2);
	
	if(op1->type == DATATYPE_STRING || op2->type == DATATYPE_STRING) // logicke operace nemuzeme provadet nad stringy
		errored(E_SEM_COMPAT_TYP);
		
	resultType = DATATYPE_BOOL;
	
	// OPTIMALIZACNI CAST VYRAZU
	if(op2->term == VAL) { // xxx o imm
		if(op1->term == VAL) { // imm o imm -> imm
			nonTerm->term = VAL;
			nonTerm->operand.integer = computeimms(_OP_);
		} else { // VAR/RES o imm
			if(usenumeric(op2) == _IDENTITY_) { // VAR/RES o NEUTRALNI_PRVEK -> VAR/RES
				if(op2->type == DATATYPE_BOOL)
					*nonTerm = *op1;
				// jinak nacteme prvni operand a zkonvertujeme na bool
				else {
					newStackIndex = op1->term == RES ? op1->operand.index_size : newStackIndex;
					generatecinstr(BC_LOAD, 0,
						(Toperand1){.index_size = newStackIndex}, resultType,
						op1->operand, op1->type
					);
					*nonTerm = (stackItemType){.type = resultType, .operand.index_size = newStackIndex, .term = RES}; // nyni uz je to mezivysledek!
					newStackIndex += datatypesize(resultType);
				}
			} 
			else if(usenumeric(op2) == _AGGRESSIVE_) { // VAR/RES o 0 -> 0
				nonTerm->term = VAL;
				nonTerm->operand.integer = _AGGRESSIVE_;
			}
			// a jina varianta neni!
		}
	}
	
	// GENEROVANI BAJTKODU
	if(!nonTerm->term) { // VAR/RES o VAR/RES
	
		if(op2->term == RES) { // xxx o RES
			if(op1->term != RES) // VAR o RES -> RES o VAR
				swap(op1, op2); // prohodime, aby sme nezapisovali na spatne misto
		}
		
		if(op1->term == RES) newStackIndex = op1->operand.index_size;
		if(op1->term != RES) { // VAR o VAR
				generatecinstr(BC_LOAD, 0,
					(Toperand1){.index_size = newStackIndex}, resultType,
					op1->operand, op1->type
				);
		}
		// nyni je to uz jen RES o RES/VAR
		generatecinstr(_INSTR_, swapped, // komutativni instrukce priznaky neberou vpotaz, nekomutativni se vytvori spravnym poradim operandu
			(Toperand1){.index_size = newStackIndex}, resultType,
			op2->operand, op2->type
		);
		nonTerm->operand.index_size = newStackIndex;
		newStackIndex += datatypesize(DATATYPE_INT); // misto pro pomocnou promennou
	}