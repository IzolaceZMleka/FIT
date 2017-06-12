/* ***************************************************************** */
/*
	projekt: Implementace interpretu imperativniho jazyka IFJ15
	autori: Jan Velecky, Adam Piecek
	xvelec07: Jan Velecky
	xzahra24: Tomas Zahradnicek
	xpiece00: Adam Piecek
	xpavli78: Jan Pavlica
*/
/* ***************************************************************** */
#define RUNTIME_ENV // davame na vedomi, ze tento modul se pouziva behem interpretace
#include <stdlib.h>
#include <string.h>
#include "ial.h"
#include "builtin_func.h"
#include "errors.h"

Tstring * sort(const Tstring * const string) {
	Tstring * arr = allocString(string->len); // alokace pro kopii retezce
	
	if(arr) {
		memcpy(arr->str, string->str, string->len + 1); // duplikace retezce
		QuickSort(arr->str, 0, string->len - 1);
	} else
		errored(E_INTER);
	
	return arr;
}

int __attribute__((hot)) find(const Tstring * const text, const Tstring * const pattern) { 
	if(pattern->len == 0)
		return 0;
	if(text->len < pattern->len)
		return -1;
	return Bma(text->str, text->len, pattern->str, pattern->len);
}

Tstring * substr(const Tstring * const string, int i, int n) {
	int stringLen = string->len;
	if((i < 0) || (i > stringLen))
		errored(E_RUN_OTH);
	if((n < 0) || ((n + i) > stringLen))
		n = stringLen - i;

	Tstring * arr = allocString(n);

	if(arr){
		memcpy(arr->str, &string->str[i], n);
		arr->str[n] = '\0';
	}
	else
		errored(E_INTER);

	return arr;
}

Tstring * concat(const Tstring * const string1, const Tstring * const string2) {
	Tstring * arr = allocString(string1->len + string2->len);

	if(arr){
		memcpy(arr->str, string1->str, string1->len);
		memcpy(&arr->str[string1->len], string2->str, string2->len + 1);
	}
	else
		errored(E_INTER);

	return arr;
}

