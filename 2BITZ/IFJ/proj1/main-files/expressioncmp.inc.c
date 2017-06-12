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
// Semanticke zpracovani porovnani a jeji optimalizace

	if(op1->term == VAL && op2->term != VAL) // imm o xxx -> xxx o imm
		swap(op1, op2);
	
	resultType = DATATYPE_BOOL;
	strCmp = 0;
	
	if(op1->type == DATATYPE_STRING || op2->type == DATATYPE_STRING) { // nemuze se menit typ stringu
		if(op1->type != op2->type)
			errored(E_SEM_COMPAT_TYP);
		strCmp = 1;
		promotedType = DATATYPE_STRING;
	} else
		promotedType = op1->type == DATATYPE_DOUBLE ? op1->type : op2->type;
	
	// OPTIMALIZACNI CAST VYRAZU
	if(op2->term == VAL) { // xxx o imm
		if(op1->term == VAL) { // imm o imm -> imm
			nonTerm->term = VAL;
			if(!strCmp) { // porovnani cisel
				nonTerm->operand.integer = computeimms(_OP_);
			} else // porovnani retezcu
				nonTerm->operand.integer = strcmp(((Tstring *)(op1->operand.ptr))->str, ((Tstring *)(op2->operand.ptr))->str) _OP_ 0;
		} else { // VAR/RES o imm
			if(op2->type != promotedType) // int -> double
				op2->type = DATATYPE_DOUBLE,
				op2->operand.real = op2->operand.integer;
		}
	}
	
	// GENEROVANI BAJTKODU
	if(!nonTerm->term) { // VAR/RES o xxx
	
		if(op2->term == RES) { // xxx o RES
			if(op1->term != RES) // VAR o RES -> RES o VAR
				swap(op1, op2); // prohodime, aby sme nezapisovali na spatne misto
		}
		
		if(op1->term == RES) newStackIndex = op1->operand.index_size;
		if(op1->term != RES) { // VAR o VAR/imm
				generatecinstr(BC_LOAD, 0,
					(Toperand1){.index_size = newStackIndex}, promotedType,
					op1->operand, op1->type
				);
				if(maxStackSize < newStackIndex + datatypesize(promotedType)) maxStackSize += datatypesize(promotedType);
				// muze dojit k pretypovani na double, vysledek ale stejne bude jako int
		}
		// nyni je to uz jen RES o RES/VAR/imm
		generatecinstr(_INSTR_, swapped, // komutativni instrukce priznaky neberou vpotaz, nekomutativni se vytvori spravnym poradim operandu
			(Toperand1){.index_size = newStackIndex}, promotedType,
			op2->operand, op2->term == VAL ? NO_DATATYPE : op2->type
		);
		nonTerm->operand.index_size = newStackIndex;
		newStackIndex += datatypesize(DATATYPE_INT); // misto pro pomocnou promennou
	}