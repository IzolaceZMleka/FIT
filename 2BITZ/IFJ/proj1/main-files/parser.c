/* ***************************************************************** */
/*
	projekt: Implementace interpretu imperativniho jazyka IFJ15
	autori: Tomas Zahradnicek
	xvelec07: Jan Velecky
	xzahra24: Tomas Zahradnicek
	xpiece00: Adam Piecek
	xpavli78: Jan Pavlica
*/
/* ***************************************************************** */
#include "symboltable.h"
#include "ial.h"
#include "listparam.h"
#include "strvars.h"
#include "lexer.h"
#include "parser.h"
#include "errors.h"
#include "instructions.h"
#include "expression.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h> // pouze na testovy vypis

/*	makra zjednodusujci zapis overeni zda token je jeden z overovanych terminalu.
	argumeny makra musi byt jen samotne promenne nebo konstanty */
#define isin2(token, t1, t2) (token == t1 || token == t2)
#define isin3(token, t1, t2, t3) (token == t1 || token == t2 || token == t3)
#define isin4(token, t1, t2, t3, t4) (token == t1 || token == t2 || token == t3 || token == t4)

/* makra zkracujci zapis kontrolovani terminalu a neterminalu, a cteni terminalu */
#define chkt(t) 											\
	do {													\
		if (_tok != t) {									\
			errorcode = E_SYN;								\
			if (_tok == LEX_ERROR_LEX) errorcode = E_LEX;	\
			if (_tok == LEX_ERROR_IN) errorcode = E_INTER;	\
			return false;									\
		}													\
	} while (0)												
#define chknt(nt) do {if (!nt) {return false;}} while (0)
#define gettok do {_tok = lexerGetLexem(); if (_tok == LEX_ERROR_LEX) {errorcode = E_LEX; return false;} if (_tok == LEX_ERROR_IN) {errorcode = E_INTER; return false;}} while (0)
	
extern symboltable_t symboltable;

static int errorcode;
static int _tok; // token nacteny z lexikalniho analyzatoru
static int datatype; // hodnoty definovane v instructions.h (a jsou stejne i v lexer.h pro odpovidajci tokeny definujci datovy typ)
static htab_data *funData; // ukazatel na data prave parsovane funkce
static htab_data *varData; // ukazatel na data prave parsovane promenne
static htab_data *assignData; // ukazatel na data prave volane funkce
static size_t frameSizeParam; // velikost potrebna pro parametry funkce
static size_t frameSizeLocal; // velikost potrebna pro parametry + ve funkci definovane promenne + docasne promenne
static size_t sizeRealParam; // velikost potrebna k vypoctu prave parsovaneho skutecneho parametru
static int totalUndefinedFun = 0; // celkovy pocet deklarovanych, ale jeste nedefinovanych funkci

bool ntProg();
bool ntDefs();
bool ntFDef();
bool ntType();
bool ntFParams();
bool ntFParamsL();
bool ntFDefe();
bool ntStmts();
bool ntVDef();
bool ntTypeA();
bool ntInit();
bool ntStmt();
bool ntElse();
bool ntTerm(bool *isconst, Toperand *op);
bool ntCinL();
bool ntCoutL();
bool ntAssign();
bool ntParams();
bool ntParamsL();
bool ntExprDummy();
bool loadfun(); // nacte built-in funkce a funkci main do tabulky symbolu
bool testMain(); // zkontroluje definici funkce main, navratovy typ a 0 parametru

/*	samotna funkce pro parsovani celeho programu, zacina pocatecnim neterminalem */
void parse() {
	if (!ntProg() && errorcode == 0)
		errorcode = E_SYN;
	if (totalUndefinedFun != 0 && errorcode == 0)
		errorcode = E_SEM_IN_PROG; // existuje deklarovana ale nedefinovana funkce
	if (!testMain() && errorcode == 0)
		errorcode = E_SEM_IN_PROG;
	if (errorcode != 0)
		errored(errorcode);
	return;
}

/* nasleduji definice funkci odpovidajcim neterminalum */

bool ntProg(){
	Toperand1 op1 = { 0 };
	Toperand op2 = { 0 };
	gettok; // nacteni prvniho tokenu zdrojoveho kodu
	symboltable_init(&symboltable);
	if (symboltable_add(&symboltable) == 0)  // pridani globalni tabulky symbolu (pro funkce)
		{errorcode = E_INTER; return false;}
	if (!loadfun())
		{errorcode = E_INTER; return false;} // nepodarilo se nacist zakladni funkce do tabulky symbolu
	// PROG -> DEFS
	if (isin4(_tok, LEX_INT, LEX_DOUBLE, LEX_STRING, LEX_BOOL)) {
		tprint("1: PROG -> DEFS");
		chknt(ntDefs());
		if (generateInstr(BC_EOP, op1, op2) == NULL)
			{errorcode = E_INTER; return false;} // chyba alokace pameti
		return true;
		
	}
	else
		return false;
}

bool ntDefs() {
	// DEFS -> FDEF DEFS
	if (isin4(_tok, LEX_INT, LEX_DOUBLE, LEX_STRING, LEX_BOOL)) {
		tprint("2: DEFS -> FDEF DEFS");
		chknt(ntFDef());
		return ntDefs();
	}
	// DEFS -> eps
	else if(_tok == LEX_EOF){
		tprint("3: DEFS -> eps");
		return true;
	}
	else
		return false;
}


