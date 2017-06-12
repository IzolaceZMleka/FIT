#include "../main-files/lexer.h"
#include <stdio.h>
#include <string.h>

extern FILE *sourceCodeFile;

int main() {
	
	/* Source Code (ifj15):
	int f(int param1){}
	int f2(int param1, double param2);
	int main(){
		auto s = "text";
		int i = 0;
		double f;
		if (i>0)
			{return 1;}
		else
			{cin >> s;}
		for(int i = 1; i > 0; i=i-1)
			{cout << s;}
	}
	*/
	static LexemType expected[] = {LEX_INT, LEX_ID, LEX_L_PARENTHESIS, LEX_INT, LEX_ID, LEX_R_PARENTHESIS, LEX_L_CURLY_BRACKETS, LEX_R_CURLY_BRACKETS,
									LEX_INT, LEX_ID, LEX_L_PARENTHESIS, LEX_INT, LEX_ID, LEX_COMMA, LEX_DOUBLE, LEX_ID, LEX_R_PARENTHESIS, LEX_SEMICOLON,
									LEX_INT, LEX_ID, LEX_L_PARENTHESIS, LEX_R_PARENTHESIS, LEX_L_CURLY_BRACKETS,
									LEX_AUTO, LEX_ID, LEX_BO_ASSIGNMENT, LEX_LIT_STRING, LEX_SEMICOLON,
									LEX_INT, LEX_ID, LEX_BO_ASSIGNMENT, LEX_LIT_INT, LEX_SEMICOLON,
									LEX_DOUBLE, LEX_ID, LEX_SEMICOLON,
									LEX_IF, LEX_L_PARENTHESIS, LEX_ID, LEX_BO_GREATER, LEX_LIT_INT, LEX_R_PARENTHESIS,
									LEX_L_CURLY_BRACKETS, LEX_RETURN, LEX_LIT_INT, LEX_SEMICOLON, LEX_R_CURLY_BRACKETS,
									LEX_ELSE,
									LEX_L_CURLY_BRACKETS, LEX_CIN ,LEX_IN, LEX_ID, LEX_SEMICOLON,LEX_R_CURLY_BRACKETS,
									LEX_FOR, LEX_L_PARENTHESIS, LEX_INT, LEX_ID, LEX_BO_ASSIGNMENT, LEX_LIT_INT, LEX_SEMICOLON, LEX_ID, LEX_BO_GREATER, LEX_LIT_INT,
										LEX_SEMICOLON, LEX_ID, LEX_BO_ASSIGNMENT, LEX_ID, LEX_BO_MINUS, LEX_LIT_INT, LEX_R_PARENTHESIS,
									LEX_L_CURLY_BRACKETS, LEX_COUT, LEX_OUT, LEX_ID, LEX_SEMICOLON, LEX_R_CURLY_BRACKETS,
									LEX_R_CURLY_BRACKETS, LEX_EOF};
	LexemType lexem;
	
	int integers [5] = {99,101,103,48195995,123456789};
	double reals [4] = {4e13,6.484849,0.13e-3,12.4e3};
	const char *strings[4] =
	{
		"Escape pro konstantu nenasledovany znakem.", "String neukonceny |\"| - \\n", "String neukonceny |\"| - EOF"
	};
	
	const char *types[44] =
	{	
	"LEX_INT    = DATATYPE_INT",    // integer
	"LEX_DOUBLE = DATATYPE_DOUBLE", // realne cislo
	"LEX_STRING = DATATYPE_STRING", // string
	"LEX_BOOL   = DATATYPE_BOOL",   // bool
/* Klicova slova */
	"LEX_CIN",                // cin 0
	"LEX_COUT",               // cout 
	"LEX_AUTO",               // auto
	"LEX_IF",                 // if
	"LEX_ELSE",               // else
	"LEX_WHILE",              // while
	"LEX_DO",                 // do
	"LEX_FOR",                // for
	"LEX_RETURN",             // return
/* Literaly */
	"LEX_BOOL_F",             // false
	"LEX_BOOL_T",             // true
	"LEX_LIT_INT",            // literal pro integer
	"LEX_LIT_REAL",           // literal pro realne cislo
	"LEX_LIT_STRING",         // literal pro string 19
/*  */
	"LEX_ID",                 // identifikator 20
	"LEX_ERROR_LEX", 
	"LEX_ERROR_IN",
	"LEX_EOF",                // konec souboru
	"LEX_SEMICOLON",          // ;
	"LEX_COMMA", 
	"LEX_L_CURLY_BRACKETS",   // {
	"LEX_R_CURLY_BRACKETS",   // }
	"LEX_IN",                 // >>
	"LEX_OUT",                // << 30
/* Prirazeni */
	"LEX_BO_ASSIGNMENT",      // =
/* Unarni operatory */
	"LEX_UO_NEG",             // !
/* Binarni operatory */
	"LEX_BO_PLUS",            // +
	"LEX_BO_MINUS",           // -
	"LEX_BO_MULTIPLY",        // *
	"LEX_BO_DIVIDE",          // /
	"LEX_BO_EQUAL",           // ==
	"LEX_BO_NOT_EQUAL",       // !=
	"LEX_BO_LESS",            // <
	"LEX_BO_LESS_EQUAL",      // <=
	"LEX_BO_GREATER",         // >
	"LEX_BO_GREATER_EQUAL",   // >=
	"LEX_BO_AND",             // &&
	"LEX_BO_OR",              // ||
	"LEX_L_PARENTHESIS",      // (
	"LEX_R_PARENTHESIS",      // )
	};
	
	
	
	sourceCodeFile = fopen("test.ifj15", "r");
	int i = 0;
	int error = 0;
	
	lexerInit();
	
	
	while (i < 78) {
		lexem = lexerGetLexem();
		if(lexem != expected[i])
			printf("%d: Got from lexerGetLexem(): %d ; expected %d ; ID: %s ; lit: %s\n",i, lexem, expected[i], identifier, string->str);
		i++;
	}
	
	printf("\n\nTEST ID:\n\n");
	
	lexem = lexerGetLexem();
	if((lexem != LEX_ID) || strcmp(identifier,"_jablicko"))
		error++;
	printf("ID: \"_jablicko\" , got from lexer: \"%s\"\n",identifier);
	
	lexem = lexerGetLexem();
	if((lexem != LEX_ID) || strcmp(identifier,"mrkvicka"))
		error++;
	printf("ID: \"mrkvicka\" , got from lexer: \"%s\"\n",identifier);
	
	lexem = lexerGetLexem();
	if((lexem != LEX_ID) || strcmp(identifier,"pejsekako4i4ka"))
		error++;
	printf("ID: \"pejsekako4i4ka\" , got from lexer: \"%s\"\n",identifier);
		
	if(error ==0)
		printf("\nTEST ID: OK\n");
	else
		printf("Errory: %d\n",error);
	

	
	printf("\n\nTEST ESCAPE:\n\na) literal\n");
	for(int i = 0; i<7; i++){
		lexem = lexerGetLexem();
		printf("%s\n",string->str);
	}
	
	printf("\nb) konstanta\n");
	for(int i = 0; i<3; i++){
		lexem = lexerGetLexem();
		if((lexem != LEX_LIT_INT) || (integer != integers[i]))
			error++;
		printf("%d == %d\n",integers[i],integer);
	}
	
	if(error ==0)
		printf("\nTEST ESCAPE: OK\n");
	else
		printf("Errory: %d\n",error);
	
	
	printf("\n\nTEST REAL|INT:\n\na) real\n");
	for(int i = 0; i<4; i++){
		lexem = lexerGetLexem();
		if((lexem != LEX_LIT_REAL) || (real != reals[i]))
			error++;
		printf("%f == %f\n",reals[i],real);
	}
	
	printf("\nb) int\n");
	for(int i = 3; i<5; i++){
		lexem = lexerGetLexem();
		if((lexem != LEX_LIT_INT) || (integer != integers[i]))
			error++;
		printf("%d == %d\n",integers[i],integer);
	}
	
	if(error ==0)
		printf("\nTEST REAL|INT: OK\n");
	else
		printf("Errory: %d\n",error);
	
	printf("\nPocet erroru %d!\n\n",error);
	
	int k = 0;
	
	printf("Nelegalni zapisy\n\n");
	
	while((lexem = lexerGetLexem()) != LEX_EOF){
		
		if(lexem == LEX_ERROR_LEX)
		{
			printf("____%d____\n",lexem);
			printf("ERROR: %s\n",strings[k]);
			error++;
			k++;
		}
		if(error == 3)
			printf("\n\nERRORY ZACHYCENY\n\n");
		
		
	}
	
	lexerClean();
	fclose(sourceCodeFile);
	
	
	/*
	lexerInit();
	sourceCodeFile = fopen("x", "r");
	
	
	while((lexem = lexerGetLexem()) != LEX_EOF)
	{
		
		printf("%s\t|%d|\t\n",types[lexem],integer);
	}
	
	lexerClean();
	fclose(sourceCodeFile);
	*/
	return 0;
}