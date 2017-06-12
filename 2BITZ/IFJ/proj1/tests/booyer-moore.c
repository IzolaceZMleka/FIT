#include <stdio.h>
#include <string.h>
#include "../main-files/ial.h"
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

int main()
{
    Tstring* sP = NULL;
    Tstring* schP = NULL;
    char* text = NULL;
    char* pattern = NULL;

	printf("\n--------TEST Booyer-Moorea--------\n");

    printf("\nTest[1]\n");
    printf("---------------\n");
    text = "abracaddbracadabracadabra";
    pattern = "abracadabra";
    printf("Spravne je: 14\n");
    sP = addTstring(text);
    schP = addTstring(pattern);
    printf("%d\n", find(sP, schP));

    printf("\nTest[2]\n");
    printf("---------------\n");
    text =    "abracadabra";
    pattern = "abracadabra";
    printf("Spravne je: 0\n");
    sP = addTstring(text);
    schP = addTstring(pattern);
    printf("%d\n", find(sP, schP));

    printf("\nTest[3]\n");
    printf("---------------\n");
    text =    "ratata cunata";
    pattern = "ta";
    printf("Spravne je: 2\n");
    sP = addTstring(text);
    schP = addTstring(pattern);
    printf("%d\n", find(sP, schP));

    printf("\nTest[4]\n");
    printf("---------------\n");
    text =        "ta";
    pattern = "ratata cunata";
    printf("Spravne je: -1\n");
    sP = addTstring(text);
    schP = addTstring(pattern);
    printf("%d\n", find(sP, schP));

    printf("\nTest[5]\n");
    printf("---------------\n");
    text =  "Bozena zkourena hleda korena";
    pattern = "";
    printf("Spravne je: 0\n");
    sP = addTstring(text);
    schP = addTstring(pattern);
    printf("%d\n", find(sP, schP));

    printf("\nTest[6]\n");
    printf("---------------\n");
    text =  "Strcili prst do zasuvky";
    pattern = "a"; // á ?
    printf("Spravne je: 17\n");
    sP = addTstring(text);
    schP = addTstring(pattern);
    printf("%d\n", find(sP, schP));

    printf("\nTest[7]\n");
    printf("---------------\n");
    text =  "I met my soulmate...But she didn't";
    pattern = "happiness";
    printf("Spravne je: -1\n");
    sP = addTstring(text);
    schP = addTstring(pattern);
    printf("%d\n", find(sP, schP));

    printf("\nTest[8]\n");
    printf("---------------\n");
    text =  "ahoj";
    pattern = "ahoj";
    printf("Spravne je: 0\n");
    sP = addTstring(text);
    schP = addTstring(pattern);
    printf("%d\n", find(sP, schP));

    printf("\nTest[9]\n");
    printf("---------------\n");
    text =  "a";
    pattern = "a";
    printf("Spravne je: 0\n");
    sP = addTstring(text);
    schP = addTstring(pattern);
    printf("%d\n", find(sP, schP));

    printf("\nTest[10]\n");
    printf("---------------\n");
    text =  "bbaabaaba";
    pattern = "abaaba";
    printf("Spravne je: 3\n");
    sP = addTstring(text);
    schP = addTstring(pattern);
    printf("%d\n", find(sP, schP));

    printf("\nTest[11]\n");
    printf("---------------\n");
    text =  "bbaabaaba aba";
    pattern = " ";
    printf("Spravne je: 9\n");
    sP = addTstring(text);
    schP = addTstring(pattern);
    printf("%d\n", find(sP, schP));

    printf("\nTest[12]\n");
    printf("---------------\n");
    text =  "akcxi vlakcnx vlakakcni vlak";
    pattern = "akcni vlak";
    printf("Spravne je: 18\n");
    sP = addTstring(text);
    schP = addTstring(pattern);
    printf("%d\n", find(sP, schP));

    printf("\nTest[13]\n");
    printf("---------------\n");
    text =  "kuskusuxkuskusu kuskusu kux";
    pattern = "kuskusu kus";
    printf("Spravne je: 8\n");
    sP = addTstring(text);
    schP = addTstring(pattern);
    printf("%d\n", find(sP, schP));

    printf("\nTest[14]\n");
    printf("---------------\n");
    text =  "abcaxabxabaxabaxcabxbcabcabxxxabxab";
    pattern = "abcab";
    printf("Spravne je: 22\n");
    sP = addTstring(text);
    schP = addTstring(pattern);
    printf("%d\n", find(sP, schP));

    printf("\nTest[15]\n");
    printf("---------------\n");
    text =    "xbcabcab";
    pattern = "abcab";
    printf("Spravne je: 3\n");
    sP = addTstring(text);
    schP = addTstring(pattern);
    printf("%d\n", find(sP, schP));

    printf("\nTest[16]\n");
    printf("---------------\n");
    text =    "Toto je nejaky text, ktery jeste trochu obohatime";
    pattern = "text";
    printf("Spravne je: 15\n");
    sP = addTstring(text);
    schP = addTstring(pattern);
    printf("%d\n", find(sP, schP));
	printf("----Konec testu----\n");

}