bool ntFDef() {
	// FDEF -> TYPE id ( FPARAMS ) FDEFE
	if (isin4(_tok, LEX_INT, LEX_DOUBLE, LEX_STRING, LEX_BOOL)){
		tprint("4: FDEF -> TYPE id ( FPARAMS ) FDEFE");
		chknt(ntType());
		chkt(LEX_ID);
		funData = symboltable_symbolInsert(&symboltable, identifier);
		if (funData == NULL) {errorcode = E_INTER; return false;}
		if (funData->defined == ISNEW) {
			funData->type = datatype; // navratovy typ
			funData->isfun = ISFUN; // jedna se identifikator uzivatlske funkce
		}
		else
			if (funData->type != datatype || funData->isfun != ISFUN) // znovudeklarace funkce, kontrola navratoveho typu a zda jde o uzivatelskou funkci
				{errorcode = E_SEM_IN_PROG; return false;}
		gettok;
		chkt(LEX_L_PARENTHESIS);
		gettok;
		chknt(ntFParams());
		chkt(LEX_R_PARENTHESIS);
		gettok;
		return (ntFDefe());
	}
	else
		return false;
}

bool ntType() {
	// TYPE -> int
	// TYPE -> double
	// TYPE -> string
	if (isin4(_tok, LEX_INT, LEX_DOUBLE, LEX_STRING, LEX_BOOL)) {
		if (_tok == LEX_INT) {
			tprint("5: TYPE -> int");
		}
		else if (_tok == LEX_DOUBLE) {
			tprint("6: TYPE -> double");
		}
		else if (_tok == LEX_STRING) {
			tprint("7: TYPE -> string");
		}
		else if (_tok == LEX_BOOL) {
			tprint(": TYPE -> bool");
		}
		datatype = _tok; // hodnoty pro datove typy jsou stejne v lexer.h i v instructions.h
		gettok;
		return true;
	}
	else
		return false;
}

bool ntFParams() {
	Tparam *funParam;
	// FPARAMS -> TYPE id FPARAMSL
	if (isin4(_tok, LEX_INT, LEX_DOUBLE, LEX_STRING, LEX_BOOL)) {
		tprint("8: FPARAMS -> TYPE id FPARAMSL");
		chknt(ntType());
		chkt(LEX_ID);
		if (funData->defined == ISNEW) {
			//funkce je nova, vlozi se parametr
			if (listParam_insertLast(&funData->listParam, datatype, identifier) == 0)
				{errorcode = E_INTER; return false;} // chyba alokace pameti
		}
		else {
			//znovudeklarace funkce, kontroluji se parametry
			listParam_first(&funData->listParam);
			funParam = listParam_read(&funData->listParam);
			if (funParam == NULL || funParam->datatype != datatype || strcmp(funParam->name, identifier) != 0)
				// parametr v 1. deklaraci neni || lisi se datovy typ parametru || lisi se nazev parametru
				{errorcode = E_SEM_IN_PROG; return false;} // pokus o zmenu parametru funkce
		}
		gettok;
		return ntFParamsL();
	}
	// FPARAMS -> eps
	else if (_tok == LEX_R_PARENTHESIS) {
		tprint("9: FPARAMS -> eps");
		if (funData->defined != ISNEW) {
			//znovudeklarace funkce, kontroluje se zda v predchozi deklaraci neni dalsi parametr
			listParam_first(&funData->listParam);
			if (listParam_isactive(&funData->listParam))
				// v puvodni deklaraci jsou jeste dalsi parametry
				{errorcode = E_SEM_IN_PROG; return false;} // pokus o zmenu parametru funkce funkce
		}
		return true;
	}
	else
		return false;
}

bool ntFParamsL() {
	Tparam *funParam;
	// FPARAMSL -> , TYPE id FPARAMSL
	if (_tok == LEX_COMMA) {
		tprint("10: FPARAMSL -> , TYPE id FPARAMSL");
		gettok;
		chknt(ntType());
		chkt(LEX_ID);
		if (funData->defined == ISNEW) {
			//funkce je nova, vlozi se parametr
			if (listParam_insertLast(&funData->listParam, datatype, identifier) == 0)
				{errorcode = E_INTER; return false;}
		}
		else {
			//znovudeklarace funkce, kontroluji se parametry
			listParam_succ(&funData->listParam);
			funParam = listParam_read(&funData->listParam);
			if (funParam == NULL || funParam->datatype != datatype || strcmp(funParam->name, identifier) != 0)
				// parametr v 1. deklaraci neni || lisi se datovy typ parametru || lisi se nazev parametru
				{errorcode = E_SEM_IN_PROG; return false;} // pokus o zmenu parametru funkce
		}
		gettok;
		return ntFParamsL();
	}
	// FPARAMSL -> eps
	else if (_tok == LEX_R_PARENTHESIS) {
		tprint("11: FPARAMSL -> eps");
		if (funData->defined != ISNEW) {
			//znovudeklarace funkce, kontroluje se zda uz neni dalsi parametr
			listParam_succ(&funData->listParam);
			if (listParam_isactive(&funData->listParam))
				// v drivejsi deklaraci jsou jeste dalsi parametry
				{errorcode = E_SEM_IN_PROG; return false;} // pokus o zmenu parametru funkce
		}
		return true;
	}
	else
		return false;
}

