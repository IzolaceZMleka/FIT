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
#include "instructions.h"
#include <stdlib.h>
#include <assert.h>
#include "errors.h"

// pocet instrukci v kazdem bloku, pokud se dojde na konec, posledni je vzdy BC_JMP do noveho bloku
#define BLOCKSIZE 1024

Tinstruction* nextInstr() {
	if (byteProgram.nextInstr == NULL) { // zadny blok pro kod jeste neni alokovan, musi se predalokovat
		byteProgram.nextInstr = malloc(BLOCKSIZE * sizeof(Tinstruction));
		if (byteProgram.nextInstr == NULL) // chyba alokace
			return NULL;
	}
	return byteProgram.nextInstr;
}

Tinstruction* generateInstr(char byteCode, Toperand1 op1, Toperand op2) {
	Tinstruction instr;
	Tinstruction *newblock;
	if ((byteProgram.nextInstr == NULL) || (byteProgram.nextInstr == byteProgram.curBlockEnd)) {
		// musi se vytvorit novy blok
		newblock = malloc(BLOCKSIZE*sizeof(Tinstruction));
		if (newblock == NULL)
			return NULL;
		if (byteProgram.nextInstr != NULL) { // nejedna se o prvni vytvoreny blok, tedy se doslo na konec stavajciho bloku
			// vlozi se skok z konce bloku na zacatek nove vytvoreneho
			instr.code = BC_JMP;
			instr.op1.ptr = newblock;
			*byteProgram.curBlockEnd = instr;
		} else // jedna se o prvni alokovany blok, tuto informaci ulozime
			byteProgram.startBlock = newblock;
		byteProgram.nextInstr = newblock;
		byteProgram.curBlockEnd = newblock + BLOCKSIZE-1;
	}
	// pokracuje se nasledujci instrukci v poli
	instr.code = byteCode;
	instr.op1 = op1;
	instr.op2 = op2;
	*byteProgram.nextInstr = instr;
	return byteProgram.nextInstr++; // vrati ukazatel na pridanou instrukci, potom posune ukazatel na volne misto v alokovanem poli
}


