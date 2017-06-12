/* ***************************************************************** */
/*
	projekt: Implementace interpretu imperativniho jazyka IFJ15
	autori: Jan Pavlica
	xvelec07: Jan Velecky
	xzahra24: Tomas Zahradnicek
	xpiece00: Adam Piecek
	xpavli78: Jan Pavlica
*/
/* ***************************************************************** */
#include "lexer.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <ctype.h>

int integer;       // cele cislo
double real;       // desetinne cislo
char * identifier; // identifikator, treba naalokovat pred volanim
Tstring * string;  // retezec, treba naalokovat pred volanim
FILE *sourceCodeFile;
static Tstring **loadedStrings;
static char * bufferino = NULL;
static Tstring * help_string = NULL;
static int TstringsC = 0;

char * makeBuffer (int size);
int loadedStringsRealloc (int size);
int strToInt (int state, int base);

typedef enum State {
	S_START,                // pocatecni stav
	S_INT,                  // nacitani cislice
	S_LITER,
	S_ID,
	S_COMMENT_1,              // nacitani komentare
	S_COMMENT_2,
	S_ESC,                  // escape sekvence
	S_ESC_CON,              // escape sekvence pro konstantu
	S_REAL,                 // nacitani realneho cisla
	S_EXP,                  // nacteno e
	S_PM,                   // nacten -+
	S_PM_N,
	S_REAL_DOT,
	S_EXP_N
} State;

#define BUFFER_SIZE_S 50
#define BUFFER_SIZE 10
#define TSTRINGS_SIZE 20
#define BASE_2 2
#define BASE_8 8
#define BASE_10 10
#define BASE_16 16
#define ESC_NUMBER_2 8
#define ESC_NUMBER_8 3
#define ESC_NUMBER_16 2
#define WRONG_ESCAPE -1

#define EXPANDSTRING(buffer, i, znak, bufferSize){ \
	if (bufferSize == i){ \
		bufferSize = bufferSize * 2;\
		buffer = (char *) realloc(buffer,sizeof(char) * bufferSize + 1);\
		if (buffer == NULL){\
			free(buffer);\
			return LEX_ERROR_IN;}\
	}\
	\
	buffer[i] = znak;\
	i++;\
}
	
#define EXPANDSTRING_LIT(stringous, i, znak, bufferSize){ \
	if (bufferSize == i){ \
		bufferSize = bufferSize * 2;\
		stringous = (Tstring *) realloc(stringous,sizeof(Tstring) + bufferSize + 1);\
		if (stringous == NULL){\
			free(stringous);\
			return LEX_ERROR_IN;}\
	}\
			\
	stringous->str[i] = znak;\
	i++;\
}

/* funkce pro vytvoreni bufferu */
char * makeBuffer (int size){
	char * buffer = (char *) malloc(sizeof(char) * size + 1);
	if (buffer != NULL)
		return buffer;
	else
		return NULL;
}

/* funkce pro inicializaci lexikalniho analyzatoru */
int lexerInit(){
	loadedStrings = malloc(sizeof(Tstring *) * TSTRINGS_SIZE);
	
	if(loadedStrings == NULL)
		return 1;
	else
		return 0;
}

/* realokace stringu */
int loadedStringsRealloc (int size){
	
	size = size * 2;
	loadedStrings = realloc(loadedStrings,sizeof(Tstring *) * size);
	if(loadedStrings == NULL)
		return 0;
	else
		return size;
}

/* funkce pro uklizeni lexikalniho analyzatoru */
void lexerClean(){
	
	int i = 0;
	
	if(help_string != NULL)
		free(help_string);
	
	free(bufferino);
	
	while (i<TstringsC){
		free(loadedStrings[i]);
		i++;
	}
	
	free(loadedStrings);
}


/* prevod stringu pri esc na integer */
int strToInt (int state, int base)
{
	int c; // promenna pro nacteni znaku
	int k; // promenna pro ulozeni soustavy
	int i = 0; // pocitadlo znaku
	char buffer[30];
	
	if(state == S_ESC){
		if(base == BASE_2) // prirazeni soustavy
			k = ESC_NUMBER_2;
		else if(base == BASE_8)
			k = ESC_NUMBER_8;
		else if(base == BASE_16)
			k = ESC_NUMBER_16;

		for(; i<k; i++){ // nacitani escape sekvence v retezci
			c = getc(sourceCodeFile);
			if((isxdigit(c) && (base == BASE_16)) || ((base == BASE_8) && (c <= '7' && c >= '0')) || ((base == BASE_2) && (c == '1' || c == '0'))){
				buffer[i]=c;
			}
			else{
				return 0;	
			}
		}
	}
	else{
		while((c = getc(sourceCodeFile)) != EOF){ // nacitani escape sekvence
			
			if((isxdigit(c) && (base == BASE_16)) || ((base == BASE_8) && (c <= '7' && c >= '0')) || ((base == BASE_2) && (c == '1' || c == '0'))){
				buffer[i]=c;
				i++;
			}
			else{
				if(i == 0)
					return (-1);
				ungetc(c, sourceCodeFile);
				break;
			}
		}
	}
	buffer[i] = '\0';
	c = strtol(buffer,NULL,base);
	return c;
}