bool ntFDefe() {
	// FDEFE -> ;
	if (_tok == LEX_SEMICOLON) {
		tprint("12: FDEFE -> ;");
		if (funData->defined == ISNEW) { // prvni deklarace funkce
			funData->defined = ISDECLARED;
			totalUndefinedFun++;
		}
		gettok;
		return true;
	}
	// FDEFE -> { STMTS }
	else if (_tok == LEX_L_CURLY_BRACKETS) {
		Tinstruction *instrInit; // ukazatel na instrukce ve kterych se musi modifikovat ukazatel
		Tparam *funParam; // parametr prave pridavany do lokalni tabulky symbolu
		Toperand1 op1 = { 0 };
		Toperand op2 = { 0 };
		
		tprint("13: FDEFE -> { STMTS }");
		if (funData->defined == ISDEFINED)
			{errorcode = E_SEM_IN_PROG; return false;} // pokus o redefinici funkce
		
		if (funData->defined == ISDECLARED)
			totalUndefinedFun--;
	
		funData->defined = ISDEFINED; // prvni definice funkce funkce
		frameSizeLocal = 0;
		frameSizeParam = 0;
		instrInit = generateInstr(BC_INIT, op1, op2); // operandy teto instrukce se musi prepsat az budou znamy spravne hodnoty
		funData->begin = instrInit; // funkce zacina touto instrukci (INIT s pozdeji upravenymi operandy)
		if (instrInit == NULL) {errorcode = E_INTER; return false;} // chyba alokace pameti
		symboltable_add(&symboltable); // vytvoreni lokalni tabulky symbolu
		
		//nacteni formalnich parametru do lokalni tabulky symbolu
		listParam_first(&funData->listParam);
		while ((funParam = listParam_read(&funData->listParam)) != NULL) { // dokud je parametr ktery pridat
			varData = symboltable_symbolSearchAll(&symboltable, identifier);
			if (varData != NULL && varData->isfun != ISVAR)
				{errorcode = E_SEM_IN_PROG; return false;} // stejny nazev parametru a funkce
			varData = symboltable_symbolInsert(&symboltable, funParam->name); // vlozeni parametru do lokalni tabulky symbolu
			if (varData == NULL) {errorcode = E_INTER; return false;} // chyba alokace pameti
			if (varData->defined != ISNEW) {errorcode = E_SEM_IN_PROG; return false;} // chyba, 2 parametry maji stejny identifikator
			varData->defined = ISDEFINED;
			varData->isfun = ISVAR;
			varData->type = funParam->datatype;
			varData->index = frameSizeParam;
			if (funParam->datatype == DATATYPE_STRING)
				strvars_add(&funData->strvars, frameSizeParam); // prida se do seznamu stringovych  promennych funkce
			frameSizeParam += datatypesize(funParam->datatype); // rezervuje se pametovehe misto pro parametr v ramci
			listParam_succ(&funData->listParam); // posunuti na dalsi parametr
		}
		frameSizeLocal = frameSizeParam; // lokalni promenne budou v pameti navazovat na parametry
		
		gettok;
		chknt(ntStmts()); // generovani celeho kodu funkce
		chkt(LEX_R_CURLY_BRACKETS);
		gettok;
		
		funData->index = frameSizeLocal;
		// konecne upraveni instrukce INIT na zacatku funkce
		instrInit->op1.index_size = frameSizeParam;
		instrInit->op2.index_size = funData->index;
		
		op1.integer = E_RUN_UNINIT_VAR;
		op2.integer = 0;
		if (generateInstr(BC_ERROR, op1, op2) == NULL) // na tuto instrukci by se nemelo dojit, pokud je spravne RETURN
			{errorcode = E_INTER; return false;} // chyba alokace 
		symboltable_return(&symboltable);
		return true;
	}
	else
		return false;
}

bool ntStmts() { // v tomto nonterminalu nejsou potreba zadne semanticke akce
	// STMTS -> STMT STMTS
	if (isin4(_tok, LEX_L_CURLY_BRACKETS, LEX_ID, LEX_IF, LEX_FOR) || isin2(_tok, LEX_WHILE, LEX_DO) ||isin3(_tok, LEX_RETURN, LEX_CIN, LEX_COUT)) {
		tprint("14: STMTS -> STMT STMTS");
		chknt(ntStmt());
		return ntStmts();
	}
	// STMTS -> VDEF STMTS
	else if (isin4(_tok, LEX_AUTO, LEX_INT, LEX_DOUBLE, LEX_STRING) || _tok == LEX_BOOL) {
		tprint("15: STMTS -> VDEF STMTS");
		chknt(ntVDef());
		return ntStmts();
	}
	// STMTS -> eps
	else if (_tok == LEX_R_CURLY_BRACKETS) {
		tprint("16: STMTS -> eps");
		return true;
	}
	else
		return false;
}

bool ntVDef() {
	// VDEF -> TYPEA id INIT
	if (isin4(_tok, LEX_AUTO, LEX_INT, LEX_DOUBLE, LEX_STRING) || _tok == LEX_BOOL) {
		tprint("17: VDEF -> TYPEA id INIT");
		chknt(ntTypeA()); // nastavi datatype
		chkt(LEX_ID);
		
		varData = symboltable_symbolSearchAll(&symboltable, identifier);
		if (varData != NULL && varData->isfun != ISVAR)
			{errorcode = E_SEM_IN_PROG; return false;} // stejny nazev promenne a funkce
		varData = symboltable_symbolInsert(&symboltable, identifier);
		if (varData == NULL) {errorcode = E_INTER; return false;} // chyba alokace
		if (varData->defined != ISNEW) {errorcode = E_SEM_IN_PROG; return false;} // redefinice promenne
		varData->isfun = ISVAR;
		varData->defined = ISDEFINED;
		varData->type = datatype;
		varData->index = frameSizeLocal;
		frameSizeLocal += datatypesize(varData->type); // rezervuje se misto pro prave definovanou promennou 
		                                               // (pokud je promenna typu auto, nic se nepricte, pouzije se misto pro 1. docasnou promennou z inicializace)
		
		gettok;
		chknt(ntInit());
		
		if (varData->type == NO_DATATYPE)
			{errorcode = E_SEM_DERIV_TYP; return false;} // s modifikatorem auto nebyla pouzita inicializace
		if (varData->type == DATATYPE_STRING)
			strvars_add(&funData->strvars, varData->index); // prida se do seznamu stringovych  promennych funkce
		
		return true;
	}
	else
		return false;
}

