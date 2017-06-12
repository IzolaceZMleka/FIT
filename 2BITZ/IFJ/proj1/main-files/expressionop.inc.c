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
// Semanticke zpracovani aritmetickych operaci a jeji optimalizace

	#ifdef _COMMUTATIVE_ // optimalizace vyrazu pro konstanty nemuze byt v opacnem poradi, pokud neni operace komutativni
	if(op1->term == VAL) // imm o xxx -> xxx o imm
		swap(op1, op2);
	#endif
	
	if(op1->type == DATATYPE_STRING || op2->type == DATATYPE_STRING) // nemuzeme pocitat se stringy
		errored(E_SEM_COMPAT_TYP);
	
	// prioritni pretypovani na double, jinak int (vysledek je vzdy ciselny)
	resultType = op1->type == DATATYPE_DOUBLE || op2->type == DATATYPE_DOUBLE ? DATATYPE_DOUBLE : DATATYPE_INT;
	
	// OPTIMALIZACNI CAST VYRAZU
	if(op2->term == VAL) { // xxx o imm
		#ifdef _UNDEFINED_ // operace s nedefinovanym prvkem
		if(usenumeric(op2) == _UNDEFINED_) { // VAR/RES o PRVEK_MIMO_DF -> NEDEFINOVANY_VYSLEDEK
			// behova (semanticka) chyba - deleni nulou!
			generatecinstr(BC_ERROR, 0, // nicmene chyba nastane az za behu, pokud se sem kod dostane
				(Toperand1){.integer = E_RUN_ZERO_DIV}, 0,
				(Toperand){0}, 0
			);
		} else
		#endif
		if(op1->term == VAL) { // imm o imm -> imm
			nonTerm->term = VAL;
			if(resultType == DATATYPE_DOUBLE) { // min. jeden je double
				nonTerm->operand.real = computeimms(_OP_);
			} else { // oba typu int
				nonTerm->operand.integer = computeimms(_OP_);
			}
		} else { // VAR/RES o imm
			if(usenumeric(op2) == _IDENTITY_) { // VAR/RES o NEUTRALNI_PRVEK -> VAR
				if(op1->type == op2->type || op2->type != DATATYPE_DOUBLE) *nonTerm = *op1; // pocitani absolutne bez efektu
				else { // pretypovani
					newStackIndex = op1->term == RES ? op1->operand.index_size : newStackIndex;
					generatecinstr(BC_LOAD, 0,
						(Toperand1){.index_size = newStackIndex}, resultType,
						op1->operand, op1->type
					);
					*nonTerm = (stackItemType){.type = resultType, .operand.index_size = newStackIndex, .term = RES}; // nyni uz je to mezivysledek!
					newStackIndex += datatypesize(resultType);
				}
			} else
			#ifdef _AGGRESSIVE_ // agresivni prvek - nasobeni
			if(usenumeric(op2) == _AGGRESSIVE_) { // VAR/RES o 0 -> 0
				nonTerm->term = VAL;
				if(resultType == DATATYPE_DOUBLE) // min. jeden je double
					nonTerm->operand.real = _AGGRESSIVE_;
				else // oba typu int
					nonTerm->operand.integer = _AGGRESSIVE_;
			} else
			#endif
			if(op2->type != resultType) // int -> double
				op2->type = resultType,
				op2->operand.real = op2->operand.integer;
		}
	}
	
	// GENEROVANI BAJTKODU
	if(!nonTerm->term) { // VAR/RES o xxx
		#ifndef _COMMUTATIVE_
		if(op1->term == VAL) // imm o xxx -> xxx o imm
			swap(op1, op2);
		#endif
	
		if(op2->term == RES) { // xxx o RES
			if(op1->term != RES) // VAR o RES -> RES o VAR
				swap(op1, op2); // prohodime, aby sme nezapisovali na spatne misto
		}
		
		if(op1->term == RES) newStackIndex = op1->operand.index_size;
		if(op1->term != RES) { // VAR o VAR/imm
			generatecinstr(BC_LOAD, 0,
				(Toperand1){.index_size = newStackIndex}, resultType,
				op1->operand, op1->type
			);
			op1->type = resultType;
		}
		
		// nyni je to uz jen RES o RES/VAR/imm
		generatecinstr(_INSTR_, swapped, // komutativni instrukce priznaky neberou vpotaz, nekomutativni se vytvori spravnym poradim operandu
			(Toperand1){.index_size = newStackIndex}, op1->type,
			op2->operand, op2->term == VAL ? NO_DATATYPE : op2->type
		);
		nonTerm->operand.index_size = newStackIndex;
		newStackIndex += datatypesize(resultType); // misto pro pomocnou promennou
	}