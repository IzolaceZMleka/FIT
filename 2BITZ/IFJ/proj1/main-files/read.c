/* ***************************************************************** */
/*
	projekt: Implementace interpretu imperativniho jazyka IFJ15
	autori: Adam Piecek
	xvelec07: Jan Velecky
	xzahra24: Tomas Zahradnicek
	xpiece00: Adam Piecek
	xpavli78: Jan Pavlica
*/
/* ***************************************************************** */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "errors.h"
#include "read.h"

#define BUFFER_SIZE 10

#define bufferCheck() \
	    if (bufferSize == i){ \
			bufferSize = bufferSize * 2;\
			buffer = (char *) realloc(buffer, sizeof(char) * bufferSize + 1);\
			if (buffer == NULL){\
				free(buffer);\
				errored(E_INTER);}}\

#define writeToBuffer() \
		bufferCheck();\
		buffer[i] = c;\
		i++;\

#define digitOrError() \
		if(isdigit(c)){\
			writeToBuffer();}\
		else\
			errored(E_RUN_LOAD_NUM);\

char * bufferMalloc (int size){
    char * buffer = (char *) malloc(sizeof(char) * size + 1);
    if (buffer != NULL)
        return buffer;
    else{
        errored(E_INTER);
		return NULL;
	}
}

double readDouble(){
	char * buffer = bufferMalloc(BUFFER_SIZE);
	int bufferSize = BUFFER_SIZE;
	int i = 0;
	int state = S_DIGIT;
	char c;
	bool wasDot = false;
	bool wasExp = false;
	bool whiteSpace = false; // prazdny znak uprostred retezce byt nemuze -> porusila by se definice double
	//				= true;		prazdny znak byt muze -> retezec definici double splni
	bool holdSwitch;
	bool finish = false;


	while((c = getchar()) != EOF){ // cela cast
		if(!isspace(c)){
			digitOrError();
			break;
		}
	}

	while((c = getchar()) != EOF && (!isspace(c))){
		holdSwitch = true;
		while(holdSwitch){
			switch(state){
				case S_DIGIT:
					if(isdigit(c)){
						writeToBuffer();
						holdSwitch = false;
					}
					else if(c == '.' && (!wasDot))
						state = S_DOT;
					else if(((c == 'e') || (c == 'E')) && (!wasExp))
						state = S_EXP;
					else if(wasDot || wasExp){
						ungetc(c, stdin);
						holdSwitch = false;
						finish = true;
						break;
					}
					else
						errored(E_RUN_LOAD_NUM);
					break;
				case S_DOT:
					writeToBuffer();
					wasDot = true;
					state = S_DIG_OR_ER;
					holdSwitch = false;
					break;
				case S_EXP:
					writeToBuffer();
					wasExp = true;
					whiteSpace = false;
					state = S_SIGN;
					holdSwitch = false;
					break;
				case S_SIGN:
					if((c == '-') || (c == '+')){
						writeToBuffer();
						holdSwitch = false;
						state = S_DIG_OR_ER;
					}
					else
						state = S_DIG_OR_ER;
					break;
				case S_DIG_OR_ER:
					digitOrError();
					whiteSpace = true;
					state = S_DIGIT;
					holdSwitch = false;
					break;
			}
		}
		if(finish)
			break;
	}

	if(!whiteSpace){
		errored(E_RUN_LOAD_NUM);
	}

	double result;
	char * newBuffer = bufferMalloc(i); // velikost newBuffer bude presne velikost nacteneho double
	memcpy(newBuffer, buffer, i);
	newBuffer[i] = '\0';
	result = strtod(newBuffer, NULL);
	free(buffer);
	free(newBuffer);
    return result;
}

int readInteger(){
	int res = 0;
	int c;
	bool zeroIn = false;

	while((c = getchar()) != EOF){
		if(!isspace(c)){
			if(c == '0')
				zeroIn = true;
			do{
				if(isdigit(c)){
					res *= 10;
					res += c - '0';
				}
				else{
					ungetc(c, stdin);
					break;
				}
			} while((c = getchar()) != EOF && (!isspace(c)));
			break;
		}
	}
	
	if((res == 0) && (!zeroIn))
		errored(E_RUN_LOAD_NUM);

	return res;
}

Tstring * readString(){
	char * buffer = bufferMalloc(BUFFER_SIZE);
	int bufferSize = BUFFER_SIZE;
	int i = 0;
	char c;

	while ((c = getchar()) != EOF){
		if(!isspace(c)){
			do {
				writeToBuffer();
			} while((c = getchar()) != EOF && (!isspace(c)));
			ungetc(c, stdin); // vraceni prazdneho, nebo ukoncujiciho znaku
			break;
		}
	}

	Tstring * string = allocString(i);
	memcpy(string->str, buffer, i);
	string->str[i] = '\0';
	free(buffer);
    return string;
}