bool ntTypeA() {
	// TYPEA -> TYPE
	if (isin4(_tok, LEX_INT, LEX_DOUBLE, LEX_STRING, LEX_BOOL)) {
		tprint("18 TYPEA -> TYPE");
		chknt(ntType());
		return true;
	}
	// TYPEA -> auto
	else if (_tok == LEX_AUTO) {
		tprint("19 TYPEA -> auto");
		datatype = NO_DATATYPE;
		gettok;
		return true;
	}
	else
		return false;
}

bool ntInit() {
	// INIT -> ;
	if (_tok == LEX_SEMICOLON) {
		tprint("20 INIT -> ;");
		gettok;
		return true;
	}
	// INIT -> = EXPR ;
	else if (_tok == LEX_BO_ASSIGNMENT) {
		tprint("21 INIT -> = EXPR ;");
		_tok = LEX_EOF; // znaci pro analyzator vyrazu ze nebyl prednacten zadny token
		frameSizeLocal += expressionAnalyse(frameSizeLocal, &varData->type, varData->index, &_tok, BC_LOAD|INSTR_MOVE);
		chkt(LEX_SEMICOLON);
		gettok;
		return true;
	}
	else
		return false;
}

bool ntStmt() {
	Tinstruction *instrif; // instrukce skoku na kod v pripad true podminky
	Tinstruction *instrelse; // instrukce skoku na kod v pripade false podminky
	Tinstruction *instrend; // instrukce skoku na kod konce podminky (v if/else), nebo 1. instrukce inkrementace (ve for, skace se na ni z konce tela cyklu)
	Tinstruction *instrcond; // 1. instrukce vyhodnocovani podminky (ve for se na ni skace po inkrementaci, ve while na konci tela cyklu)
	Toperand1 op1 = { 0 };
	Toperand op2 = { 0 };
	// STMT -> { STMTS }
	if (_tok == LEX_L_CURLY_BRACKETS) {
		tprint("22 STMT -> { STMTS }");
		if (symboltable_add(&symboltable) == 0) // vnoreni do bloku
			{errorcode = E_INTER; return false;} // chyba alokace pameti
		gettok;
		chknt(ntStmts());
		chkt(LEX_R_CURLY_BRACKETS);
		symboltable_return(&symboltable); // vynoreni z bloku
		gettok;
		return true;
	}
	// STMT -> id = ASSIGN
	else if (_tok == LEX_ID) {
		tprint("23 STMT -> id = ASSIGN");
		varData = symboltable_symbolSearchAll(&symboltable, identifier);
		gettok;
		chkt(LEX_BO_ASSIGNMENT);
		if (varData == NULL)
			{errorcode = E_SEM_IN_PROG; return false;} // nedefinovana promenna
		if (varData->isfun != ISVAR)
			{errorcode = E_SEM_OTH; return false;} // pokus prirazeni do funkce
		gettok;
		chknt(ntAssign());
		return true;
	}
	// STMT -> while ( EXPR ) STMT
	else if (_tok == LEX_WHILE) {
		tprint("STMT -> while ( EXPR ) STMT");
		gettok;
		chkt(LEX_L_PARENTHESIS);
		
		instrcond = nextInstr(); // ukazatel na 1. instrukci vyhodnoceni podminky
		
		op1.index_size = frameSizeLocal; // index vysledku podminky
		datatype = NO_DATATYPE;
		_tok = LEX_EOF; // znaci pro analyzator vyrazu ze nebyl prednacten zadny token
		frameSizeLocal += expressionAnalyse(frameSizeLocal, &datatype, frameSizeLocal, &_tok, BC_LOAD);
		
		chkt(LEX_R_PARENTHESIS);
		gettok;
		
		instrelse = generateCInstr(BC_CJMP, 0, op1, datatype, op2, 0); // pozdeji se prepise cil skoku na 1. instrukci mimo cyklus
		if (instrelse == NULL) {errorcode = E_INTER; return false;} // chyba alokace pameti
		if (symboltable_add(&symboltable) == 0) // vnoreni do tela cyklu
			{errorcode = E_INTER; return false;} // chyba alokace pameti
		
		chknt(ntStmt()); // generace kodu tela cyklu
		symboltable_return(&symboltable); // vynoreni z bloku tela cyklu
		
		op1.ptr = instrcond;
		if (generateInstr(BC_JMP, op1, op2) == NULL) // skok na zacatek vyhodnocovani podminky
			{errorcode = E_INTER; return false;}
		instrelse->op2.ptr = nextInstr(); // podmineny skok mimo cyklus v pripade false podminky
		
		return true;
	}
	// STMT -> do STMT while ( EXPR ) ;
	else if (_tok == LEX_DO) {
		tprint("STMT -> do STMT while ( EXPR ) ;");
		gettok;
		
		instrif = nextInstr(); // ukazatel na 1. instrukci tela cyklu
		if (symboltable_add(&symboltable) == 0) // vnoreni do bloku tela cyklu
			{errorcode = E_INTER; return false;} // chyba alokace pameti
		
		chknt(ntStmt()); // generace kodu pro true vetev
		symboltable_return(&symboltable); // vynoreni z bloku tela cyklu
		
		chkt(LEX_WHILE);
		gettok;
		chkt(LEX_L_PARENTHESIS);
		
		op1.index_size = frameSizeLocal; // index vysledku podminky
		datatype = NO_DATATYPE;
		_tok = LEX_EOF; // znaci pro analyzator vyrazu ze nebyl prednacten zadny token
		frameSizeLocal += expressionAnalyse(frameSizeLocal, &datatype, frameSizeLocal, &_tok, BC_LOAD);
		
		instrelse = generateCInstr(BC_CJMP, 0, op1, datatype, op2, 0); // pozdeji se prepise cil skoku na 1. instrukci mimo cyklus
		if (instrelse == NULL) {errorcode = E_INTER; return false;} // chyba alokace pameti
		op1.ptr = instrif;
		if (generateInstr(BC_JMP, op1, op2) == NULL) // skok na zacatek tela cyklu
			{errorcode = E_INTER; return false;}
		instrelse->op2.ptr = nextInstr(); // upraven cil skoku mimo cyklus v pripade false podminky
		
		chkt(LEX_R_PARENTHESIS);
		gettok;
		chkt(LEX_SEMICOLON);
		gettok;
		
		return true;
	}
	// STMT -> if ( EXPR ) STMT ELSE
	else if (_tok == LEX_IF) {
		tprint("24 STMT -> if ( EXPR ) STMT ELSE");
		gettok;
		chkt(LEX_L_PARENTHESIS);
		
		op1.index_size = frameSizeLocal; // index vysledku podminky
		datatype = NO_DATATYPE;
		_tok = LEX_EOF; // znaci pro analyzator vyrazu ze nebyl prednacten zadny token
		frameSizeLocal += expressionAnalyse(frameSizeLocal, &datatype, frameSizeLocal, &_tok, BC_LOAD);
		
		chkt(LEX_R_PARENTHESIS);
		gettok;
		
		instrelse = generateCInstr(BC_CJMP, 0, op1, datatype, op2, 0); // pozdeji se prepise cil skoku na false vetev
		if (instrelse == NULL) {errorcode = E_INTER; return false;} // chyba alokace pameti
		if (symboltable_add(&symboltable) == 0) // vnoreni do if bloku
			{errorcode = E_INTER; return false;} // chyba alokace pameti
		
		chknt(ntStmt()); // generace kodu pro true vetev
		symboltable_return(&symboltable); // vynoreni z if bloku
		
		chknt(ntElse(instrelse)); // dobrovolna else vetev
		
		return true;
	}
	// STMT -> for ( VDEF EXPR ; id = EXPR ) STMT
	else if (_tok == LEX_FOR) {
		tprint("25 STMT -> for ( VDEF EXPR ; id = EXPR ) STMT");
		gettok;
		chkt(LEX_L_PARENTHESIS);
		gettok;
		
		// inicializacni cast
		if (symboltable_add(&symboltable) == 0) // vnoreni do bloku hlavicky cyklu (do tohoto bloku patri pouze iteracni promenna)
			{errorcode = E_INTER; return false;} // chyba alokace pameti
		chknt(ntVDef());
		
		//cast podminky
		op1.index_size = frameSizeLocal; // index vysledku podminky
		datatype = NO_DATATYPE;
		instrcond = nextInstr(); // prvni instrukce vyhodnocovani podminky, skace se sem po inkrementacni casti
		frameSizeLocal += expressionAnalyse(frameSizeLocal, &datatype, frameSizeLocal, &_tok, BC_LOAD);

		instrelse = generateCInstr(BC_CJMP, 0, op1, datatype, op2, 0); // pozdeji se prepise cil skoku na prvni instrukci za cyklem
		if (instrelse == NULL) {errorcode = E_INTER; return false;} // chyba alokace pameti
		op1.ptr = NULL;
		instrif = generateInstr(BC_JMP, op1, op2); // pozdeji se prepise cil skoku na zacatek tela cyklu
		if (instrelse == NULL) {errorcode = E_INTER; return false;} // chyba alokace pameti
		
		chkt(LEX_SEMICOLON);
		gettok;
		chkt(LEX_ID);
		
		//inkrementacni cast
		instrend = nextInstr(); // prvni instrukce inkrementace cyklu, skace se po telu cyklu
		varData = symboltable_symbolSearchAll(&symboltable, identifier);
		if (varData == NULL)
			{errorcode = E_SEM_IN_PROG; return false;} // nedefinovana promenna
		if (varData->isfun != ISVAR)
			{errorcode = E_SEM_OTH; return false;} // pokus prirazeni do funkce
		gettok;
		chkt(LEX_BO_ASSIGNMENT);
		_tok = LEX_EOF; // znaci pro analyzator vyrazu ze nebyl prednacten zadny token
		frameSizeLocal += expressionAnalyse(frameSizeLocal, &varData->type, varData->index, &_tok, BC_LOAD|INSTR_MOVE);
		chkt(LEX_R_PARENTHESIS);
		gettok;
		op1.ptr = instrcond;
		if (generateInstr(BC_JMP, op1, op2) == NULL) // skok na zacatek vyhodnocovani podminky
			{errorcode = E_INTER; return false;} // chyba alokace
		
		//zacatek tela cyklu
		instrif->op1.ptr = nextInstr(); // opraven cil nepodmineneho skoku v pripade true podminky
		if (instrif->op1.ptr == NULL) {errorcode = E_INTER; return false;} // chyba alokace pameti
		if (symboltable_add(&symboltable) == 0) // vnoreni do bloku tela cyklu
			{errorcode = E_INTER; return false;} // chyba alokace pameti
		
		chknt(ntStmt());
		
		op1.ptr = instrend;
		if (generateInstr(BC_JMP, op1, op2) == NULL) // skok na zacatek inkrementace cyklu
			{errorcode = E_INTER; return false;} // chyba alokace
		instrelse->op2.ptr = nextInstr(); // podmineny skok mimo cyklus v pripade false podminky
		symboltable_return(&symboltable); // vynoreni z bloku tela cyklu
		symboltable_return(&symboltable); // vynoreni z bloku hlavicky cyklu
		
		return true;
	}
	// STMT -> return EXPR ;
	else if (_tok == LEX_RETURN) {
		tprint("26 STMT -> return EXPR ;");
		_tok = LEX_EOF; // znaci pro analyzator vyrazu ze nebyl prednacten zadny token
		op1.index_size = frameSizeLocal; // index, kam bude vypocitana navratova hodnota
		frameSizeLocal += datatypesize(funData->type);
		frameSizeLocal += expressionAnalyse(frameSizeLocal, &funData->type, op1.index_size, &_tok, BC_RETURN);
		chkt(LEX_SEMICOLON);
		gettok;
		return true;
	}
	// STMT -> cin >> id CINL
	else if (_tok == LEX_CIN) {
		tprint("27 STMT -> cin >> id CINL");
		gettok;
		chkt(LEX_IN);
		gettok;
		chkt(LEX_ID);
		varData = symboltable_symbolSearchAll(&symboltable, identifier);
		if (varData == NULL)
			{errorcode = E_SEM_IN_PROG; return false;} // nedefinovana promenna
		if (varData->isfun != ISVAR)
			{errorcode = E_SEM_OTH; return false;} // pokus cteni do funkce
		op1.index_size = varData->index;
		if (generateCInstr(BC_READ, 0, op1, varData->type, op2, 0) == NULL)
			{errorcode = E_INTER; return false;}
		gettok;
		chknt(ntCinL());
		return true;;
	}
	// STMT -> cout << EXPR COUTL
	else if (_tok == LEX_COUT) {
		bool isconst;
		tprint("28 STMT -> cout << EXPR COUTL");
		gettok;
		chkt(LEX_OUT);
		gettok;
		chknt(ntTerm(&isconst, &op2));
		if (generateCInstr(BC_WRITE, 0, op1, 0, op2, datatype|isconst<<2) == NULL)
			{errorcode = E_INTER; return false;}
		chknt(ntCoutL());
		return true;;
	}
	else
		return false;
}

