
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../main-files/builtin_func.h"

Tstring* addTstring(char* seqString)
{
    int seqLen = strlen(seqString);
    Tstring* s = NULL;
    s = allocString((size_t)seqLen);
    if(s == NULL){
        fprintf(stderr,"Chyba pri alokaci \n");
        return NULL;
    }
    strcpy(s->str, seqString); // duplikace retezce
    return s;
}

void testSubstr(int number, char text[], char right[], int i, int n){

	Tstring* string1 = NULL;
	Tstring* result = NULL;

    printf("\nTest[%d]\n", number);
    printf("---------------\n");
    printf("%s\n", right);
    string1 = addTstring(text);
	result = substr(string1, i, n);
    printf("%s, %zu\n", result->str, result->len);
	if(result->str[result->len] != '\0')
	   printf("Chybi ukoncovaci znak!\n");	
}

void testConcat(int number, char text1[], char text2[], char right[]){
	Tstring* string1 = NULL;
	Tstring* string2 = NULL;
	Tstring* result;

    printf("\nTest[%d]\n", number);
    printf("---------------\n");
    printf("%s\n", right);
    string1 = addTstring(text1);
    string2 = addTstring(text2);
	result = concat(string1, string2);
    printf("%s, %zu\n", result->str, result->len);
	if(result->str[result->len] != '\0')
	   printf("Chybi ukoncovaci znak!\n");	

}

void testLength(int number, char text1[], char right[]) {
	Tstring* string = NULL;
	int result;

    printf("\nTest[%d]\n", number);
    printf("---------------\n");
    printf("%s\n", right);
    string = addTstring(text1);
	result = length(string);
    printf("%d\n", result);
}


int main(int argc, char * argv[])
{
	int choose;

	if(argc == 1){ // testy s validnimi vstupy
		printf("\n--------TEST valid--------\n");
		printf("\n--------TEST Length--------\n");
		testLength(1, "ahoj vsichni", "12");
		printf("\n--------TEST Substring--------\n");
		// obycejne pouziti - horni okraj
		testSubstr(1, "ahoj vsichni", "vsichni, 7", 5, 7);
		// obycejce pouziti - dolni okraj
		testSubstr(2, "ahoj vsichni", "ahoj, 4", 0, 4);
		// prostredek
		testSubstr(3, "ahoj vsichni", "oj vsi, 6", 2, 6);
		// 0, 0
		testSubstr(4, "ahoj vsichni", """, 0", 0, 0);
		// zaporna delka
		testSubstr(5, "ahoj vsichni", "ahoj vsichni, 12", 0, -1);
		testSubstr(6, "ahoj vsichni", "vsichni, 7", 5, -1);
		// moc velka delka
		testSubstr(7, "ahoj vsichni", "ahoj vsichni, 12", 0, 20);
		// index + delka je mimo rozsah
		testSubstr(8, "ahoj vsichni", "vsichni, 7", 5, 9);
		// prazdny retezec
		testSubstr(9, "", """, 0", 0, 0);
		// index je stejne velky jako delka retezce
		testSubstr(10, "ahoj vsichni", """, 0", 12, 8);
		printf("\n--------TEST Concat--------\n");
		testConcat(1, "ahoj ", "vsichni", "ahoj vsichni, 12");
		testConcat(2, "ahoj", "", "ahoj, 4");
		testConcat(3, "", "vsichni", "vsichni, 7");
		testConcat(4, "vsichni", "", "vsichni, 7");
		testConcat(5, "", "", """, 0");
		printf("\n--------TEST valid konec--------\n");
	}
	else { // testy se spatnymi vstupy - musi vzniknout chyba
		choose = atoi(argv[1]);
		switch(choose){
			case 1: // zaporny index
				testSubstr(1, "ahoj vsichni", "10", -1, 4);
			case 2: // moc velky index
				testSubstr(2, "ahoj vsichni", "10", 13, 1);
				break;
			default:
				printf("argument je mimo rozsah\n");
				return 1;
		}
		printf("Musi vzniknout chyba!\n");
	}
}
