/* ***************************************************************** */
/*
	projekt: Implementace interpretu imperativniho jazyka IFJ15
	autori: Jan Velecky, Tomas Zahradnicek, Adam Piecek
	xvelec07: Jan Velecky
	xzahra24: Tomas Zahradnicek
	xpiece00: Adam Piecek
	xpavli78: Jan Pavlica
*/
/* ***************************************************************** */
#include <string.h>
#include "interpret.h"
// #include "builtin_func.h"
#include "errors.h"
#include "read.h"

// Dekodovani instrukci s primou hodnotou jako operandem.
#define IS_CONSTANT 1
#define IS_SAMETYPE 3
// Zkratka pro datovy typ string.
#define string Tstring *
// Makro pro pouziti lokalni promenne ve spravnem typu.
#define usevariable(index, type) \
	(*(type *)(frame + index))
// Kontroluje, zda jiz je pouzivana promenna inicializovana. Pokud ne, nastava chyba kvuli praci s neinicializovanou promennou!
#define isinit(index) do {                                     \
		if(!initialized[index]) errored(E_RUN_UNINIT_VAR);     \
	} while(0)
// Makro oznaci zadanou promenou jako inicializovanou (provadi se pri zapisu hodnoty).
#define inited(index) (initialized[index] = 1)
// Inkrementuje pocet referenci na dany retezec, pokud nejde o literal. Nesmi se provest pro neinicializovane retezce.
#define straddref(index) do {                 \
	string str = usevariable(index, string);  \
	if(str->refs) {                           \
		debug("addref %d str: %s", str->refs+1, str->str);\
		str->refs++;                          \
	}                                         \
} while(0)
// Dekrementuje pocet referenci na dany retezec, pokud nejde o literal, a pripadne jej dealokuje, pokud na nej jiz zadna promenna v programu neodkazuje.
// Pro pouziti nad promenou. Zkontroluje prvni, zda je promenna inicializovana.
#define strunref(index) do {                                   \
	if(initialized[index]) {                                   \
		string str = usevariable(index, string);               \
		if(str->refs) { /* test, zda se nejedna o literal */   \
			debug("unref %d str: %s", str->refs-1, str->str);  \
			/* toto byl posledni odkaz na nej - dealokujeme*/  \
			if(str->refs == STR_TO_DELETE) free(str);          \
			/* nebo jenom snizime pocet referenci */           \
			else str->refs--;                                  \
		}                                                      \
	}                                                          \
} while(0)