bool ntElse(Tinstruction *instrelse) {
	// ELSE -> else STMT
	Tinstruction *instrend; // ukazatel na instrukci na konci else vetve, na kterou se skace z konce if vetve
	Toperand1 op1 = { 0 };
	Toperand op2 = { 0 };
	if (_tok == LEX_ELSE) {
		tprint("ELSE -> else STMT");
		instrend = generateInstr(BC_JMP, op1, op2); // pozdeji se prepise cil skoku z konce if vetve na konec else vetve
		if (instrend == NULL) {errorcode = E_INTER; return false;} // chyba alokace pameti
		instrelse->op2.ptr = nextInstr(); // opraven cil podmineneho skoku v pripade false podminky
		if (instrelse->op2.ptr == NULL) {errorcode = E_INTER; return false;} // chyba alokace pameti
		if (symboltable_add(&symboltable) == 0) // vnoreni do else bloku
			{errorcode = E_INTER; return false;} // chyba alokace pameti
		
		gettok;
		chknt(ntStmt()); // generace kodu pro false vetev
		
		instrend->op1.ptr = nextInstr(); // oprava nepodmineneho skoku na konci bloku true vetve
		if (instrend->op1.ptr == NULL) {errorcode = E_INTER; return false;} // chyba alokace pameti
		symboltable_return(&symboltable); // vynoreni z else bloku
		
		return true;
	}
	
	else {
		tprint("ELSE -> eps");
		instrelse->op2.ptr = nextInstr(); // opraven cil podmineneho skoku v pripade false podminky
		if (instrelse->op2.ptr == NULL) {errorcode = E_INTER; return false;} // chyba alokace pameti
		
		return true;
	}
}

