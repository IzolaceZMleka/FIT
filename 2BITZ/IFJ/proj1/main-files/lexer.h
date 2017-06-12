/* ***************************************************************** */
/*
	projekt: Implementace interpretu imperativniho jazyka IFJ15
	autori: Jan Pavlica, Jan Velecky
	xvelec07: Jan Velecky
	xzahra24: Tomas Zahradnicek
	xpiece00: Adam Piecek
	xpavli78: Jan Pavlica
*/
/* ***************************************************************** */
#include "builtin_func.h"
#include "instructions.h"
#include <stdio.h>

// GLOBALNI PROMENNE pro posledni nactene bohate lexemy daneho typu

extern int integer;       // cele cislo
extern double real;       // desetinne cislo
extern char * identifier; // identifikator
extern Tstring * string;  // retezec
extern FILE *sourceCodeFile; // 	vstupni soubor

// vraci typ nacteneho lexemu
// konkretni hodnotu bohateho lexemu vraci v globalnich promennych vyse, je-li treba
int lexerGetLexem();
int lexerInit();
void lexerClean();

typedef enum LexemType { 
/* Datove typy */
	LEX_INT    = DATATYPE_INT,    // integer
	LEX_DOUBLE = DATATYPE_DOUBLE, // realne cislo
	LEX_STRING = DATATYPE_STRING, // string
	LEX_BOOL   = DATATYPE_BOOL,   // bool
/* Klicova slova */
	LEX_CIN,                // cin 0
	LEX_COUT,               // cout 
	LEX_AUTO,               // auto
	LEX_IF,                 // if
	LEX_ELSE,               // else
	LEX_WHILE,              // while
	LEX_DO,                 // do
	LEX_FOR,                // for
	LEX_RETURN,             // return
/* Literaly */
	LEX_BOOL_F,             // false
	LEX_BOOL_T,             // true
	LEX_LIT_INT,            // literal pro integer
	LEX_LIT_REAL,           // literal pro realne cislo
	LEX_LIT_STRING,         // literal pro string 19
/*  */
	LEX_ID,                 // identifikator 20
	LEX_ERROR_LEX, 
	LEX_ERROR_IN,
	LEX_EOF,                // konec souboru
	LEX_SEMICOLON,          // ;
	LEX_COMMA,              // , 24
	LEX_L_CURLY_BRACKETS,   // {
	LEX_R_CURLY_BRACKETS,   // }
	LEX_IN,                 // >>
	LEX_OUT,                // << 30
/* Prirazeni */
	LEX_BO_ASSIGNMENT,      // =
/* Unarni operatory */
	LEX_UO_NEG,             // !
/* Binarni operatory */
	LEX_BO_PLUS,            // +
	LEX_BO_MINUS,           // -
	LEX_BO_MULTIPLY,        // *
	LEX_BO_DIVIDE,          // /
	LEX_BO_EQUAL,           // ==
	LEX_BO_NOT_EQUAL,       // !=
	LEX_BO_LESS,            // <
	LEX_BO_LESS_EQUAL,      // <=
	LEX_BO_GREATER,         // >
	LEX_BO_GREATER_EQUAL,   // >=
	LEX_BO_AND,             // &&
	LEX_BO_OR,              // ||
	LEX_L_PARENTHESIS,      // (
	LEX_R_PARENTHESIS,      // )
} LexemType;