void interpret(htab_data * func, size_t frameSize, DOUBLEWORD * callerFrame) {
	Tinstruction * PC = func->begin;   // odkaz na pocatecni instrukci podprogramu
	DOUBLEWORD frame[frameSize];    // ramec pro lokalni (a pomocne) promenne
	char initialized[frameSize]; // priznaky inicializovanosti promennych
	
	goto init_phase;
	
	// cyklus interpretovani instrukci
	while(1) {
		// Tinstruction IR = *PC; // nacteni instrukce
		#define IR (*PC)  // nactena instrukce
		tprint("INSTR: %d op1: %d op2: %d", (unsigned char) IR.code, IR.op1.integer, IR.op2.integer);
		
		switch((unsigned char) IR.code) {
			case BC_CJMP|DATATYPE_INT:
				if(!usevariable(IR.op1.index_size, int)) {
					PC = IR.op2.ptr;
					continue;
				}
				break;
			case BC_CJMP|DATATYPE_DOUBLE:
				if(!usevariable(IR.op1.index_size, double)) {
					PC = IR.op2.ptr;
					continue;
				}
				break;
			case BC_JMP:
				PC = IR.op1.ptr;
				continue;
			case BC_CALL: {
				htab_data * func = IR.op1.ptr;
				interpret(func, func->index, frame + IR.op2.index_size);
				inited(IR.op2.index_size); // funkce nemusi mit zadny parametr, musime tedy zajistit inicializaci prvni pomocne promenne pro vraceni
				break;
			}
			case BC_CALLBIN: {
				switch(IR.op1.index_size) {
					case BIN_LEN: {  // int length(string s);
						int res = length(usevariable(IR.op2.index_size, string));
						usevariable(IR.op2.index_size, int) = res;
						break;
					} case BIN_SUBSTR: { // string substr(string text, int i, int n);
						string res = substr(
							usevariable(IR.op2.index_size, string),
							usevariable(IR.op2.index_size + datatypesize(DATATYPE_STRING), int),
							usevariable(IR.op2.index_size + datatypesize(DATATYPE_STRING) + datatypesize(DATATYPE_INT), int)
						);
						usevariable(IR.op2.index_size, string) = res; // po odebrani referenci ulozime vysledek
						break;
					} case BIN_CONCAT: { // string concat(string s1, string s2);
						string res = concat(
							usevariable(IR.op2.index_size, string),
							usevariable(IR.op2.index_size + datatypesize(DATATYPE_STRING), string)
						);
						usevariable(IR.op2.index_size, string) = res; // po odebrani referenci ulozime vysledek
						break;
					} case BIN_FIND: { // int find(string text, string pattern);
						int res = find(
							usevariable(IR.op2.index_size, string),
							usevariable(IR.op2.index_size + datatypesize(DATATYPE_STRING), string)
						);
						usevariable(IR.op2.index_size, int) = res;
						break;
					} case BIN_SORT:   // string sort(string s);
					default: { // neni jine moznosti
						string res = sort(usevariable(IR.op2.index_size, string));
						usevariable(IR.op2.index_size, string) = res;
					}
				}
				break;
			}
			init_phase: // prvni provedena instrukce na zacatku kazde funkce
			case BC_INIT: {
				unsigned argn = IR.op1.index_size; // pocet elementarniho pametoveho mista pro argumenty
				// Puvodni varianty s memcpy na stejne pozici
				// memcpy(frame, callerFrame, argn * DOUBLEWORD_S); // predavani parametru obsluhuje volana funkce
				// Varianta A
				// memset(initialized, 0, frameSize);
				// for(unsigned i = 0; i < argn; i++) initialized[i] = 1; // parametry jsou vzdy zadane, tedy i inicializovane
				// memcpy(frame, callerFrame, argn * DOUBLEWORD_S); // predavani parametru obsluhuje volana funkce
				// Varianta B
				memcpy(frame, callerFrame, argn * DOUBLEWORD_S); // predavani parametru obsluhuje volana funkce
				for(unsigned i = 0; i < argn; i++) initialized[i] = 1; // parametry jsou vzdy zadane, tedy i inicializovane
				memset(initialized + argn, 0, frameSize - argn);
				// Varianta C
				// memcpy(frame, callerFrame, argn * DOUBLEWORD_S); // predavani parametru obsluhuje volana funkce
				// for(unsigned i = 0; i < argn; i++) initialized[i] = 1; // parametry jsou vzdy zadane, tedy i inicializovane
				// for(unsigned i = argn; i < frameSize; i++) initialized[i] = 0; // ostatni lokalni a pomocne promenne nejsou 
				break;
			}
			// ___________________________RETURN instrukce___________________________
			// Makro snizi pocet referenci na kazdy string definovany ve funkci
			#define cleanstrvars(retstrvar) do {                     \
				int * arr = func->strvars;                           \
				/* kontrola na zarazku */                            \
				if (arr != NULL) {                                   \
					if(retstrvar) {                                  \
						while(*arr != (signed) IR.op2.index_size) {  \
							strunref(*arr);                          \
							arr++;                                   \
						}                                            \
						arr++;                                       \
					}                                                \
					while(*arr >= 0) {                               \
						strunref(*arr);                              \
						arr++;                                       \
					}                                                \
				}                                                    \
			} while(0)
			case BC_RETURN|DATATYPE_INT<<2|DATATYPE_INT:                                                                       
                isinit(IR.op2.index_size);
				*(int *) callerFrame = usevariable(IR.op2.index_size, int);
				cleanstrvars(0);
				return;
			case BC_RETURN|DATATYPE_INT<<2|DATATYPE_DOUBLE:                                                                       
                isinit(IR.op2.index_size);
				*(int *) callerFrame = usevariable(IR.op2.index_size, double);
				cleanstrvars(0);
				return;
			case BC_RETURN|DATATYPE_STRING:                                                                       
                isinit(IR.op2.index_size);
				*(string *) callerFrame = usevariable(IR.op2.index_size, string);
				cleanstrvars(1); // vraci to string, ktery nechceme nechtene znicit
				return;
			case BC_RETURN|DATATYPE_INT<<2|IS_SAMETYPE:
				*(int *) callerFrame = IR.op2.integer;
				cleanstrvars(0);
				return;
			case BC_RETURN|DATATYPE_DOUBLE<<2|DATATYPE_INT:                                                                       
                isinit(IR.op2.index_size);
				*(double *) callerFrame = usevariable(IR.op2.index_size, int);
				cleanstrvars(0);
				return;
			case BC_RETURN|DATATYPE_DOUBLE<<2|DATATYPE_DOUBLE:                                                                       
                isinit(IR.op2.index_size);
				*(double *) callerFrame = usevariable(IR.op2.index_size, double);
				cleanstrvars(0);
				return;
			case BC_RETURN|DATATYPE_DOUBLE<<2|IS_SAMETYPE:
				*(double *) callerFrame = IR.op2.real;
				cleanstrvars(0);
				return;
			case BC_RETURN|IS_CONSTANT<<2|DATATYPE_STRING:
				*(string *) callerFrame = IR.op2.ptr;
				cleanstrvars(0);
				return;
			case BC_RETURN|DATATYPE_BOOL<<2|DATATYPE_INT:                                                                       
                isinit(IR.op2.index_size);
				*(int *) callerFrame = !!usevariable(IR.op2.index_size, int);
				cleanstrvars(0);
				return;
			case BC_RETURN|DATATYPE_BOOL<<2|DATATYPE_DOUBLE:                                                                       
                isinit(IR.op2.index_size);
				*(int *) callerFrame = !!usevariable(IR.op2.index_size, double);
				cleanstrvars(0);
				return;
			// ____________________________READ instrukce____________________________
			case BC_READ|DATATYPE_INT: 
                usevariable(IR.op1.index_size, int) = readInteger();
				inited(IR.op1.index_size);
                break;                                                                                        
            case BC_READ|DATATYPE_DOUBLE: 
                usevariable(IR.op1.index_size, double) = readDouble();
				inited(IR.op1.index_size);
                break;                                                                                        
            case BC_READ|DATATYPE_STRING:
				strunref(IR.op1.index_size);
                usevariable(IR.op1.index_size, string) = readString();
				inited(IR.op1.index_size);
                break;                                                                                         
            case BC_READ|DATATYPE_BOOL:
                usevariable(IR.op1.index_size, int) = !!readInteger();
				inited(IR.op1.index_size);
                break;
			// ___________________________WRITE instrukce____________________________
            case BC_WRITE|DATATYPE_INT:                                                                          
                isinit(IR.op2.index_size);                                                                       
                printf("%d", usevariable(IR.op2.index_size, int));                                            
                break;                                                                                        
            case BC_WRITE|DATATYPE_DOUBLE:                                                                    
                isinit(IR.op2.index_size);                                                                    
                printf("%g", usevariable(IR.op2.index_size, double));                                            
                break;                                                                                        
            case BC_WRITE|DATATYPE_STRING:                                                                    
                isinit(IR.op2.index_size);                                                                    
                printf("%s", usevariable(IR.op2.index_size, string)->str);                                            
                break;
			#define IS_LIT (1<<2)
			case BC_WRITE|IS_LIT|DATATYPE_INT:                                                                                                                                                
                printf("%d", IR.op2.integer);                                            
                break;                                                                                        
            case BC_WRITE|IS_LIT|DATATYPE_DOUBLE:                                                                                                                                     
                printf("%g", IR.op2.real);                                            
                break;                                                                                        
            case BC_WRITE|IS_LIT|DATATYPE_STRING:                                                                                                                                      
                printf("%s", ((string)(IR.op2.ptr))->str);                                            
                break;

			// ____________________________LOAD instrukce____________________________
			// Instrukce pro presuny mezi pametovymi misty a konverze mezi datovymi typy
			// Makra pro prevod datoveho typu na slozku unie
			#define int_OP integer
			#define double_OP real
			#define string_OP ptr
			// Specialni operace (inkrementace/dekrementace referenci) pro stringy
			#define int_REFS(u)
			#define double_REFS(u)
			#define string_REFS(const) {                                    \
				if(!(const&INSTR_CONST) && const&INSTR_PARAM)               \
					straddref(IR.op2.index_size);                           \
				if(const&INSTR_RETVAL && initialized[IR.op1.index_size])    \
					strunref(IR.op1.index_size);                            \
			}                                                  
			// generator kodu vysledne instrukce + spec. kod navic pro retezce
			#define instrgenload(typeout, typein, const) {                   \
				if(!((const) & INSTR_CONST)) isinit(IR.op2.index_size);      \
				typein##_REFS((const)); /* inkrementujeme a dekrementujeme */\
				usevariable(IR.op1.index_size, typeout) =                    \
					(const) & INSTR_CONST ? IR.op2.typein##_OP :             \
					usevariable(IR.op2.index_size, typein);                  \
				inited(IR.op1.index_size);                                   \
				break;                                                       \
			}
			// Varianty instrukce LOAD
			case BC_LOAD|DATATYPE_INT<<2|DATATYPE_INT:
				instrgenload(int, int, 0);
			case BC_LOAD|DATATYPE_INT<<2|DATATYPE_DOUBLE:
				instrgenload(int, double, 0);
			case BC_LOAD|DATATYPE_DOUBLE<<2|DATATYPE_INT:
				instrgenload(double, int, 0);
			case BC_LOAD|DATATYPE_DOUBLE<<2|DATATYPE_DOUBLE:
				instrgenload(double, double, 0);
			case BC_LOAD|DATATYPE_STRING: // nacitani stringu do vyrazu - bez upravy referenci
				instrgenload(string, string, 0);
			case BC_LOAD|INSTR_RETVAL<<1|DATATYPE_STRING: // navracena hodnota - pouze dekrementace
				instrgenload(string, string, INSTR_RETVAL);
			case BC_LOAD|INSTR_PARAM<<1|DATATYPE_STRING: // parametr funkce - pouze inkrementace
				instrgenload(string, string, INSTR_PARAM);
			case BC_LOAD|INSTR_MOVE<<1|DATATYPE_STRING: // prirazeni - upravy referenci
				instrgenload(string, string, INSTR_MOVE);
			case BC_LOAD|DATATYPE_INT<<2|IS_SAMETYPE:
				instrgenload(int, int, INSTR_CONST);
			case BC_LOAD|DATATYPE_DOUBLE<<2|IS_SAMETYPE:
				instrgenload(double, double, INSTR_CONST);
			case BC_LOAD|DATATYPE_STRING<<2|IS_SAMETYPE: // konstanta do vyrazu/parametru - bez upravy referenci
				instrgenload(string, string, INSTR_CONST);
			case BC_LOAD|DATATYPE_STRING<<2|INSTR_RETVAL<<1|IS_SAMETYPE: // konstanta navrace/prirazeni - pouze dekrementace
				instrgenload(string, string, INSTR_RETVAL|INSTR_CONST);
			case BC_LOAD|DATATYPE_BOOL<<2|DATATYPE_INT:
				isinit(IR.op2.index_size);
				usevariable(IR.op1.index_size, int) = !!usevariable(IR.op2.index_size, int);
				inited(IR.op1.index_size);
				break;
			case BC_LOAD|DATATYPE_BOOL<<2|DATATYPE_DOUBLE:
				isinit(IR.op2.index_size);
				usevariable(IR.op1.index_size, int) = !!usevariable(IR.op2.index_size, double);
				inited(IR.op1.index_size);
				break;
			// ____________________________ARL instrukce____________________________
			// ------------------------aritmeticke instrukce------------------------
			// makra na determinovani vysledneho datoveho typu
			#define int_int_RES int
			#define int_double_RES double
			#define double_int_RES double
			#define double_double_RES double
			// generator kodu vysledne instrukce
			#define instrgen(type1, type2, const, rev, _OP_) {          \
				if(!const) isinit(IR.op2.index_size);                   \
				usevariable(IR.op1.index_size, type1##_##type2##_RES) = \
					rev ? /* pripadne reverzovane poradi operandu */    \
						(const ? IR.op2.type2##_OP :                    \
						usevariable(IR.op2.index_size, type2)) _OP_     \
						usevariable(IR.op1.index_size, type1)           \
					:                                                   \
						usevariable(IR.op1.index_size, type1) _OP_      \
						(const ? IR.op2.type2##_OP :                    \
						usevariable(IR.op2.index_size, type2));         \
				inited(IR.op1.index_size);                              \
				break;                                                  \
			}
			// seznam vsech smysluplnych variant pro aritmeticke operandy
			#define instrsall(bC, rev, _OP_) {                             \
				case bC|DATATYPE_INT<<2|DATATYPE_INT<<1:                   \
					instrgen(int, int, 0, rev, _OP_);                      \
				case bC|DATATYPE_INT<<2|IS_CONSTANT:                       \
					instrgen(int, int, 1, rev, _OP_);                      \
				case bC|DATATYPE_INT<<2|DATATYPE_DOUBLE<<1:                \
					instrgen(int, double, 0, rev, _OP_);                   \
				case bC|DATATYPE_DOUBLE<<2|DATATYPE_INT<<1:                \
					instrgen(double, int, 0, rev, _OP_);                   \
				case bC|DATATYPE_DOUBLE<<2|DATATYPE_DOUBLE<<1:             \
					instrgen(double, double, 0, rev, _OP_);                \
				case bC|DATATYPE_DOUBLE<<2|IS_CONSTANT:                    \
					instrgen(double, double, 1, rev, _OP_);                \
			}
			// pred delenim musime zkontrolovat, ze nedelime nulou (neni nutno, pokud delime konstantnou)
			#define checkzero(type1, type2, const, rev) {                  \
				if(!const && !rev)                                         \
					if(!usevariable(IR.op2.index_size, type2))             \
						errored(E_RUN_ZERO_DIV);                           \
				if(!const && rev)                                          \
					if(!usevariable(IR.op1.index_size, type1))             \
						errored(E_RUN_ZERO_DIV);                           \
				if(const && rev)                                           \
					if(!usevariable(IR.op1.index_size, type1))             \
						errored(E_RUN_ZERO_DIV);                           \
			}
			// kvuli ochrane proti deleni nulou ma deleni vlastni generator instrukci
			#define instrsdiv(bC, rev) {                                   \
				case bC|DATATYPE_INT<<2|DATATYPE_INT<<1:                   \
					checkzero(int, int, 0, rev);                           \
					instrgen(int, int, 0, rev, /);                         \
				case bC|DATATYPE_INT<<2|IS_CONSTANT:                       \
					checkzero(int, int, 1, rev);                           \
					instrgen(int, int, 1, rev, /);                         \
				case bC|DATATYPE_INT<<2|DATATYPE_DOUBLE<<1:                \
					checkzero(int, double, 0, rev);                        \
					instrgen(int, double, 0, rev, /);                      \
				case bC|DATATYPE_DOUBLE<<2|DATATYPE_INT<<1:                \
					checkzero(double, int, 0, rev);                        \
					instrgen(double, int, 0, rev, /);                      \
				case bC|DATATYPE_DOUBLE<<2|DATATYPE_DOUBLE<<1:             \
					checkzero(double, double, 0, rev);                     \
					instrgen(double, double, 0, rev, /);                   \
				case bC|DATATYPE_DOUBLE<<2|IS_CONSTANT:                    \
					checkzero(double, double, 1, rev);                     \
					instrgen(double, double, 1, rev, /);                   \
			}
			// Nyni utvorime vsechny instrukce pracujici s cisly pomoci maker:
			instrsall(BC_ADD , 0, +); // vsechny smysluplne varianty instrukce scitani
			instrsall(BC_MUL , 0, *); // a nasobeni
			instrsall(BC_SUB , 0, -); // a odcitani (op1 - op2)
			instrsall(BC_SUBR, 1, -); // a reverzniho odcitani (op2 - op1)
			instrsdiv(BC_DIV,  0);    // a deleni
			instrsdiv(BC_DIVR, 1);    // a reverzniho deleni
			
			#undef int_int_RES
			#undef int_double_RES
			#undef double_int_RES
			#undef double_double_RES
			#undef instrsall
			// ------------------------porovnavaci instrukce------------------------
			// vysledek porovnavacih instrukci je vzdy 1, nebo 0 (tedy int)
			#define int_int_RES int
			#define int_double_RES int
			#define double_int_RES int
			#define double_double_RES int
			// zvlastni generator pro porovnavaci instrukce nad stringy
			#define instrgenstr(const, _OP_) {                                      \
				string op1; string op2;                                             \
				if(!const) {                                                        \
					isinit(IR.op2.index_size);                                      \
					op2 = usevariable(IR.op2.index_size, string);                   \
				} else                                                              \
					op2 = (string) IR.op2.ptr;                                      \
				op1 = usevariable(IR.op1.index_size, string);                       \
				int cmpres                          = memcmp(&op1->str, &op2->str,  \
						op2->len > op1->len ? op2->len : op1->len                   \
					);                                                              \
				/* Pokud vraci 0, musime jeste zkontrolovat delky! */               \
				usevariable(IR.op1.index_size, int) =                               \
					cmpres ? cmpres _OP_ 0 : op1->len _OP_ op2->len;                \
				break;                                                              \
			}
			// seznam vsech moznych i nemoznych porovnavani; kecam, jenom moznych :-)
			#define instrsall(bC, _OP_) {                                  \
				case bC|DATATYPE_INT<<2|DATATYPE_INT:                      \
					instrgen(int, int, 0, 0, _OP_);                        \
				case bC|DATATYPE_INT<<2|IS_SAMETYPE:                       \
					instrgen(int, int, 1, 0, _OP_);                        \
				case bC|DATATYPE_INT<<2|DATATYPE_DOUBLE:                   \
					instrgen(int, double, 0, 0, _OP_);                     \
				case bC|DATATYPE_DOUBLE<<2|DATATYPE_INT:                   \
					instrgen(double, int, 0, 0, _OP_);                     \
				case bC|DATATYPE_DOUBLE<<2|DATATYPE_DOUBLE:                \
					instrgen(double, double, 0, 0, _OP_);                  \
				case bC|DATATYPE_DOUBLE<<2|IS_SAMETYPE:                    \
					instrgen(double, double, 1, 0, _OP_);                  \
				case bC|DATATYPE_STRING<<2|DATATYPE_STRING:                \
					instrgenstr(0, _OP_);                                  \
				case bC|DATATYPE_STRING<<2|IS_SAMETYPE:                    \
					instrgenstr(1, _OP_);                                  \
			}
			// Nyni utvorime vsechny instrukce pro porovnani pomoci maker:
			instrsall(BC_CMPEQ, ==); // ekvivalence
			instrsall(BC_CMPNE, !=); // non-ekvivalence
			instrsall(BC_CMPL , < ); // mensi nez
			instrsall(BC_CMPG , > ); // vetsi nez
			instrsall(BC_CMPLE, <=); // mensi nebo rovno
			instrsall(BC_CMPGE, >=); // vetsi nebo rovno
			
			#undef instrsall
			// -------------------------logicke instrukce------------------------
			// seznam vsech moznych variant logickeho soucinu a nasobeni :-)
			#define instrsall(bC, _OP_) {                                  \
				case bC|DATATYPE_INT<<1|DATATYPE_INT:                      \
					instrgen(int, int, 0, 0, _OP_);                        \
				case bC|DATATYPE_INT<<1|DATATYPE_DOUBLE:                   \
					instrgen(int, double, 0, 0, _OP_);                     \
				case bC|DATATYPE_DOUBLE<<1|DATATYPE_INT:                   \
					instrgen(double, int, 0, 0, _OP_);                     \
				case bC|DATATYPE_DOUBLE<<1|DATATYPE_DOUBLE:                \
					instrgen(double, double, 0, 0, _OP_);                  \
			}
			// Nyni utvorime logicke instrukce:
			instrsall(BC_AND, &&); // logicky soucin
			instrsall(BC_OR , ||); // logicky soucet
			// Negaci operandu zvlast:
			case BC_NEG|DATATYPE_INT:
				usevariable(IR.op1.index_size, int) = !usevariable(IR.op2.index_size, int);
				inited(IR.op1.index_size);
				break;
			case BC_NEG|DATATYPE_DOUBLE:
				usevariable(IR.op1.index_size, int) = !usevariable(IR.op2.index_size, double);
				inited(IR.op1.index_size);
				break;
			
			// Vysledek maker by mel vypadat asi takto (po odstraneni mrtveho kodu; zde priklad pro scitani):
			/*case BC_ADD|DATATYPE_INT<<2|DATATYPE_INT<<1:
				isinit(IR.op2.index_size);
				usevariable(IR.op1.index_size, int) = usevariable(IR.op1.index_size, int) + usevariable(IR.op2.index_size, int);
				break;
			case BC_ADD|DATATYPE_INT<<2|DATATYPE_INT<<1|IS_CONSTANT:
				usevariable(IR.op1.index_size, int) = usevariable(IR.op1.index_size, int) + IR.op2.integer;
				break;
			// vysledek typu double
			case BC_ADD|DATATYPE_INT<<2|DATATYPE_DOUBLE<<1:
				isinit(IR.op2.index_size);
				usevariable(IR.op1.index_size, double) = usevariable(IR.op1.index_size, int) + usevariable(IR.op2.index_size, double);
				break;
			case BC_ADD|DATATYPE_DOUBLE<<2|DATATYPE_INT<<1:
				isinit(IR.op2.index_size);
				usevariable(IR.op1.index_size, double) = usevariable(IR.op1.index_size, double) + usevariable(IR.op2.index_size, int);
				break;
			case BC_ADD|DATATYPE_DOUBLE<<2|DATATYPE_DOUBLE<<1:
				isinit(IR.op2.index_size);
				usevariable(IR.op1.index_size, double) = usevariable(IR.op1.index_size, double) + usevariable(IR.op2.index_size, double);
				break;
			case BC_ADD|DATATYPE_DOUBLE<<2|DATATYPE_DOUBLE<<1|IS_CONSTANT:
				usevariable(IR.op1.index_size, double) = usevariable(IR.op1.index_size, double) + IR.op2.real;
				break;*/
			
			case BC_ERROR:
				errored(IR.op1.integer);
			
			default: // neznama instrukce
				errored(E_RUN_OTH);
		}
		
		PC++; // posun na dalsi instrukci
	}
}