bool ntTerm(bool *isconst, Toperand *op) {
	// TERM -> id
	if (_tok == LEX_ID) {
		tprint("29 TERM -> id");
		*isconst = 0;
		varData = symboltable_symbolSearchAll(&symboltable, identifier);
		if (varData == NULL)
			{errorcode = E_SEM_IN_PROG; return false;} // nedefinovana promenna
		if (varData->isfun != ISVAR)
			{errorcode = E_SEM_OTH; return false;} // term nemuze byt identifikator funkce
		datatype = varData->type;
		op->index_size = varData->index;
		gettok;
		return true;
	}
	// TERM -> const
	else if (isin3(_tok, LEX_LIT_INT, LEX_LIT_REAL, LEX_LIT_STRING) || isin2 (_tok, LEX_BOOL_T, LEX_BOOL_F)) {
		tprint("30 TERM -> const");
		*isconst = 1;
		switch (_tok) {
			case LEX_LIT_INT:
				datatype = DATATYPE_INT;
				op->integer = integer;
				break;
			case LEX_LIT_REAL:
				datatype = DATATYPE_DOUBLE;
				op->real = real;
				break;
			case LEX_LIT_STRING:
				datatype = DATATYPE_STRING;
				op->ptr = string;
				break;
			case LEX_BOOL_T:
				datatype = DATATYPE_INT;
				op->integer = 1;
				break;
			case LEX_BOOL_F:
				datatype = DATATYPE_INT;
				op->integer = 0;
				break;
		}
		gettok;
		return true;
	}
	else
		return false;
}