#define MASK_DATATYPE 3 // 0b11 - pro vymaskovani datoveho typu, napr. z -1 udela 3 (synonymum pro NO_DATATYPE&MASK_DATATYPE)
#define MASK_NUMERIC 1 // 0b1 - pro vymaskovani cisleneho datoveho typu
Tinstruction* generateCInstr(unsigned char byteCode, int flags, Toperand1 op1, char type1, Toperand op2, char type2) {
	// ve vetsine pripadu s datovym typem BOOL pracujeme jako s cislem (o hodnote nula, nebo jedna)
	if(type2 == DATATYPE_BOOL) type2 = DATATYPE_INT;
	// tyto operace zajistuji konverzi jineho typu nez je bool do nej
	if(type1 == DATATYPE_BOOL && !((byteCode&BC_LOAD) == BC_LOAD || byteCode == BC_READ || byteCode == BC_RETURN))
		type1 = DATATYPE_INT;
	
	switch((unsigned char) byteCode) {
		case BC_CJMP:
			// [0] datovy typ kontrolovaneho vyrazu (pouze int nebo double)
			if(type1 == DATATYPE_STRING)
				errored(E_SEM_COMPAT_TYP); // string nemuze byt vysledkem vyrazu
			byteCode |= type1;
			break;
		case BC_RETURN:
			// pro cisla:
			// [3:2] datovy typ cilove hodnoty
			// [1:0] datovy typ navratove hodnoty (vcetne hodnoty NO_DATATYPE - konstanta)
			// pro stringy:
			// [2] priznak vraceni retezce literalu
			// [1:0] DATATYPE_STRING
			if(type1 != type2 && (type1 == DATATYPE_STRING || type2 == DATATYPE_STRING))
				errored(E_SEM_COMPAT_TYP); // nelze menit typ stringu
			if(type1 != DATATYPE_STRING) { // zakodovani pro cisla
				if(flags&INSTR_CONST && type1 == DATATYPE_BOOL) type1 = DATATYPE_INT;
				byteCode |= (type1&MASK_DATATYPE) << 2 | (flags&INSTR_CONST ? MASK_DATATYPE : type2);
			} else // zakodovani pro retezce
				byteCode |= (flags&INSTR_CONST) << 2 | type2;
			// instrukce RETURN pouziva pouze druhy operand, aby mohla vracet i konstantni realna cisla
			break;
		case BC_READ:
			// [1:0] datovy typ ctene hodnoty
			byteCode |= type1;
			break;
		case BC_WRITE:
			// [2] zda se jedna o literal (je soucasti type2)
			// [1:0] datovy typ zapisovane hodnoty
			byteCode |= type2;
			break;
		case BC_LOAD:
		case BC_LOAD|INSTR_RETVAL:
		case BC_LOAD|INSTR_PARAM:
		case BC_LOAD|INSTR_MOVE:
			// Pro cislene typy:
			// [3:2] datovy typ cile
			// Pro retezcove promenne:
			// [3] priznak inkrementace reference
			// [2] priznak dekrementace reference
			// Pro retezcove literaly:
			// [3:2] datovy typ string, navic [2] priznak dekrementace reference
			// Pro vsechny:
			// [1:0] datovy typ zdroje (vcetne hodnoty NO_DATATYPE - konstanta)
			if(type1 == DATATYPE_STRING || type2 == DATATYPE_STRING) {
				if(type1 != type2)
					errored(E_SEM_COMPAT_TYP); // nelze menit typ stringu	
				// zakodovani pro retezec
				byteCode = BC_LOAD | (((byteCode&INSTR_PARAM)|((flags&INSTR_CONST) << 2)|(byteCode&INSTR_RETVAL)) << 1); // RETVAL zacina od dvou, proto jenom 1
			} else if(type1 == DATATYPE_BOOL) // zakodovani pro booloevskou hodnotu
				byteCode = BC_LOAD | (flags&INSTR_CONST ? DATATYPE_INT : type1) << 2;
			else // zakodovani pro cislo
				byteCode = BC_LOAD | type1 << 2;
			// zakodovani zdrojoveho datoveho typu
			byteCode |= flags&INSTR_CONST ? MASK_DATATYPE : type2;
			break;
			
		// -------------- ALR instrukce ----------------
		case BC_ADD:
		case BC_MUL:
			// [2] datovy typ prvniho operandu (a cile)
			// [1] datovy typ druheho operandu
			// [0] priznak prioperovani konstanty
			if(type1 == DATATYPE_STRING || type2 == DATATYPE_STRING) // aritmeticke operace jsou pouze nad cisly
				errored(E_SEM_COMPAT_TYP);
			byteCode |= type1 << 2 | ((type2 != NO_DATATYPE) ? type2 << 1 : 1);
			break;
		case BC_SUB:
		case BC_DIV:
			// [3] priznak prohozeni operandu pri vykonani operace (cilovy operand, ale zustava)
			// [2] datovy typ prvniho operandu (a cile)
			// [1] datovy typ druheho operandu
			// [0] priznak prioperovani konstanty
			if(type1 == DATATYPE_STRING || type2 == DATATYPE_STRING) // aritmeticke operace jsou pouze nad cisly
				errored(E_SEM_COMPAT_TYP);
			byteCode |= !!flags << 3 | type1 << 2 | ((type2 != NO_DATATYPE) ? type2 << 1 : 1);
			break;
		case BC_CMPEQ:
		case BC_CMPNE:
			// vysledek je integer
			// [3:2] datovy typ prvniho operandu
			// [1:0] urceni typu druheho operandu (vcetne hodnoty NO_DATATYPE - konstanta)
			if(type1 != type2 && type2 != NO_DATATYPE && (type1 == DATATYPE_STRING || type2 == DATATYPE_STRING))
				errored(E_SEM_COMPAT_TYP); // nelze pretypovavat string
			byteCode |= type1 << 2 | (type2 & MASK_DATATYPE);
			break;
		case BC_CMPL:
		case BC_CMPG:
		case BC_CMPLE:
		case BC_CMPGE:
			// vysledek je integer
			// ^[4] priznak prohozeni operandu pri vykonani operace (cilovy operand, ale zustava)
			// musi se vyxorovat, tim se z instrukce stava jeji inverz (napr. BC_CMPLE je s reverzaci BC_CMPG)
			// [3:2] datovy typ prvniho operandu
			// [1:0] urceni typu druheho operandu (vcetne hodnoty NO_DATATYPE - konstanta)
			if(type1 != type2 && type2 != NO_DATATYPE && (type1 == DATATYPE_STRING || type2 == DATATYPE_STRING))
				errored(E_SEM_COMPAT_TYP); // nelze pretypovavat string
			byteCode ^= !!flags << 4 | type1 << 2 | (type2 & MASK_DATATYPE);
			break;
		case BC_AND:
		case BC_OR:
			// [1] datovy typ prvniho operandu
			// [0] datovy typ druheho operandu
			byteCode |= type1<<1 | type2;
			break;
		case BC_NEG:
			// [0] datovy typ operandu
			if(type1 != type2 && (type1 == DATATYPE_STRING || type2 == DATATYPE_STRING))
				errored(E_SEM_COMPAT_TYP); // nelze pretypovavat string
			byteCode |= type2&MASK_NUMERIC;
			break;
	}
	
	return generateInstr(byteCode, op1, op2); // vygenerovat finalni instrukci pro interpret
}

void byteProgramDispose() {
	Tinstruction * delBlock = byteProgram.startBlock,
		* aux;
	#ifndef NDEBUG
		int instrCount = 0;
	#endif
	if(!delBlock) return; // jeste nebyl alokovan zadny blok pro instrukce
	
	do {
		aux = delBlock;
		delBlock = aux[BLOCKSIZE-1].op1.ptr; // ulozime adresu dalsiho bloku, ta tam nebude pri poslednim bloku, ale to nevadi
		free(aux);
		#ifndef NDEBUG
			instrCount++;
		#endif
	} while(aux + BLOCKSIZE != byteProgram.curBlockEnd + 1); // opakujeme dokud nenarazime na posledni alokovany blok

	#ifndef NDEBUG
		tprint("Celkovy pocet instrukci: %d ", (int) (instrCount * BLOCKSIZE - (byteProgram.curBlockEnd - byteProgram.nextInstr) - 1)); // spocte, kolik bylo pouzito instrukci
	#endif
}
