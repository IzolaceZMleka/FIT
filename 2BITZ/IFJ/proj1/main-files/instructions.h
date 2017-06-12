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
#ifndef INSTRUCTIONS_H_INCLUDED
#define INSTRUCTIONS_H_INCLUDED

#include <stddef.h>
#include <stdint.h>

// DEKLARACE DATOVYCH TYPU:
enum DataTypes {
	NO_DATATYPE = -1,
	DATATYPE_INT,
	DATATYPE_DOUBLE,
	DATATYPE_STRING,
	DATATYPE_BOOL
};

#define DOUBLEWORD int32_t // promenna velikosti dvojslova
#define DOUBLEWORD_S sizeof(DOUBLEWORD) // velikost dvouslovech (= 4 B)
// makro, ktere zjisti velikost interpretovaneho datove typu ve dvouslovech
#define datatypesize(type) \
	((type == DATATYPE_INT || type == DATATYPE_BOOL ? sizeof(int) : type == DATATYPE_DOUBLE ? sizeof(double) : type == DATATYPE_STRING ? sizeof(void *) : 0) / DOUBLEWORD_S)



enum ByteCode {
	BC_CJMP,        // CJMP INDEX, PTR            Conditional JuMP
	BC_JMP = 2,     // JMP PTR                    unconditional JuMP
	BC_CALL,        // CALL PTR, INDEX            user function CALL
	BC_CALLBIN,     // CALLBIN INDEX, INDEX       Built-IN function CALL
	BC_INIT,        // INIT SIZE                  INITiate called function
	BC_EOP,         // EOP                        End Of Program
	BC_ERROR,       // ERROR VALUE                Program in ERROR state
	BC_RETURN = 16, // RETURN , INDEX/VALUE       RETURN from function
	BC_READ = 32,   // READ INDEX                 READ from stdin
	BC_WRITE = 40,  // WRITE INDEX/VALUE          WRITE to stdout
	BC_LOAD = 48,   // LOAD INDEX, INDEX/VALUE    LOAD variable/value into variable
	// ALR instrukce                              
	BC_ADD = 64,    // ADD INDEX, INDEX/VALUE     perform ADDition between two operands
	BC_MUL = 72,    // MUL INDEX, INDEX/VALUE     perform MULtiplication between two operands
	BC_SUB = 80,    // SUB INDEX, INDEX/VALUE     perform SUBtraction between two operands
	BC_DIV = 96,    // DIV INDEX, INDEX/VALUE     perform DIVision between two operands
	BC_CMPEQ = 112, // CMPEQ INDEX, INDEX/VALUE   CoMPare two operands on EQuality
	BC_CMPNE = 128, // CMPNE INDEX, INDEX/VALUE   CoMPare two operands on iNEquality
	BC_CMPL = 144,  // CMPL  INDEX, INDEX/VALUE   CoMPare two operands on Less
	BC_CMPG = 160,  // CMPG  INDEX, INDEX/VALUE   CoMPare two operands on Greater
	BC_CMPLE = 176, // CMPLE INDEX, INDEX/VALUE   CoMPare two operands on Less or Equal
	BC_CMPGE = 192, // CMPGE INDEX, INDEX/VALUE   CoMPare two operands on Greater or Equal
	BC_AND = 208,   // AND INDEX, INDEX           perform logic AND between two operands
	BC_OR = 212,    // OR  INDEX, INDEX           perform logic OR between two operands
	BC_NEG = 216,   // NEG INDEX, INDEX           NEGate operand
};

// toto jsou pouze pojmenovani pro reverzni varianty odcitani a deleni
#define BC_SUBR ((BC_DIV+BC_SUB) / 2)
#define BC_DIVR ((BC_CMPEQ+BC_DIV) / 2)

enum BIN_func {
	BIN_LEN,
	BIN_SUBSTR,
	BIN_CONCAT,
	BIN_FIND,
	BIN_SORT
};

typedef union {
	size_t index_size; // index into local frame, or size
	void *        ptr; // pointer to memory
	int       integer; // integer constant
} Toperand1;

typedef union {
	size_t index_size; // index into local frame, or size
	void *        ptr; // pointer to memory
	int       integer; // integer constant
	double       real; // double constant
} Toperand;

typedef struct {
	char      code; // bytecode of the instruction
	Toperand1  op1; // optional first and second operand
	Toperand   op2; // instruction can have only one double constant as operand
} Tinstruction;

extern struct byteProgram {
	Tinstruction * nextInstr;    // pointer to free room for next instruction
	Tinstruction * curBlockEnd;  // pointer to last instruction in current block 
	Tinstruction * startBlock;   // prvni blok instrukci - pro dealokaci po skonceni programu
} byteProgram;

// vrati ukazatel na prvni volnou instrukci v kodu (pro ucely skoku a zacatku funkci), vraci NULL pri chybe
Tinstruction* nextInstr();

// generator instrukci bajtkodu, vraci NULL pri chybe
Tinstruction* generateInstr(char byteCode, Toperand1 op1, Toperand op2);

#define INSTR_CONST 1
#define INSTR_RETVAL 2
#define INSTR_PARAM 4  
#define INSTR_MOVE (INSTR_PARAM|INSTR_RETVAL)
// generator komplexnich instrukci
Tinstruction* generateCInstr(unsigned char byteCode, int flags, Toperand1 op1, char type1, Toperand op2, char type2);

// Zajistuje uklizeni programu pri ukonceni.
void byteProgramDispose();

#endif // INSTRUCTIONS_H_INCLUDED