bool ntCinL() {
	Toperand1 op1 = { 0 };
	Toperand op2 = { 0 };
	// CINL -> >> id CINL
	if (_tok == LEX_IN) {
		tprint("31 CINL -> >> id CINL");
		gettok;
		chkt(LEX_ID);
		varData = symboltable_symbolSearchAll(&symboltable, identifier);
		if (varData == NULL)
			{errorcode = E_SEM_IN_PROG; return false;} // nedefinovana promenna
		if (varData->isfun != ISVAR)
			{errorcode = E_SEM_OTH; return false;} // pokus cteni do funkce
		op1.index_size = varData->index;
		if (generateCInstr(BC_READ, 0, op1, varData->type, op2 , 0) == NULL)
			{errorcode = E_INTER; return false;}
		gettok;
		chknt(ntCinL());
		return true;
	}
	// CINL -> ;
	else if (_tok == LEX_SEMICOLON) {
		tprint("32 CINL -> ;");
		gettok;
		return true;
	}
	else
		return false;
}

bool ntCoutL() {
	Toperand1 op1 = { 0 };
	Toperand op2 = { 0 };
	bool isconst;
	// COUTL -> << EXPR COUTL
	if (_tok == LEX_OUT) {
		tprint("33 CINL -> << EXPR COUTL");
		chkt(LEX_OUT);
		gettok;
		chknt(ntTerm(&isconst, &op2));
		if (generateCInstr(BC_WRITE, 0, op1, 0, op2, datatype|isconst<<2) == NULL)
			{errorcode = E_INTER; return false;}
		chknt(ntCoutL());
		return true;
	}
	// COUTL -> ;
	else if (_tok == LEX_SEMICOLON) {
		tprint("34 COUTL -> ;");
		gettok;
		return true;
	}
	else
		return false;
}

bool ntAssign() {
	size_t paramsi;
	Toperand1 op1;
	Toperand op2;
	bool isexpr = false;
	size_t resultSize = 0;
	if (isin4(_tok, LEX_L_PARENTHESIS, LEX_LIT_INT, LEX_LIT_REAL, LEX_LIT_STRING) || isin3(_tok, LEX_BOOL_T, LEX_BOOL_F, LEX_UO_NEG))
		isexpr = true; // prvni token je zavorka nebo literal, je to vyraz
	else if (_tok == LEX_ID) {
		assignData = symboltable_symbolSearchAll(&symboltable, identifier);
		if (assignData == NULL) {errorcode = E_SEM_IN_PROG; return false;}
		if (assignData->isfun == ISVAR)
			isexpr = true; // prvni token je identifikator promenne, je to vyraz
		
		// ASSIGN -> id ( PARAMS ) ;
		else { // prvni token je identifikator funkce, je to volani funkce
			tprint("ASSIGN -> id ( PARAMS ) ;");
			gettok;
			chkt(LEX_L_PARENTHESIS);
			gettok;
			sizeRealParam = 0;
			resultSize = datatypesize(assignData->type); // velikost navratoveho typu
			paramsi = frameSizeLocal; // zacatek skutecnych parametru v ramci volajci funkce, zaroven index vracene hodnoty
			chknt(ntParams());
			chkt(LEX_R_PARENTHESIS);
			
			op2.index_size = paramsi;
			if (assignData->isfun == ISFUN) { // vola se uzivatelska funkce
				op1.ptr = assignData;
				if (generateInstr(BC_CALL, op1, op2) == NULL)
					{errorcode = E_INTER; return false;} // chyba alokace pameti
			}
			else { // vola se vestavena funkce
				op1.index_size = assignData->index; // cislo vestavene funkce
				if (generateInstr(BC_CALLBIN, op1, op2) == NULL)
					{errorcode = E_INTER; return false;} // chyba alokace pameti
			}
		
			frameSizeLocal += (sizeRealParam > resultSize) ? sizeRealParam : resultSize; // pricte se potrebna pamet k vypoctu posledniho skutecneho parametru (bez parametru velikost navratoveho typu)
			op1.index_size = varData->index;
			op2.index_size = paramsi;
			if (generateCInstr(BC_LOAD|INSTR_RETVAL, INSTR_MOVE, op1, varData->type, op2, assignData->type) == NULL) // vracenou hodnotu nacte do promenne pred '='
				{errorcode = E_INTER; return false;} // chyba alokace pameti
			gettok;
			chkt(LEX_SEMICOLON);
			gettok;
			return true;
		}
	}
		
	// ASSIGN -> EXPR ;
	if (isexpr) {
		tprint("ASSIGN -> EXPR ;");
		frameSizeLocal += expressionAnalyse(frameSizeLocal, &varData->type, varData->index, &_tok, BC_LOAD|INSTR_MOVE);
		chkt(LEX_SEMICOLON);
		gettok;
		return true;
	}
	else
		return false;
}

bool ntParams() {
	Tparam *param;
	size_t prevSize;
	// PARAMS -> EXPR PARAMSL
	if (isin4(_tok, LEX_ID, LEX_LIT_INT, LEX_LIT_REAL, LEX_LIT_STRING) || isin2(_tok, LEX_BOOL_T, LEX_BOOL_F)) {
		tprint("PARAMS -> EXPR PARAMSL");
		listParam_first(&assignData->listParam);
		if (!listParam_isactive(&assignData->listParam))
			{errorcode = E_SEM_COMPAT_TYP; return false;} // neni dalsi formalni parametr (skutecnych je prilis mnoho)
		param = listParam_read(&assignData->listParam);
		prevSize = sizeRealParam; // predchozi velikost mista potrebne na vypocet skutecneho parametru
		sizeRealParam = expressionAnalyse(frameSizeLocal, &param->datatype, frameSizeLocal, &_tok, BC_LOAD|((assignData->isfun == ISFUN)?INSTR_PARAM:0));
		if (sizeRealParam < prevSize)
			sizeRealParam = prevSize; // zachova se nejvetsi potrebne misto potrebne na vypocet skutecneho parametru
		if (_tok == LEX_ERROR_LEX) {errorcode = E_LEX; return false;}
		if (_tok == LEX_ERROR_IN) {errorcode = E_INTER; return false;}
		frameSizeLocal += datatypesize(param->datatype);
		chknt(ntParamsL());
		return true;
	}
	// PARAMS -> eps
	else if (_tok == LEX_R_PARENTHESIS) {
		tprint("PARAMS -> eps");
		listParam_first(&assignData->listParam);
		if (listParam_isactive(&assignData->listParam))
			{errorcode = E_SEM_COMPAT_TYP; return false;} // prilis malo skutecnych parametru
		return true;
	}
	else
		return false;
}