int lexerGetLexem() {
	
	int c; // promenna pro aktualni znak
	int cNext; // promenna pro dalsi pro zjisteni lexemu
	int state = S_START; // promenna pro stav
	int i = 0; // pocitadlo nactenych znaku

	static int bufferSize = BUFFER_SIZE_S; // velikost bufferu pro string
	static int bufferSizeID = BUFFER_SIZE; // velikost bufferu pro ID
	static int Tstrings_size = TSTRINGS_SIZE; // velikost 
	
	while((c = getc(sourceCodeFile))){
		switch(state){
			case S_START :
				switch(c){
					case EOF : // v pripade eof vraci eof
						return LEX_EOF;
					case ' ' : // preskocime bile znaky
					case '\n':
					case '\t':
					case '\r':
					case '\f':
						break;
					case ';' :
						return LEX_SEMICOLON;
					case ',' :
						return LEX_COMMA;	
					case '(' :
						return LEX_L_PARENTHESIS;
					case ')' :
						return LEX_R_PARENTHESIS;
					case '{' :
						return LEX_L_CURLY_BRACKETS;
					case '}' :
						return LEX_R_CURLY_BRACKETS;
					case '"' :
						if(help_string == NULL){
							bufferSize = BUFFER_SIZE_S;
							help_string = allocString(bufferSize);
								if(help_string == NULL)
									return LEX_ERROR_IN;
						}
						state = S_LITER;
						break;
					case '*' :
						return LEX_BO_MULTIPLY;
					case '/' : // zjistime zda jde o deleni nebo o komentar
						cNext = getc(sourceCodeFile);
						if (cNext == '/') {
							state = S_COMMENT_1;
							break;
						}
						else if (cNext == '*'){
							state = S_COMMENT_2;
							break;
						}
						else{ // v pripade deleni vratime nacteny znak
							ungetc(cNext, sourceCodeFile);
							return LEX_BO_DIVIDE;
						}
						break;
					case '<' :
						cNext = getc(sourceCodeFile);
						if (cNext == '<')
							return LEX_OUT;
						else if(cNext == '=')
							return LEX_BO_LESS_EQUAL;
						else{
							ungetc(cNext, sourceCodeFile);
							return LEX_BO_LESS;
						}
					case '>' :
						cNext = getc(sourceCodeFile);
						if (cNext == '>')
							return LEX_IN;
						else if(cNext == '=')
							return LEX_BO_GREATER_EQUAL;
						else{
							ungetc(cNext, sourceCodeFile);
							return LEX_BO_GREATER;
						}
					case '-' :
						return LEX_BO_MINUS;
					case '+' :
						return LEX_BO_PLUS;
		
					case '!' :
						cNext = getc(sourceCodeFile);
						if (cNext == '=')
							return LEX_BO_NOT_EQUAL;
						else{
							ungetc(cNext, sourceCodeFile);
							return LEX_UO_NEG;
						}
					case '=' :
						cNext = getc(sourceCodeFile);
						if (cNext == '=')
							return LEX_BO_EQUAL;
						else{
							ungetc(cNext, sourceCodeFile);
							return LEX_BO_ASSIGNMENT;
						}
					case '&' :
						cNext = getc(sourceCodeFile);
						if (cNext == '&')
							return LEX_BO_AND;
						else{
							ungetc(cNext, sourceCodeFile);
							return LEX_ERROR_LEX;
						}
					case '\\' :
						state = S_ESC_CON;
						break;
					case '|' :
						cNext = getc(sourceCodeFile);
						if (cNext == '|')
							return LEX_BO_OR;
						else{
							ungetc(cNext, sourceCodeFile);
							return LEX_ERROR_LEX;
						}
					default :
						if (isdigit(c)){ // konstanta
							if(help_string == NULL){
								bufferSize = BUFFER_SIZE_S;
								help_string = allocString(bufferSize);
								if(help_string == NULL)
									return LEX_ERROR_IN;
							}
							EXPANDSTRING_LIT(help_string, i, c, bufferSize);
							state = S_INT;
							break;
						}
						else if((isalpha(c)) || c=='_'){ // id
							if(bufferino == NULL){
								bufferSizeID = BUFFER_SIZE;
								bufferino = makeBuffer(bufferSizeID);
								if(bufferino == NULL)
									return LEX_ERROR_IN;
							}
							EXPANDSTRING(bufferino, i, c, bufferSizeID);
							state = S_ID;
							break;
						}
						else{
							return LEX_ERROR_LEX;
						}
					}
				break;
			case S_LITER :
				switch(c){
					case '\\' :
						state = S_ESC;
						break;
					case '"' : // konec stringu
						help_string->str[i] = '\0'; // ukoncim string
						help_string->len = i; // ulozim jeho delku
						
						if(TstringsC == Tstrings_size - 1) // kontrola na dostatecnou velikost
							Tstrings_size = loadedStringsRealloc(Tstrings_size);
						
						loadedStrings[TstringsC] = help_string; // ulozeni ukazatele
						string = help_string; // predani v globalni promenne
						help_string = NULL; // vyNULLovani pro dalsi alokaci
						TstringsC++;
						return LEX_LIT_STRING;
					default :
						if(c >= 31 && c != '"'){
							EXPANDSTRING_LIT(help_string, i, c, bufferSize);
							break;
						}
						else
							return LEX_ERROR_LEX;
					}
					break;
			case S_ESC : // escape sekvence
				switch(c){
					case '\\' :
						EXPANDSTRING_LIT(help_string, i, '\\', bufferSize);
						state = S_LITER;
						break;
					case 'n' :
						EXPANDSTRING_LIT(help_string, i, '\n', bufferSize);
						state = S_LITER;
						break;
					case 't' :
						EXPANDSTRING_LIT(help_string, i, '\t', bufferSize);
						state = S_LITER;
						break;
					case '"' :
						EXPANDSTRING_LIT(help_string, i, '"', bufferSize);
						state = S_LITER;
						break;
					case 'x' :
						if((c = strToInt(state,BASE_16)) != 0){
							EXPANDSTRING_LIT(help_string, i, c, bufferSize);
						}
						else{
							return LEX_ERROR_LEX;
						}
						state = S_LITER;
						break;
					case 'b' :
						if((c = strToInt(state,BASE_2)) != 0){
							EXPANDSTRING_LIT(help_string, i, c, bufferSize);
						}
						else{
							return LEX_ERROR_LEX;
						}
						state = S_LITER;
						break;
					case '0' :
						if((c = strToInt(state,BASE_8)) != 0){
							EXPANDSTRING_LIT(help_string, i, c, bufferSize);
						}
						else{
							return LEX_ERROR_LEX;
						}
						state = S_LITER;
						break;
					default :
						return LEX_ERROR_LEX;
						
				}break;
			case S_COMMENT_1 : // preskakujeme komentar //
				while(c != '\n' && c != EOF)
					c = getc(sourceCodeFile);
				
				if (c == '\n'){
					state = S_START;
					break;
				}
				else if(c == EOF){
					ungetc(c, sourceCodeFile);
					state = S_START;
					break;
				}
				break;
			case S_COMMENT_2 : // preskakujeme komentar /*
				while(c != '*' && c != EOF)
					c = getc(sourceCodeFile);
				
				if(c == '*'){
					cNext = getc(sourceCodeFile);
					if(cNext == '/'){
						state = S_START;
						break;
					}
					else{
						ungetc(cNext, sourceCodeFile);
						break;
					}
				}
				else if(c == EOF){
					return LEX_ERROR_LEX;	
				}
				break;
			case S_ESC_CON : // konstanta zapsana v esc sekvenci
				switch(c){
					case 'x' :
						if((integer = strToInt(state,BASE_16)) != (-1))
							return LEX_LIT_INT;
						else
							return LEX_ERROR_LEX;
					case 'b' :
						if((integer = strToInt(state,BASE_2)) != (-1))
							return LEX_LIT_INT;
						else
							return LEX_ERROR_LEX;
					case '0' :
						if((integer = strToInt(state,BASE_8)) != (-1))
							return LEX_LIT_INT;
						else
							return LEX_ERROR_LEX;;
					default :
						return LEX_ERROR_LEX;
						
				}break;
			case S_INT : // nacitani integeru
				switch(c){
					case 'e' :
					case 'E' :
						EXPANDSTRING_LIT(help_string, i, c, bufferSize);
						state = S_EXP;
						break;
					case '.' :
						EXPANDSTRING_LIT(help_string, i, c, bufferSize);
						state = S_REAL_DOT;
						break;
					default :
						if(isdigit(c)){
							EXPANDSTRING_LIT(help_string, i, c, bufferSize);
							break;
						}
						else{
							ungetc(c, sourceCodeFile);
							help_string->str[i]='\0';
							integer = strtol(help_string->str,NULL,BASE_10);
							return LEX_LIT_INT;
						}
				} break;
			case S_REAL_DOT : // nacitani real
				if(isdigit(c)){
					EXPANDSTRING_LIT(help_string, i, c, bufferSize);
					state = S_REAL;
					break;
				}
				else{
					return LEX_ERROR_LEX;
				}break;
			case S_REAL : // nacitani real
				if(isdigit(c)){
					EXPANDSTRING_LIT(help_string, i, c, bufferSize);
					break;
				}
				else if(c == 'e' || c == 'E'){
					EXPANDSTRING_LIT(help_string, i, c, bufferSize);
					state = S_EXP;
					break;
				}
				else{
					ungetc(c, sourceCodeFile);
					help_string->str[i]='\0';
					real = strtod(help_string->str,NULL);
					return LEX_LIT_REAL;
				} 
				break;
			case S_EXP : // nacteno E
				switch(c){
					case '-' :
					case '+' :
						EXPANDSTRING_LIT(help_string, i, c, bufferSize);
						state = S_PM;
						break;
					default :
						if(isdigit(c)){
							EXPANDSTRING_LIT(help_string, i, c, bufferSize);
							state = S_EXP_N;
							break;
						}
						else{
							return LEX_ERROR_LEX;
						}
				}break;
			case S_EXP_N :
						if(isdigit(c)){
							EXPANDSTRING_LIT(help_string, i, c, bufferSize);
							break;
						}
						else{
							ungetc(c, sourceCodeFile);
							help_string->str[i]='\0';
							real = strtod(help_string->str,NULL);
							return LEX_LIT_REAL;
						}
			case S_PM : // nacteno + nebo -, musi nasledovat cislo
				if(isdigit(c)){
					EXPANDSTRING_LIT(help_string, i, c, bufferSize);
					state = S_PM_N;
					break;
				}
				else{
					return LEX_ERROR_LEX;
				}break;
			case S_PM_N : // nacteno + nebo - a nacteno 1.cislo 
				if(isdigit(c)){
					EXPANDSTRING_LIT(help_string, i, c, bufferSize);
					break;
				}
				else{
					ungetc(c, sourceCodeFile);
					help_string->str[i]='\0';
					real = strtod(help_string->str,NULL);
					return LEX_LIT_REAL;
				}break;
			case S_ID :{
				if(((isalpha(c)) || (c=='_') || (isdigit(c)))){
					EXPANDSTRING(bufferino, i, c, bufferSizeID);
					break;
				}
				else{
					ungetc(c, sourceCodeFile);
					bufferino[i] = '\0';
					switch(bufferino[0]){
						case 'a' :
							if(!(strcmp(bufferino,"auto")))
								return LEX_AUTO;
							else
								break;
						case 'b' :
							if (!(strcmp(bufferino,"bool")))
								return LEX_BOOL;
							else
								break;
						case 'c' :
							if(!(strcmp(bufferino,"cin")))
								return LEX_CIN;
							else if (!(strcmp(bufferino,"cout")))
								return LEX_COUT;
							else
								break;
						case 'd' :
							if(!(strcmp(bufferino,"do")))
								return LEX_DO;
							else if (!(strcmp(bufferino,"double")))
								return LEX_DOUBLE;
							else
								break;
						case 'e' :
							if(!(strcmp(bufferino,"else")))
								return LEX_ELSE;
							else
								break;
						case 'f' :
							if(!(strcmp(bufferino,"for")))
								return LEX_FOR;
							else if (!(strcmp(bufferino,"false")))
								return LEX_BOOL_F;
							else
								break;
						case 'i' :
							if(!(strcmp(bufferino,"int")))
								return LEX_INT;
							else if (!(strcmp(bufferino,"if")))
								return LEX_IF;
							else
								break;
						case 's' :
							if(!(strcmp(bufferino,"string")))
								return LEX_STRING;
							else
								break;
						case 't' :
							if(!(strcmp(bufferino,"true")))
								return LEX_BOOL_T;
							else
								break;
						case 'r' :
							if(!(strcmp(bufferino,"return")))
								return LEX_RETURN;
							else
								break;
						case 'w' :
							if(!(strcmp(bufferino,"while")))
								return LEX_WHILE;
							else
								break;
						default :
							break;
					}
					
					identifier = bufferino;
					return LEX_ID;
					
				}
			}
				break;
		}
	}
	return 0; 
}