bool ntParamsL() {
	Tparam *param;
	size_t prevSize;
	// PARAMSL -> , EXPR PARAMSL
	if (_tok == LEX_COMMA) {
		tprint("PARAMSL -> , EXPR PARAMSL");
		_tok = LEX_EOF; // znaci pro analyzator vyrazu ze nebyl prednacten zadny token
		listParam_succ(&assignData->listParam);
		if (!listParam_isactive(&assignData->listParam))
			{errorcode = E_SEM_COMPAT_TYP; return false;} // neni dalsi formalni parametr (skutecnych je prilis mnoho)
		param = listParam_read(&assignData->listParam);
		prevSize = sizeRealParam; // predchozi velikost mista potrebne na vypocet skutecneho parametru
		sizeRealParam = expressionAnalyse(frameSizeLocal, &param->datatype, frameSizeLocal, &_tok, BC_LOAD|((assignData->isfun == ISFUN)?INSTR_PARAM:0));
		if (sizeRealParam < prevSize)
			sizeRealParam = prevSize; // zachova se nejvetsi potrebne misto potrebne na vypocet skutecneho parametru
		if (_tok == LEX_ERROR_LEX) {errorcode = E_LEX; return false;}
		if (_tok == LEX_ERROR_IN) {errorcode = E_INTER; return false;}
		frameSizeLocal += datatypesize(param->datatype);
		chknt(ntParamsL());
		return true;
	}
	// PARAMSL -> eps
	else if (_tok == LEX_R_PARENTHESIS) {
		tprint("PARAMSL -> eps");
		listParam_succ(&assignData->listParam);
		if (listParam_isactive(&assignData->listParam))
			{errorcode = E_SEM_COMPAT_TYP; return false;} // prilis malo skutecnych parametru
		return true;
	}
	else
		return false;
}


bool loadfun() {
	htab_data *fun;
	
	// int length(string)
	fun = symboltable_symbolInsert(&symboltable, "length");
	if (fun == NULL) return false;
	fun->isfun = ISBINFUN;
	fun->type = DATATYPE_INT;
	fun->defined = ISDEFINED;
	fun->index = BIN_LEN;
	if (listParam_insertLast(&fun->listParam, DATATYPE_STRING, "") == 0) return false;
	
	// string substr(string, int, int)
	fun = symboltable_symbolInsert(&symboltable, "substr");
	if (fun == NULL) return false;
	fun->isfun = ISBINFUN;
	fun->type = DATATYPE_STRING;
	fun->defined = ISDEFINED;
	fun->index = BIN_SUBSTR;
	if (listParam_insertLast(&fun->listParam, DATATYPE_STRING, "") == 0) return false;
	if (listParam_insertLast(&fun->listParam, DATATYPE_INT, "") == 0) return false;
	if (listParam_insertLast(&fun->listParam, DATATYPE_INT, "") == 0) return false;
	
	// string concat(string, string)
	fun = symboltable_symbolInsert(&symboltable, "concat");
	if (fun == NULL) return false;
	fun->isfun = ISBINFUN;
	fun->type = DATATYPE_STRING;
	fun->defined = ISDEFINED;
	fun->index = BIN_CONCAT;
	if (listParam_insertLast(&fun->listParam, DATATYPE_STRING, "") == 0) return false;
	if (listParam_insertLast(&fun->listParam, DATATYPE_STRING, "") == 0) return false;
	
	// int find(string, string)
	fun = symboltable_symbolInsert(&symboltable, "find");
	if (fun == NULL) return false;
	fun->isfun = ISBINFUN;
	fun->type = DATATYPE_INT;
	fun->defined = ISDEFINED;
	fun->index = BIN_FIND;
	if (listParam_insertLast(&fun->listParam, DATATYPE_STRING, "") == 0) return false;
	if (listParam_insertLast(&fun->listParam, DATATYPE_STRING, "") == 0) return false;
	
	// string sort(string);
	fun = symboltable_symbolInsert(&symboltable, "sort");
	if (fun == NULL) return false;
	fun->isfun = ISBINFUN;
	fun->type = DATATYPE_STRING;
	fun->defined = ISDEFINED;
	fun->index = BIN_SORT;
	if (listParam_insertLast(&fun->listParam, DATATYPE_STRING, "") == 0) return false;
	
	return true;
}

bool testMain() {
	htab_data *fun;
	fun = symboltable_symbolSearchAll(&symboltable, "main");
	if (fun == NULL) return false; // nebyla deklarovana / definovana 
	if (fun->isfun != ISFUN || fun->type != DATATYPE_INT || fun->defined != ISDEFINED)
		return false;
	listParam_first(&fun->listParam);
	if (listParam_isactive(&fun->listParam)) return false; // funkce nema mit zadne parametry
	return true;
}